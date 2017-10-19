#include "voiceidentification_win.h"
#include "ui_voiceidentification_win.h"
#include "utils/audiotool.h"
#include "pcmwaveform_widget.h"
#include "utils/rpc_kvp_tool.h"
#include <QPainter>
#include <QCryptographicHash>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "soundsdata_db.h"
#include "masklabel.h"

namespace{
    const char* vp_node = "Test_RPC";
}

VoiceIdentification_win::VoiceIdentification_win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VoiceIdentification_win)
{
    ui->setupUi(this);
    ///导航栏
    QLabel *logoLab = new QLabel();
    logoLab->setPixmap(QPixmap("://images/logo.png"));
    QLabel *titleLab = new QLabel("声纹识别");
    QFont font;
    font.setFamily("Microsoft YaHei");
    titleLab->setFont(font);
    titleLab->setStyleSheet("* {color: #3ddcf6; font-size: 20px;}");
    //创建声纹库显示句柄
    m_voiceRegistrationWin_showBtn = new QPushButton(this);
    m_voiceRegistrationWin_showBtn->setStyleSheet("* {background-color: transparent;border-image:url(:/images/voiceRegistration.png); width:76px; height:30px;}* :hover{border-image:url(:/images/voiceRegistration_hover.png);}");
    //创建声纹注册界面显示句柄
    m_voiceLibWin_showBtn = new QPushButton(this);
    m_voiceLibWin_showBtn->setStyleSheet("* {background-color: transparent;border-image:url(:/images/voiceLib.png); width:76px; height:30px;}* :hover{border-image:url(:/images/voiceLib_hover.png);}");
    QHBoxLayout *top_middle_layout = new QHBoxLayout;
    top_middle_layout->addSpacing(34);
    top_middle_layout->addWidget(logoLab);
    top_middle_layout->addWidget(titleLab);
    top_middle_layout->addStretch(1);
    top_middle_layout->addWidget(m_voiceLibWin_showBtn);
    top_middle_layout->addSpacing(28);
    top_middle_layout->addWidget(m_voiceRegistrationWin_showBtn);
    top_middle_layout->addSpacing(34);
    QVBoxLayout *top_layout = new QVBoxLayout;
    top_layout->addStretch(1);
    top_layout->addLayout(top_middle_layout);
    top_layout->addStretch(1);

    ///声纹识别栏
    //创建音频工具
    m_audio = new AudioTool;
    //创建识别人头像图片控件
    m_registrantHeadLab = new MaskLabel(this);
    m_registrantHeadLab->hide();
    m_registrantHeadLab->setFixedSize(80,80);
    QHBoxLayout *bottom_first_layout = new QHBoxLayout;
    bottom_first_layout->addStretch(1);
    bottom_first_layout->addWidget(m_registrantHeadLab);
    bottom_first_layout->addStretch(1);
    //创建声纹识别信息显示控件
    m_identifyInfoLab = new QLabel(this);
    m_identifyInfoLab->setFont(font);
    m_identifyInfoLab->setStyleSheet("color: #4e9cf1;font-size:18px;");
    QHBoxLayout *bottom_second_layout = new QHBoxLayout;
    bottom_second_layout->addStretch(1);
    bottom_second_layout->addWidget(m_identifyInfoLab);
    bottom_second_layout->addStretch(1);
    //创建声纹波形显示控件
    m_PCMWaveform_widget = new PCMWaveform_widget(this);
    m_PCMWaveform_widget->setFixedSize(456,157);
    QHBoxLayout *bottom_third_layout = new QHBoxLayout;
    bottom_third_layout->addStretch(1);
    bottom_third_layout->addWidget(m_PCMWaveform_widget);
    bottom_third_layout->addStretch(1);
    //创建声纹识别按钮
    m_identifyBtn = new QPushButton(this);
    m_identifyBtn->setStyleSheet(QString("* {background-color: transparent;"
                                 "border-radius: %1px;"
                                 "border-image:url(:/images/record.png);"
                                 "width: 184px;"
                                 "height:184px;}"
                                 "* :hover{border-image:url(:/images/record_hover.png);}").arg(m_identifyBtn->width()/2));
    QHBoxLayout *bottom_fourth_layout = new QHBoxLayout;
    bottom_fourth_layout->addStretch(1);
    bottom_fourth_layout->addWidget(m_identifyBtn);
    bottom_fourth_layout->addStretch(1);
    QLabel *identifyLab = new QLabel("开始识别");
    identifyLab->setFont(font);
    identifyLab->setStyleSheet("* {color: #2b85e3; font-size: 20px;}");
    QHBoxLayout *bottom_fifth_layout = new QHBoxLayout;
    bottom_fifth_layout->addStretch(1);
    bottom_fifth_layout->addWidget(identifyLab);
    bottom_fifth_layout->addStretch(1);
    QVBoxLayout *bottom_layout = new QVBoxLayout;
    bottom_layout->addSpacing(40);
    bottom_layout->addLayout(bottom_first_layout);
    bottom_layout->addSpacing(5);
    bottom_layout->addLayout(bottom_second_layout);
    bottom_layout->addLayout(bottom_third_layout);
    bottom_layout->addLayout(bottom_fourth_layout);
    bottom_layout->addLayout(bottom_fifth_layout);
    bottom_layout->addStretch(1);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(top_layout);
    main_layout->addLayout(bottom_layout);
    main_layout->setStretchFactor(top_layout, 50);
    main_layout->setStretchFactor(bottom_layout, 850);
    this->setLayout(main_layout);

    connect(m_audio,SIGNAL(recordInput_updateAverageVolume(int)),this,SLOT(update_pcmWave(int)));
    connect(m_audio,SIGNAL(record_timeout(QByteArray,int)),this,SLOT(record_timeout(QByteArray,int)));
    connect(m_voiceRegistrationWin_showBtn,SIGNAL(clicked()),this,SLOT(voiceRegistrationWin_showBtn_clicked()));
    connect(m_voiceLibWin_showBtn,SIGNAL(clicked()),this,SLOT(voiceLibWin_showBtn_clicked()));
    connect(m_identifyBtn, SIGNAL(clicked()), this, SLOT(identifyBtn_clicked()));
}

VoiceIdentification_win::~VoiceIdentification_win()
{
    if(m_audio != nullptr) {
        delete m_audio;
        m_audio = nullptr;
    }
    delete ui;
}

void VoiceIdentification_win::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    //背景绘制
    painter.drawPixmap(0,0,width(),height(),QPixmap("://images/bg.png"));
}

void VoiceIdentification_win::identifyBtn_clicked()
{
    m_registrantHeadLab->hide();
    m_identifyInfoLab->setText("");
//    auto callback = [=](int val)->void{m_PCMWaveform_widget->UpdateForm(val);};
    m_audio->record(30000);
}

void VoiceIdentification_win::update_pcmWave(int volume)
{
    m_PCMWaveform_widget->UpdateForm(volume);
}

void VoiceIdentification_win::record_timeout(QByteArray buf,int len)
{
    RPC_Kvp_Tool *rpc_tool = RPC_Kvp_Tool::GetInstance();
    _Rpc_TopSpeakerInfo* ret = new _Rpc_TopSpeakerInfo();
    rpc_tool->KvpIdentifyTopSpeakerByStream(ret, (short*)buf.data(), len, &vp_node,1,1, 1, 0);
    m_error_code = ret->ErrCode;
    if(ret->ErrCode == 0){
        SoundsData_db *db = SoundsData_db::GetInstance();
        RegistrantInfo registrant_info;
        db->GetRegistrantInfoBySpkId(ret->Scores->Spkid, registrant_info);
        m_registrantHeadLab->show();
        m_registrantHeadLab->setPixmap(QPixmap(registrant_info.head_path));
        m_identifyInfoLab->setText(registrant_info.name);
    }else{
        m_identifyInfoLab->setText("识别失败");
    }
}

void VoiceIdentification_win::voiceRegistrationWin_showBtn_clicked()
{
    emit switchToVoiceRegistrationWin();
}

void VoiceIdentification_win::voiceLibWin_showBtn_clicked()
{
    emit switchToVoiceLibWin();
}