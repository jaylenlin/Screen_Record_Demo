#pragma once
#include "IWXConfDefine.h"

namespace wxvoipsdk
{

// 多人通话回调接口
class IWXConfCallback
{
public:
    // 事件通知
    virtual int OnConfEvent(WXConfEvent eventType, int errCode, void *pParamBuf, int bufLen) = 0;

    // 收到视频数据, iOS平台专用
    virtual void OnVideoFrame(int memberId, const unsigned char *pData, int dataLen, int width, int height, int format) = 0;

    // 仅用于外部日志
    virtual void OnWriteLog(int level, const char *tag, const char *filename, int line, const char *funcname, const char *data, uint32_t len) {}
};

} // namespace wxvoipsdk
