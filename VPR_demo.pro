#-------------------------------------------------
#
# Project created by QtCreator 2017-09-04T14:53:25
#
#-------------------------------------------------

QT       += core gui sql multimedia charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VPR_demo
TEMPLATE = app


SOURCES += main.cpp\
    soundsdata_db.cpp \
    utils/audiotool.cpp \
    pcmwaveform_widget.cpp \
    utils/ftpclient.cpp \
    registrantitem_widget.cpp \
    masklabel.cpp \
    voicelibtable_widget.cpp \
    mainwindow.cpp \
    voiceidentification_win.cpp \
    voicelib_win.cpp \
    voiceregistration_win.cpp \
    utils/qftp.cpp \
    utils/qurlinfo.cpp \
    utils/logger.cpp \
    utils/httpsender.cpp \
    rpc/xbusiness_aid.c \
    rpc/xbusiness_vad.c \
    configparser.cpp \
    rpc/rpcproxy.cpp

HEADERS  += \
    soundsdata_db.h \
    utils/audiotool.h \
    pcmwaveform_widget.h \
    utils/ftpclient.h \
    registrantitem_widget.h \
    masklabel.h \
    voicelibtable_widget.h \
    mainwindow.h \
    voiceidentification_win.h \
    voicelib_win.h \
    voiceregistration_win.h \
    configparser.h \
    utils/qftp.h \
    utils/qurlinfo.h \
    utils/logger.h \
    utils/httpsender.h \
    rpc/asv_rpc.h \
    rpc/test_rpc.h \
    rpc/xbusiness_vpr.h \
    rpc/rpcproxy.h

FORMS    += \
    pcmwaveform_widget.ui \
    registrantitem_widget.ui \
    voicelibtable_widget.ui \
    mainwindow.ui \
    voiceidentification_win.ui \
    voicelib_win.ui \
    voiceregistration_win.ui

RESOURCES += \
    registrants.qrc

LIBS += -L../lib/ -lkst_sdk \
