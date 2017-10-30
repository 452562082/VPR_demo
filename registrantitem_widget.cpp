#include "registrantitem_widget.h"
#include "ui_registrantitem_widget.h"
#include "masklabel.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>

RegistrantItem_widget::RegistrantItem_widget(const RegistrantInfo &registrant_info,
                                             QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrantItem_widget)
{
    ui->setupUi(this);
    this->setFixedSize(537,94);
    QFont font;
    font.setFamily("Microsoft YaHei");
    m_registrantHeadLab = new MaskLabel(this);
    m_registrantHeadLab->setFixedSize(68,68);
    m_registrantHeadLab->setPixmap(QPixmap(registrant_info.local_head_path));
    QVBoxLayout *first_layout = new QVBoxLayout;
    first_layout->addStretch(1);
    first_layout->addWidget(m_registrantHeadLab);
    first_layout->addStretch(1);

    QLabel *registrantNameLab = new QLabel(this);
    registrantNameLab->setText(registrant_info.name);
    registrantNameLab->setFont(font);
    registrantNameLab->setStyleSheet("* {color: #FFFFFF; font-size: 17px;}");

    QLabel *registrantRegistrationTimeLab = new QLabel(this);
    registrantRegistrationTimeLab->setText(registrant_info.registration_time.toString("yyyy-MM-dd hh:mm:ss"));
    registrantRegistrationTimeLab->setFont(font);
    registrantRegistrationTimeLab->setStyleSheet("* {color: #FFFFFF; font-size: 14px;}");
    QVBoxLayout *second_layout = new QVBoxLayout;
    second_layout->addStretch(1);
    second_layout->addWidget(registrantNameLab);
    second_layout->addWidget(registrantRegistrationTimeLab);
    second_layout->addStretch(1);

    m_removeRegistrantBtn = new QPushButton(this);
    m_removeRegistrantBtn->setStyleSheet("* {background-color: transparent;border-image:url(:/images/remove.png); width:23px; height:23px;} * :hover{border-image:url(:/images/remove_hover.png);}");
    m_removeRegistrantBtn->setCursor(Qt::PointingHandCursor);
    QVBoxLayout *third_layout = new QVBoxLayout;
    third_layout->addStretch(1);
    third_layout->addWidget(m_removeRegistrantBtn);
    third_layout->addStretch(1);

    QHBoxLayout *main_layout = new QHBoxLayout;
    main_layout->addSpacing(25);
    main_layout->addLayout(first_layout);
    main_layout->addSpacing(15);
    main_layout->addLayout(second_layout);
    main_layout->addStretch(1);
    main_layout->addLayout(third_layout);
    main_layout->addSpacing(25);
    this->setLayout(main_layout);

    m_registrant_info = registrant_info;

    connect(m_removeRegistrantBtn,SIGNAL(clicked()),this,SLOT(removeRegistrantBtn_clicked()));
}

RegistrantItem_widget::~RegistrantItem_widget()
{
    delete ui;
}

void RegistrantItem_widget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    //背景绘制
    painter.drawPixmap(0,0,width(),height(),QPixmap("://images/item_bg.png"));
}

void RegistrantItem_widget::removeRegistrantBtn_clicked()
{
    emit removeRegistrant();
}

void RegistrantItem_widget::updateHeadImg()
{
    m_registrantHeadLab->setPixmap(QPixmap(m_registrant_info.local_head_path));
}
