#pragma once
#include "IWXConfService.h"
#include "IWXConfDefine.h"
#include "IWXPubDefine.h"
#include "IWXConfCallback.h"
#include "wxconfsdk.pb.h"
#include <thread>
#include <mutex>
#include "queue.h"
#include "xgrab.h"

namespace wxvoipsdk{
class ConfCallService : public IWXConfCallback
{
public:
    ConfCallService();
    ~ConfCallService();

public:
    int Init(const std::string &deviceId, const std::string &groupId, const std::string &sessionKey,flame_queue *flamequeue = NULL);
    void Uninit();

    void JoinRoom();
    void ExitRoom(ConfExitReason reason);
    bool IsTalking(){ return m_bTalking; }
    bool IsCalling() { return m_bCalling; }


protected:
    virtual int OnConfEvent(WXConfEvent eventType, int errCode, void *pParamBuf, int bufLen);
    virtual void OnVideoFrame(int memberId, const unsigned char *pData, int dataLen, int width, int height, int format) {};


    void StartDataThread();
    void StopDataThread();

    IWXConfService *m_pService;
    bool m_bInited;
    bool m_bCalling;
    bool m_bTalking;
    uint32_t m_selfMemberId;

    uint64_t m_imRoomId;
    std::string m_groupId;

    std::mutex m_mutex;
    ConfMemberList m_MemberList;
    ConfAVMemberList m_AVMemberList;

    static void *VideoSendThreadFunc(void *pData);
    static void *VideoRecvThreadFunc(void *pData);

    bool m_bStudent;
    std::string m_h264File;

    std::thread m_hThreadSendVideo;
    std::thread m_hThreadRecvVideo;

    flame_queue *m_frame_queue;

};
}
