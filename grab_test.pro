#-------------------------------------------------
#
# Project created by QtCreator 2016-09-06T16:07:48
#
#-------------------------------------------------

QT       += core gui widgets x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = grab_test
TEMPLATE = app

SOURCES += main.cpp\
        maintest.cpp\
        xgrab.cpp\
        Logger.cpp\
        AV/Output/BaseEncoder.cpp\
        AV/Output/Muxer.cpp\
        AV/Output/VideoEncoder.cpp\
        ConfService.cpp\
        H264FileUtils.cpp\
        include/wxconfsdk.pb.cc

HEADERS  += maintest.h \
        md5.h \
        xgrab.h \
        global.h \
        Logger.h \
        TempBuffer.h \
        AV/Output/BaseEncoder.h \
        AV/Output/Muxer.h \
        AV/Output/VideoEncoder.h \
        AV/Output/OutputSettings.h\
        ConfService.h\
        H264FileUtils.h\
        include/IWXConfService.h\
        include/IWXConfCallback.h\
        include/IWXConfDefine.h\
        include/IWXPubDefine.h\
        include/wxconfsdk.pb.h\
        queue.h

FORMS    += maintest.ui

DEFINES += SSR_USE_X86_ASM=1 SSR_USE_FFMPEG_VERSIONS=1

LIBS += -pthread -lcrypto
LIBS += -lX11 -lXext -lXfixes -lavformat -lavcodec -lavutil -lswscale -lprotobuf
LIBS += -L"$$PWD/lib" -lconfService\
        -lmarsbridgenetwork\
        -lvoipChannel\
        -lvoipCodec

INCLUDEPATH += $$PWD/include AV AV/Input AV/Output
