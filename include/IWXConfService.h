// Wechat VoIP SDK

#pragma once
#include "IWXConfDefine.h"
#include "IWXConfCallback.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define WXVOIP_CALL __cdecl
#if defined(VOIPSDK_EXPORTS)
#define WXVOIP_API extern "C" __declspec(dllexport)
#else
#define WXVOIP_API extern "C" __declspec(dllimport)
#endif
#elif defined(__APPLE__)
#define WXVOIP_API __attribute__((visibility("default"))) extern "C"
#define WXVOIP_CALL
#elif defined(__ANDROID__) || defined(__linux__)
#define WXVOIP_API extern "C" __attribute__((visibility("default")))
#define WXVOIP_CALL
#else
#define WXVOIP_API extern "C"
#define WXVOIP_CALL
#endif

#define WXCONFSDK_INTERFACE_VERSION   20190731

namespace wxvoipsdk
{

// 多人通话SDK主接口
class IWXConfService
{
public:
    // 初始化（envInfo为ConfEnvInfo结构的pb序列化）
    virtual int Init(IWXConfCallback *pCallback, const unsigned char *pEnvInfo, int envInfoLen) = 0;

    // 反初始化
    virtual int Uninit() = 0;

    // 进入房间
    virtual int JoinRoom(uint64_t imRoomId, VoipRoomType roomType, VoipNetType netType) = 0;
    virtual int JoinRoom(const char *pGroupId, VoipRoomType roomType, VoipNetType netType) = 0;

    // 退出房间
    virtual int ExitRoom(ConfExitReason reason) = 0;

    // 发送音频数据
    virtual int SendAudioData(const unsigned char *pData, int dataLen, int playDelayMS) = 0;

    // 接收音频数据
    virtual int GetAudioData(unsigned char *pBuf, int bufLen) = 0;
    
#if defined(__linux__) && !defined(__ANDROID__)
    // input compressed audio frame data: nFlag 0: compressed payload, no-voice detect, 1: compressed payload,voice detected; 2: input pcm data
    virtual int SendAudioPayload(const unsigned char *pData, int nLen, int nFlag, int nFormat) = 0;

    // get compressed audio frame data for each memberid: nFormat: 0 : compressed paylad fmt, 1: pcm fmt payload , 2: mixed pcm fmt payload 
    virtual int GetAudioPayload(const unsigned char *&pBuff, int &nLen, int nMemberID, int nFormat) = 0;
#endif

    // 发送视频数据
    virtual int SendVideoData(const unsigned char *pData, int dataLen, int width, int height, int format) = 0;

    // 接收视频数据(YUV格式，Buffer长度必须至少为xx)
    virtual int GetVideoData(int &memberId, unsigned char *pBuf, int &dataLen, int &width, int &height) = 0;

    // 设置是否开启音频（麦克风）和视频（摄像头）
    virtual int SwitchAV(bool bOpenAudio, bool bOpenVideo) = 0;

    // 设置视频的请求（显示）分辨率，参数为VideoResolution的pb序列化
    virtual int SetVideoResolution(const unsigned char *pVideoResoPb, int pbLen) = 0;

    // 动态设置engine相关参数 notice: java/jni特殊，需要重组一下数据再调
    virtual int SetAppCmd(int type, unsigned char *pParameter, int len) = 0;

    // 更新登录授权Key
    virtual int UpdateAuthKey(const unsigned char *pAuthKey, int authKeyLen) = 0;

    // 网络状态改变
    virtual int OnNetworkChange(VoipNetType netType) = 0;

    // 判断成员是否在说话（1是，0否，<0失败）
    virtual int GetVoiceActivity(int nMemberid) = 0;

    // 本机在播放的其它声音
    virtual int PutExternalplayAudioData(unsigned char* pData, int samplerate, int channels, int samples,int format) = 0;

    // return: member size;
    virtual int GetVADMembers(int* pMemberlist) = 0;

#if !defined(__linux__) || defined(__ANDROID__)
    // 硬编前处理
    virtual int VideoPreEncProcessCloud(unsigned char* pImg,  unsigned char* pDst, int nLen, void* pFormat) = 0;
#endif

protected:
    virtual ~IWXConfService() {};
};

} // namespace wxvoipsdk

WXVOIP_API wxvoipsdk::IWXConfService* WXVOIP_CALL GetWXConfService(int version = WXCONFSDK_INTERFACE_VERSION);

#if defined(__linux__) && !defined(__ANDROID__)  // only for linux server
WXVOIP_API wxvoipsdk::IWXConfService* WXVOIP_CALL GetWXConfServiceSvr(int version = WXCONFSDK_INTERFACE_VERSION);
#endif
