#include "voiceregistration_win.h"
#include "ui_voiceregistration_win.h"
#include "pcmwaveform_widget.h"
#include "utils/rpc_kvp_tool.h"
#include "utils/audiotool.h"
#include "soundsdata_db.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>
#include "masklabel.h"
#include <memory>

namespace{
    const char* vp_node = "Test_RPC";
}

VoiceRegistration_win::VoiceRegistration_win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VoiceRegistration_win),
    m_buf_name(""),
    m_buf_head_path("")
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
    //创建返回按钮句柄
    m_returnBtn = new QPushButton(this);
    m_returnBtn->setStyleSheet("* {background-color: transparent;border-image:url(:/images/return.png); width:55px; height:16px;}* :hover{border-image:url(:/images/return_hover.png);}");
    QHBoxLayout *top_middle_layout = new QHBoxLayout;
    top_middle_layout->addSpacing(34);
    top_middle_layout->addWidget(logoLab);
    top_middle_layout->addWidget(titleLab);
    top_middle_layout->addStretch(1);
    top_middle_layout->addWidget(m_returnBtn);
    top_middle_layout->addSpacing(34);
    QVBoxLayout *top_layout = new QVBoxLayout;
    top_layout->addStretch(1);
    top_layout->addLayout(top_middle_layout);
    top_layout->addStretch(1);

    ///声纹注册栏
    //创建音频工具
    m_audio = new AudioTool;
    //创建头像选择按钮控件
    m_registrantHeadLab = new MaskLabel(this);
    m_registrantHeadLab->setPixmap(QPixmap("://images/default_head.png"));
    m_registrantHeadLab->setFixedSize(80,80);
    m_registrantHeadLab->setCursor(Qt::PointingHandCursor);
    QHBoxLayout *bottom_first_layout = new QHBoxLayout;
    bottom_first_layout->addStretch(1);
    bottom_first_layout->addWidget(m_registrantHeadLab);
    bottom_first_layout->addStretch(1);
    //创建注册用户名控件
    m_registrantNameEdit = new QLineEdit(this);
    m_registrantNameEdit->setFont(font);
    m_registrantNameEdit->setStyleSheet("color: #4e9cf1;font-size:18px;background:transparent;border-width:0;border-style:outset;width: 95px;height: 17px;");
    m_registrantNameEdit->setPlaceholderText("请输入姓名");
    QHBoxLayout *bottom_second_layout = new QHBoxLayout;
    bottom_second_layout->addStretch(1);
    bottom_second_layout->addWidget(m_registrantNameEdit);
    bottom_second_layout->addStretch(1);
    //创建声纹波形显示控件
    m_PCMWaveform_widget = new PCMWaveform_widget;
    m_PCMWaveform_widget->setFixedSize(456,157);
    QHBoxLayout *bottom_third_layout = new QHBoxLayout;
    bottom_third_layout->addStretch(1);
    bottom_third_layout->addWidget(m_PCMWaveform_widget);
    bottom_third_layout->addStretch(1);
    //创建声纹注册按钮
    m_registerBtn = new QPushButton(this);
    m_registerBtn->setStyleSheet(QString("* {background-color: transparent;"
                                 "border-radius: %1px;"
                                 "border-image:url(:/images/record.png);"
                                 "width: 184px;"
                                 "height:184px;}"
                                 "* :hover{border-image:url(:/images/record_hover.png);}").arg(m_registerBtn->width()/2));
    QHBoxLayout *bottom_fourth_layout = new QHBoxLayout;
    bottom_fourth_layout->addStretch(1);
    bottom_fourth_layout->addWidget(m_registerBtn);
    bottom_fourth_layout->addStretch(1);
    QLabel *registerLab = new QLabel("开始注册");
    registerLab->setFont(font);
    registerLab->setStyleSheet("* {color: #2b85e3; font-size: 20px;}");
    QHBoxLayout *bottom_fifth_layout = new QHBoxLayout;
    bottom_fifth_layout->addStretch(1);
    bottom_fifth_layout->addWidget(registerLab);
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
    connect(m_returnBtn,SIGNAL(clicked()),this,SLOT(returnBtn_clicked()));
    connect(m_registrantHeadLab,SIGNAL(clicked()),this,SLOT(registrantHeadLab_clicked()));
    connect(m_registerBtn,SIGNAL(clicked()),this,SLOT(registerBtn_clicked()));
}

VoiceRegistration_win::~VoiceRegistration_win()
{
    if(m_audio != nullptr) {
        delete m_audio;
        m_audio = nullptr;
    }
    delete ui;
}

void VoiceRegistration_win::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    //背景绘制
    painter.drawPixmap(0,0,width(),height(),QPixmap("://images/bg.png"));
    QPen pen;
    pen.setColor(QColor("#4a9cf1"));
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(m_registrantNameEdit->pos().x() - 30,m_registrantNameEdit->pos().y() + m_registrantNameEdit->height() + 4,m_registrantNameEdit->pos().x() + m_registrantNameEdit->width() + 30,m_registrantNameEdit->pos().y() + m_registrantNameEdit->height() + 4);
}

void VoiceRegistration_win::registrantHeadLab_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("打开图片"), ".", tr("图片类型(*.jpg *.png)"));
    if(filePath.isEmpty()){
        return;
    }
    m_registrantHeadLab->setPixmap(QPixmap(filePath));
    m_buf_head_path = filePath;
}

void VoiceRegistration_win::registerBtn_clicked()
{
    RPC_Kvp_Tool *rpc_tool = RPC_Kvp_Tool::GetInstance();
    int is_insert_failed = true;
    rpc_tool->KvpInsertNode(is_insert_failed,"Test_RPC");
    if(m_registrantNameEdit->text().isEmpty()){
        QMessageBox::warning(this,"错误","请输入您的名字!",QMessageBox::Yes);
        return;
    }
    m_buf_name = m_registrantNameEdit->text();
    m_audio->record(20000);
}

void VoiceRegistration_win::update_pcmWave(int volume)
{
    m_PCMWaveform_widget->UpdateForm(volume);
}

void VoiceRegistration_win::record_timeout(QByteArray buf,int len)
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(buf);
    QString spk_id = md5.result().toHex();
    RPC_Kvp_Tool *rpc_tool = RPC_Kvp_Tool::GetInstance();
    _Rpc_ModelInfo *ret = nullptr;
    rpc_tool->KvpRegisterSpeakerByStream(ret, (short*)buf.data(), len , vp_node, "/tmp/asv/", spk_id.toStdString().c_str());
    if(ret == nullptr){
        return;
    }
    if(ret->ErrCode == 0){
        SoundsData_db *db = SoundsData_db::GetInstance();
        db->AddRegistrantInfo(spk_id,m_buf_name,m_buf_head_path);
        QMessageBox::information(this,tr("信息"),QString("注册成功"),QMessageBox::Yes);
    }else{
        QMessageBox::warning(this,tr("错误"),QString("注册失败,错误码为%1,错误信息:%2").arg(QString::number(ret->ErrCode, 10)).arg(ret->ErrMsg),QMessageBox::Yes);
    }
    rpc_tool->Delete_Rpc_ModelInfo(ret);
}

void VoiceRegistration_win::returnBtn_clicked()
{
    emit switchToVoiceIdentificationWin();
}
