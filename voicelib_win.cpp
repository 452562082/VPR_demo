#include "voicelib_win.h"
#include "ui_VoiceLib_win.h"
#include "voicelibtable_widget.h"
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include "registrantitem_widget.h"

VoiceLib_win::VoiceLib_win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VoiceLib_win)
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

    //创建已注册用户显示表格控件
    m_voiceLibTable = new VoiceLibTable_widget(this);
    QHBoxLayout *bottom_middle_layout = new QHBoxLayout;
    bottom_middle_layout->addStretch(1);
    bottom_middle_layout->addWidget(m_voiceLibTable);
    bottom_middle_layout->addStretch(1);
    QVBoxLayout *bottom_layout = new QVBoxLayout;
    bottom_layout->addSpacing(98);
    bottom_layout->addLayout(bottom_middle_layout);
    bottom_layout->addStretch(1);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(top_layout);
    main_layout->addLayout(bottom_layout);
    main_layout->setStretchFactor(top_layout, 50);
    main_layout->setStretchFactor(bottom_layout, 850);
    this->setLayout(main_layout);

    connect(m_returnBtn,SIGNAL(clicked()),this,SLOT(returnBtn_clicked()));
}

VoiceLib_win::~VoiceLib_win()
{
    delete ui;
}

void VoiceLib_win::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    //背景绘制
    painter.drawPixmap(0,0,width(),height(),QPixmap("://images/bg.png"));
}

void VoiceLib_win::bindData()
{
    m_voiceLibTable->bindData();
}

void VoiceLib_win::returnBtn_clicked()
{
    emit switchToVoiceIdentificationWin();
}
