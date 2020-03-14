#include "ConfService.h"
#include "H264FileUtils.h"
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <inttypes.h>
#include <google/protobuf/message.h>
#include <chrono>
#include <sstream>
#include "md5.h"

using namespace wxvoipsdk;

const char *format_time()
{
    struct timeval tv;
    static char buf[28];
    memset(buf, 0, sizeof(buf));
    // clock time
    if (gettimeofday(&tv, NULL) == -1)
    {
        return buf;
    }
    // to calendar time
    struct tm *tm;
    if ((tm = localtime((const time_t *) &tv.tv_sec)) == NULL)
    {
        return buf;
    }
    snprintf(buf, sizeof(buf),
             "%d-%02d-%02d %02d:%02d:%02d.%06d",
             1900 + tm->tm_year, 1 + tm->tm_mon, tm->tm_mday,
             tm->tm_hour, tm->tm_min, tm->tm_sec,
             (int) (tv.tv_usec));
    buf[sizeof(buf) - 1] = 0;
    return buf;
}

#define ctrace(msg, ...) \
    fprintf(stdout, "[T][%s][%d][%d][%s] ", __FILE__,__LINE__,getpid(), format_time());\
    fprintf(stdout, msg, ##__VA_ARGS__); fprintf(stdout, "\n")

class DeltaClock
{
public:
    DeltaClock(int deltaUs)
    {
        mStart = std::chrono::system_clock::now();
        mCnt = 0;
        mDeltaUs = deltaUs;
    }

    ~DeltaClock()
    {

    }

    int DeltaUs()
    {
        auto nw = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(nw - mStart);
        mCnt += 1;
        auto res = mCnt * mDeltaUs - duration.count();
        if (res < 0 || res > mCnt * mDeltaUs)
        {
            //溢出,或者追不上进度
            return 0;
        }
        return res;
    }

private:
    std::chrono::system_clock::time_point mStart;
    int mCnt;
    int mDeltaUs;
};


ConfCallService::ConfCallService()
{
    m_pService = NULL;
    m_bInited = false;
    m_bCalling = false;
    m_bTalking = false;
    m_imRoomId = 0;
    m_selfMemberId = 0;
}

ConfCallService::~ConfCallService()
{
    if (m_pService != NULL)
    {
        Uninit();
        m_pService = NULL;
    }
}

void ConfCallService::Uninit()
{
    if (m_bInited)
    {
        m_bInited = false;
        m_pService->Uninit();
    }
}

int ConfCallService::Init(const std::string &deviceId, const std::string &groupId, const std::string &sessionKey, flame_queue *flamequeue)
{
    m_groupId = groupId;
    m_frame_queue = flamequeue;
    m_bStudent = deviceId == "student";

    uint32_t audioFlag = 0;

    wxvoipsdk::ConfEnvInfo envInfo;
    envInfo.set_app_id("wx766655dab8fe851b");
    envInfo.set_app_subid("wx522896043edeebda");
    envInfo.set_auth_key(sessionKey);
    envInfo.set_device_id(deviceId.c_str());
    envInfo.set_cpu_core(4);
    envInfo.set_cpu_freq(3000);
    envInfo.set_debug_svr(0);
    envInfo.set_work_dir("/tmp");
    envInfo.set_audio_flag(audioFlag);
    envInfo.set_device_model("iPhone9,2");
    envInfo.set_out_log(1);
    envInfo.set_log_level(2);

    std::string result;
    envInfo.SerializeToString(&result);

    ctrace("Init\n");
    m_pService = GetWXConfService();
    int ret = m_pService->Init(this, (const unsigned char *) result.c_str(), result.size());
    if (ret != 0)
    {
        printf("init fail ret %d\n", ret);
        return ret;
    }

    m_bInited = true;
    m_bCalling = false;
    m_bTalking = false;

    return 0;
}

void ConfCallService::JoinRoom()
{
    ctrace("JoinRoom, m_imRoomId[%" PRIu64 "]\n", m_imRoomId);
    if(m_imRoomId > 0)
        m_pService->JoinRoom(m_imRoomId, ROOM_TYPE_VIDEO, NET_TYPE_WIFI);
    else
        m_pService->JoinRoom(m_groupId.c_str(), ROOM_TYPE_VIDEO, NET_TYPE_WIFI);
}

void ConfCallService::ExitRoom(ConfExitReason reason)
{
    ctrace("ExitRoom, reason %d\n", (int) reason);
    m_pService->ExitRoom(reason);
}

static const char *GetConfEventName(WXConfEvent eventType)
{
    static const struct
    {
        WXConfEvent event;
        const char *pName;
    } eventMap[] =
    {
        WXCONF_EVENT_INIT_SUCC, "InitSucc",
                WXCONF_EVENT_INIT_FAIL, "InitFail",
                WXCONF_EVENT_JOIN_SUCC, "JoinSucc",
                WXCONF_EVENT_JOIN_FAIL, "JoinFail",
                WXCONF_EVENT_MEMBER_CHANGE, "MemberChange",
                WXCONF_EVENT_AVMEMBER_CHANGE, "AVMemberChange",
                WXCONF_EVENT_TALK_SUCC, "TalkSucc",
                WXCONF_EVENT_TALK_FAIL, "TalkFail",
                WXCONF_EVENT_CALL_BROKEN, "CallBroken",
                WXCONF_EVENT_CALL_END, "CallEnd",
    };

    for (int i = 0; i < sizeof(eventMap) / sizeof(eventMap[0]); i++)
    {
        if (eventMap[i].event == eventType)
            return eventMap[i].pName;
    }
    return "Unknown";
}

int ConfCallService::OnConfEvent(WXConfEvent eventType, int errCode, void *pParamBuf, int bufLen)
{
    ctrace("-----OnConfEvent, eventType=%s(%d), errCode=%d\n", GetConfEventName(eventType), eventType, errCode);
    switch (eventType)
    {
    case WXCONF_EVENT_JOIN_SUCC:
    {

        m_bCalling = true;
        ConfRoomInfo roomInfo;
        roomInfo.ParseFromArray(pParamBuf, bufLen);
        m_selfMemberId = roomInfo.self_memberid();
        break;
    }
    case WXCONF_EVENT_JOIN_FAIL:
    {
        break;
    }
    case WXCONF_EVENT_MEMBER_CHANGE:
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_MemberList.ParseFromArray(pParamBuf, bufLen);
        ctrace("imroomid[%" PRIu64 "], member_size[%d]\n", m_MemberList.roomid(), m_MemberList.member_list_size());
        break;
    }
    case WXCONF_EVENT_AVMEMBER_CHANGE:
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_AVMemberList.ParseFromArray(pParamBuf, bufLen);
        ctrace("imroomid[%" PRIu64 "], member_size[%d]\n", m_AVMemberList.roomid(), m_AVMemberList.video_member_list_size());
        break;
    }
    case WXCONF_EVENT_TALK_SUCC:
    {
        m_bTalking = true;
        StartDataThread();
        break;
    }
    case WXCONF_EVENT_CALL_BROKEN:
    {
        m_bTalking = false;
        break;
    }
    case WXCONF_EVENT_CALL_END:
    {
        m_bTalking = false;
        m_bCalling = false;
//        StopDataThread();

        //        ConfDeviceReport reportInfo;
        //        reportInfo.set_audio_player_errcode(1);
        //        reportInfo.set_audio_recorder_errcode(2);
        //        reportInfo.set_phone_mode(3);
        //        reportInfo.set_speaker_mode(4);
        //        // ...

        //        std::string result;
        //        reportInfo.SerializeToString(&result);
        //        if (bufLen >= result.size())
        //        {
        //            memcpy(pParamBuf, result.c_str(), result.size());
        //            return (int) result.size();
        //        }

        break;
    }
    }
    return 0;
}

void *ConfCallService::VideoSendThreadFunc(void *pData)
{
    ConfCallService *pThis = (ConfCallService *) pData;

    MyFrame frame;
    frame.data = new uint8_t[6000000]{0};
    int sendDeltaUs = 30000; // 30ms
    while (pThis->m_bTalking)
    {
        ConfMemberList memberList;
        {
            std::lock_guard<std::mutex> lock(pThis->m_mutex);
            memberList = pThis->m_MemberList;
        }

        if (memberList.member_list_size() <= 1)
        {
            //ctrace("members equal to 1,no need to send");
            usleep(sendDeltaUs * 10);
        }

        while (memberList.member_list_size() > 1)
        {
            pThis->m_frame_queue->wait_and_pop(frame);

            printf("send_md5:%s\n", mymd5(frame.data, frame.length).c_str());
            int ret = pThis->m_pService->SendVideoData(frame.data, frame.length, frame.width, frame.height, VFMT_NV21);
            ctrace("SendVideoData ONCE, ret=%d, length=%d, width=%d, height:%d\n",  ret, frame.length, frame.width, frame.height);
            if (ret < 0)
            {
                ctrace("send member %u video data error,ret=%d", pThis->m_selfMemberId, ret);
                break;
            }
        }
    }
    ctrace("send video end");
    delete[] frame.data, frame.data = nullptr;
}

void *ConfCallService::VideoRecvThreadFunc(void *pData)
{
    ctrace("recv h264 file");
    ConfCallService *pThis = (ConfCallService *) pData;
    unsigned char *pBuf = nullptr;
    int sz = 0;
    int w = 0, h = 0;
    int memberId = 0;
    int recvDeltaUs = 30000;
    ctrace("begin to recv video data");
    int size = 640*480*4*4;
    int cnt = 0;

    pBuf = new unsigned char[size]{0};	//bugfix:安卓模式的硬解需要预先开好内存
    while (pThis->m_bTalking)
    {
        pBuf[0] = ((cnt++)%2)==0?0x0:0x1;
        ConfMemberList memberList;
        {
            std::lock_guard<std::mutex> lock(pThis->m_mutex);
            memberList = pThis->m_MemberList;
        }

        if (memberList.member_list_size() <= 1)
        {
//            ctrace("members equal to 1,no need to recv");
            usleep(recvDeltaUs);
            continue;
        }

        DeltaClock clk(recvDeltaUs);
        for (auto i = 0; i < memberList.member_list_size(); i++)
        {
            const ConfMemberInfo &memberInfo = memberList.member_list(i);
            if (memberInfo.memberid() != pThis->m_selfMemberId)
            {
                int ret = pThis->m_pService->GetVideoData(memberId, pBuf, sz, w, h);
                if (ret < 0)
                {
                    ctrace("recv %s:%d memberId:%d, video data error,ret=%d\n", memberInfo.openid().c_str(),
                           memberInfo.memberid(), memberId, ret);
                    continue;
                }
                if(ret>0 && sz>0)
                {
                    std::stringstream ss;
                    ss<<"h264_recv_" << memberInfo.openid()<<"_"<<memberInfo.memberid() << ".264";
                    H264FileWriter writer(ss.str());
                    writer.WriteNalu(pBuf,sz);
                    ctrace("recv %s:%d video data success,buf size:%d", memberInfo.openid().c_str(),
                           memberInfo.memberid(), sz);
                }
            }
        }
        usleep(clk.DeltaUs());
    }
    ctrace("recv video end");
    delete[] pBuf, pBuf = nullptr;
}
void ConfCallService::StartDataThread()
{
    if( m_bStudent ){
        m_hThreadSendVideo = std::thread(VideoSendThreadFunc, this);
    } else {
        m_hThreadRecvVideo = std::thread(VideoRecvThreadFunc, this);
    }


}
