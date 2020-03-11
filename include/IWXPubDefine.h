// 双人和多人SDK共用的定义
#pragma once

namespace wxvoipsdk
{

enum VoipVideoFormat
{
    VFMT_UNKNOWN    = 0,
    VFMT_YUV420     = 1,
    VFMT_RGB32      = 4,    
    VFMT_NV21       = 7,  // Android
    VFMT_NV12       = 10, // iOS
    VFMT_i264       = 18,
    VFMT_i265       = 19,
    VFMT_ROTATE     = 32,
    VFMT_MIRROR     = 64,
};

enum VoipNetType
{
    NET_TYPE_NULL   = 0,
    NET_TYPE_2G     = 1,
    NET_TYPE_3G     = 3,
    NET_TYPE_WIFI   = 4,
    NET_TYPE_4G     = 5,
};

enum VoipRoomType
{
    ROOM_TYPE_UNKNOWN = 0,
    ROOM_TYPE_VIDEO = 1,      // 音视频通话
    ROOM_TYPE_AUDIO = 2,      // 纯音频通话
};

// 用于通话结束后上报，音频播放启动错误码
enum AudioPlayErrCode
{
    PLAYER_SUCCESS = 0,                 // 播放设备启动成功
    PLAYER_MINBUFFER_ERROR = 1,         // AudioTrack获取MinBufferSize失败
    PLAYER_NEWAUDIOTRACK_ERROR = 2,     // AudioTrack初始化返回NULL或状态为STATE_UNINITIALIZED
    PLAYER_SECNEWAUDIOTRACK_ERROR = 3,  // AudioTrack状态为STATE_UNINITIALIZED
    PLAYER_PLAY_ERROR = 4,              // AudioTrack启动后获取状态为PLAYER_PLAY_ERROR
    PLAYER_WRITERET_ERROR = 5,          // AudioTrack.write错误计数累计>=50次，记录异常
    PLAYER_NEWAUDIOTRACK_EXCEPTION = 6, // AudioTrack初始化捕获到异常
    PLAYER_GETPLAYBACKHEADPOSITION_EXCEPTION = 7, // AudioTrack.getPlaybackHeadPosition()调用捕获到异常
};

// 用于通话结束后上报，音频录音启动错误码
enum AudioRecordErrCode
{
    RECORDER_SUCCESS = 0,               // 成功
    RECORDER_MINBUFFER_ERROR = 1,       // 调用AudioRecord.getMinBufferSize返回错误
    RECORDER_NEWAUDIORECORD_ERROR = 2,  // 构造AudioRecord失败，这次失败的时候，不会回调onError。还会再次尝试构造AudioRecord
    RECORDER_SECNEWAUDIORECORD_ERROR = 3,// 第二次构造AudioRecord失败，这次会回调onError
    RECORDER_STARTRECORDING_ERROR = 4,  // 调用AudioRecord.startRecording后，AudioRecord的状态未能变为AudioRecord.RECORDSTATE_RECORDING。 即启动失败
    RECORDER_READRET_ERROR = 5,         // AudioRecord.read返回值小于0
    RECORDER_DATAZERO_ERROR = 6,        // 检查到录音数据超过100帧的数据值为全0
    RECORDER_DISTORTION_ERROR = 7,      // 检查到语音数据失真
    RECORDER_DATAZERO_DISTORTION_ERROR = 8, // 组合值：RECORDER_DATAZERO_ERROR & RECORDER_DISTORTION_ERROR
    RECORDER_DATAZERO_READRET_ERROR = 9,    // 组合值：RECORDER_DATAZERO_ERROR & RECORDER_READRET_ERROR
    RECORDER_DISTORTION_READRET_ERROR = 10, // 组合值：RECORDER_DISTORTION_ERROR & RECORDER_READRET_ERROR
    RECORDER_DATAZERO_DISTORTION_READRET_ERROR = 11,// 组合值：RECORDER_READRET_ERROR & RECORDER_DATAZERO_ERROR & RECORDER_DISTORTION_ERROR
    RECORDER_NEW_AUDIORECORD_EXCEPTION = 12,// new AudioRecord失败
    RECORDER_OPMARKEVENT_NEEDSHOWDLG = 13,  // 需要弹出tips，不去启动AudioRecord
    RECORDER_MICROPHONE_MUTE_ERROR = 14,    // 判断 MicroPhone 为 mute 状态
};

//音视频setAppCmd接口cmd类型的定义
enum AVCmdMethodType
{
   EMethodGetQosPara    = 25,
   EMethodSetSpeakerphoneOn  = 401, //打开扬声器
   EMethodSetSpeakerphoneOff = 402, //关闭扬声器切到听筒播放
   EMethodGraspMicAttempt    = 403,
   EMethodSetAgcOn           = 404,     //agc 开关；
   EMethodSetAgcOff          = 405,
   EMethodSetAecmOn          = 406,
   EMethodSetAecmOff         = 407,
   EMethodSetNsOn            = 408,
   EMethodSetNsOff           = 409,
   EMethodMicRecDistSmall    = 410,
   EMethodUsingRemoteIO      = 411,
   EMethodMicDisable         = 412,
   EMethodMicEnable          = 413,

   EMethodInputVolumeScaleEnable  = 414,
   EMethodOutputVolumeScaleEnable = 415,
   
   EMethodEhanceHeadsetEC          = 416,
   EMethodSetECModeLevelForHeadSet = 417,
   EMethodSetECModeLevelForSpeaker = 418,
   EMethodEnableSpeakerEnhanceEC   = 419,

   EMethodSetPlayerPreCorrectCofOn  = 420,
   EMethodSetPlayerPreCorrectCofOff = 421,
   EMethodOutputVolumeGainEnable    = 422,
   EMethodSetNgStrength             = 423,
   EMethodSetSpkEnhance             = 424,
   EMethodHeadSetPlug               = 425,
   EMethodSetAgcRxOn                = 426,
   EMethodPcMicLevel                = 427,
   EMethodSetIosMicAbCheckOn        = 428,
   EMethodSetIosMicAbCheckOff       = 429,
   EMethodGetIosMicAbCheckFlag      = 430,
   EMethodSetXnoiseSupFlag          = 431,
   EMethodSetSendToNetworkOn        = 432, //麦克风静音关闭时,设置网络发包开关打开
   EMethodSetSendToNetworkOff       = 433, //麦克风静音打开时,设置网络发包开关关闭,不向发包
   EMethodMicmuteSendPktCtrl        = 434, //服务器控制开关
   EMethodSetDebugDump         = 908,  //sdk dump debug
};

enum AudioInputFormat
{
    AFORMAT_IN_COMPRESSED_STREAM = 0,  //compressed stream(silk, aac etc.)
    AFORMAT_IN_STEREO_PCM  = 1,        //stereo pcm format
    AFORMAT_IN_MONO_PCM    = 2,        //mono pcm format
    
};
enum AudioOutputFormat
{
    AFORMAT_OUT_COMPRESSED_STREAM  = 0, //compressed stream(silk, aac etc.)
    AFORMAT_OUT_CHANNEL_PCM  = 1,       //pcm for each recved member
    AFORMAT_OUT_MIXED_PCM    = 2,       //pcm mixed for all recved member
    
};

struct stCloudVHWParam //视频硬编码参数
{
        unsigned short iKbps;            // 期望发送码率
        unsigned char cFps;                // 期望帧率
        unsigned char cIPeriod;            // I帧间隔
        unsigned char cResolution;        // 分辨率
        unsigned char cCodecType;       //编码类型
        unsigned char cHWEnableFlag;    //硬编能力开关
        unsigned char cSkipFlag;
        unsigned char cIReqFlag;
        unsigned char cRsvd1;
};


} // namespace wxvoipsdk
