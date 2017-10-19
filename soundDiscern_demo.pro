#-------------------------------------------------
#
# Project created by QtCreator 2017-09-04T14:53:25
#
#-------------------------------------------------

QT       += core gui sql multimedia charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = soundDiscern_demo
TEMPLATE = app


SOURCES += main.cpp\
    soundsdata_db.cpp \
    utils/audiotool.cpp \
    pcmwaveform_widget.cpp \
    utils/rpc_kvp_tool.cpp \
    utils/ftpclient.cpp \
    registrantitem_widget.cpp \
    masklabel.cpp \
    voicelibtable_widget.cpp \
    mainwindow.cpp \
    voiceidentification_win.cpp \
    voicelib_win.cpp \
    voiceregistration_win.cpp

HEADERS  += \
    soundsdata_db.h \
    utils/audiotool.h \
    pcmwaveform_widget.h \
    utils/rpc_kvp_tool.h \
    utils/test_rpc.h \
    utils/ftpclient.h \
    registrantitem_widget.h \
    masklabel.h \
    voicelibtable_widget.h \
    mainwindow.h \
    voiceidentification_win.h \
    voicelib_win.h \
    voiceregistration_win.h

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

