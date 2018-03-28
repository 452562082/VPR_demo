#include "voiceidentification_win.h"
#include "ui_voiceidentification_win.h"
#include "utils/audiotool.h"
#include "pcmwaveform_widget.h"
#include <QPainter>
#include <QCryptographicHash>
#include <QDir>
#include <QMenu>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "masklabel.h"
#include "soundsdata_db.h"
#include "utils/httpsender.h"
#include "utils/logger.h"
#include "rpc/rpcproxy.h"

namespace{
    const std::vector<std::string> node_list(1,"Test_RPC");
}

VoiceIdentification_win::VoiceIdentification_win(QWidget *parent) :
    QWidget(parent),
    m_xvad_handle(nullptr),
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
    m_PCMWaveform_widget->setFixedSize(600,300);
    QHBoxLayout *bottom_third_layout = new QHBoxLayout;
    bottom_third_layout->addStretch(1);
    bottom_third_layout->addWidget(m_PCMWaveform_widget);
    bottom_third_layout->addStretch(1);

    QVBoxLayout *bottom_layout = new QVBoxLayout;
    bottom_layout->addSpacing(40);
    bottom_layout->addLayout(bottom_first_layout);
    bottom_layout->addSpacing(5);
    bottom_layout->addLayout(bottom_second_layout);
    bottom_layout->addLayout(bottom_third_layout);
    bottom_layout->addStretch(1);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(top_layout);
    main_layout->addLayout(bottom_layout);
    main_layout->setStretchFactor(top_layout, 50);
    main_layout->setStretchFactor(bottom_layout, 850);
    this->setLayout(main_layout);
    m_rightMenu = new QMenu(this);
    QAction *exitAction = m_rightMenu->addAction("退出");

    connect(m_audio,SIGNAL(recordInput_updateAverageVolume(int)),this,SLOT(update_pcmWave(int)));
    connect(m_audio,SIGNAL(recordInput_cycleData(QByteArray)),this,SLOT(record_cycleDataReceived(QByteArray)));
    connect(m_voiceRegistrationWin_showBtn,SIGNAL(clicked()),this,SLOT(voiceRegistrationWin_showBtn_clicked()));
    connect(m_voiceLibWin_showBtn,SIGNAL(clicked()),this,SLOT(voiceLibWin_showBtn_clicked()));
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(exitAction_triggered()));
    switch_identificationStatus();
}

VoiceIdentification_win::~VoiceIdentification_win()
{
    if(m_audio != nullptr) {
        delete m_audio;
        m_audio = nullptr;
    }
    if(m_xvad_handle != nullptr) {
        free(m_xvad_handle);
    }
    delete ui;
}

void VoiceIdentification_win::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    //背景绘制
    painter.drawPixmap(0,0,width(),height(),QPixmap("://images/bg.png"));
}

void VoiceIdentification_win::contextMenuEvent(QContextMenuEvent* e)
{
    Q_UNUSED(e)
    m_rightMenu->exec(QCursor::pos());
}

void VoiceIdentification_win::switch_identificationStatus()
{
    m_audio->record();
    m_registrantHeadLab->hide();
    m_identifyInfoLab->setText("");
}

void VoiceIdentification_win::update_pcmWave(int volume)
{
    m_PCMWaveform_widget->UpdateForm(volume);
}

void VoiceIdentification_win::record_cycleDataReceived(QByteArray buf)
{
    if(m_xvad_handle == nullptr){
        m_xvad_handle = xvpr_vad_create_vad(8000,10);
    }

    if(xvpr_vad_is_speaking(m_xvad_handle,(short*)buf.constData(),buf.size()/2) && !m_stream_buf.isEmpty()){
        if(m_stream_buf.size() > 8000 * 2){
            identify(m_stream_buf);
        }
        m_stream_buf.clear();
    }else{
        m_stream_buf.append(buf);
    }
}

void VoiceIdentification_win::identify(QByteArray buf)
{   
    std::vector<int16_t> utt_arr;
    short* utt = (short*)buf.data();
    for(int i = 0;i < buf.size()/2;i++) {
        utt_arr.push_back(utt[i]);
    }
    _Rpc_TopSpeakerInfo *ret = RpcProxy::GetInstance()->KvpIdentifyTopSpeakerByStream(utt_arr, m_audio->getSampleRate(), node_list,1,1,0);
    if(ret != nullptr){
        m_error_code = ret->ErrCode;
    }

    if(ret != nullptr && ret->ErrCode == 0 && ret->Top != 0){
        SoundsData_db *db = SoundsData_db::GetInstance();
        RegistrantInfo registrant_info;
        //取最高得分
        int max_score_index = 0;
        for(int i = 0;i < ret->Scores.size();++i){
            if(ret->Scores[i].Score > ret->Scores[max_score_index].Score){
                max_score_index = i;
            }
        }
        Logger::Info(QString("RPC - KvpIdentifyTopSpeakerByStream Max score Spkid = %1").arg(ret->Scores[max_score_index].Spkid.c_str()));
        db->GetRegistrantInfoBySpkId(ret->Scores[max_score_index].Spkid.c_str(), registrant_info);

        QString head_dir_name = QDir::currentPath() + "/headImages/";
        QDir head_dir;
        if(!head_dir.exists(head_dir_name)){
            head_dir.mkdir(head_dir_name);
        }
        QString local_head_path = head_dir_name + registrant_info.spk_id + registrant_info.head_path.right(registrant_info.head_path.length() - registrant_info.head_path.lastIndexOf('.'));
        m_local_head_path = local_head_path;
        if(!QFile::exists(local_head_path)){
            HttpSender *http_sender = HttpSender::GetInstance();
            http_sender->downloadFile(registrant_info.head_path,local_head_path);
            connect(http_sender,SIGNAL(finished()),this,SLOT(updateRegistantHeadLab()));
        }else{
            m_registrantHeadLab->show();
            m_registrantHeadLab->setPixmap(QPixmap(local_head_path));
        }

        m_identifyInfoLab->setText(registrant_info.name);
        RpcProxy::GetInstance()->Delete_Asv_Type(_Asv_Type::_RPC_TopSpeakerInfo,ret);
    }else{
        m_registrantHeadLab->hide();
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

//void VoiceIdentification_win::updateCountDownLab()
//{
//    m_cur_countDownNum--;
//    if(m_cur_countDownNum == 0){
//        m_countDownLab->hide();
//        m_countDownTimer->stop();
//    }else{
//        m_countDownLab->setText(QString::number(m_cur_countDownNum,10));
//    }
//}

void VoiceIdentification_win::updateRegistantHeadLab()
{
    m_registrantHeadLab->show();
    m_registrantHeadLab->setPixmap(QPixmap(m_local_head_path));
}

void VoiceIdentification_win::exitAction_triggered()
{
    emit exit();
}
