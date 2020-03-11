#pragma once
#include <string>
#include "IWXPubDefine.h"

namespace wxvoipsdk
{

enum ConfExitReason
{
    EXIT_REASON_UNKNOWN = 0,
    EXIT_REASON_MANUAL = 1,         // 用户手动挂断

    EXIT_REASON_PHONE_BROKEN = 6,   // 被系统电话中断
    EXIT_REASON_APP_BROKEN = 7,     // 被其它软件中断
    EXIT_REASON_DEVICE_FAIL = 8,    // 采集播放设备启动失败
};

// 多人通话回调事件定义
enum WXConfEvent
{
#if defined(__linux__) && !defined(__ANDROID__)
    WXCONF_EVENT_INIT_SUCC = 1,     // 设备登录成功（参数为自己的openid）
    WXCONF_EVENT_INIT_FAIL,         // 设备登录失败
    WXCONF_EVENT_JOIN_SUCC,         // 加入房间成功
#else
    WXCONF_EVENT_JOIN_SUCC = 1,     // 加入房间成功（参数为ConfRoomInfo的pb序列化）
#endif
    WXCONF_EVENT_JOIN_FAIL,         // 加入房间失败

    WXCONF_EVENT_MEMBER_CHANGE,     // 房间成员列表改变（参数为ConfMemberList的pb序列化）
    WXCONF_EVENT_AVMEMBER_CHANGE,   // 开启了音视频的成员改变（参数为ConfAVMemberList的pb序列化）

    WXCONF_EVENT_TALK_SUCC,         // 开始通话（参数为ConfDeviceParam的pb序列化）
    WXCONF_EVENT_TALK_FAIL,         // 通话失败
    WXCONF_EVENT_CALL_BROKEN,       // 通话中断（通话过程中网络异常）

    WXCONF_EVENT_CALL_END,          // 通话结束（参数填入ConfDeviceReport的pb序列化，返回写入长度）

    WXCONF_EVENT_AUTHKEY_EXPIRED,   // 登录授权Key过期（参数为AuthKey）
}; 

enum WXConfErrCode
{
    WXCONF_ERR_UNKNOWN = 0,         // 其它失败
    WXCONF_ERR_INVALID_CALL,        // 无效调用（时机不对或参数无效）
    WXCONF_ERR_NETWORK_FAIL,        // 网络失败
    WXCONF_ERR_FREQ_LIMITED,        // 频率限制
    WXCONF_ERR_AUTHKEY_EXPIRED,     // 登录授权Key过期
    WXCONF_ERR_NOT_FRIEND,          // 非好友

    // 后台VoIP相关失败
    WXCONF_ERR_ROOM_CLOSED = 10,    // 房间已关闭
    WXCONF_ERR_REMOTE_NOT_SUPPORT,  // 对方版本不支持

    // CloudIM相关失败
    WXCONF_ERR_VENDOR_LOGIN = 50,   // 厂商登录失败
    WXCONF_ERR_DEVICE_LOGIN,        // 设备登录失败
    WXCONF_ERR_JOIN_GROUP,          // 进临时群失败
};

} // namespace wxvoipsdk