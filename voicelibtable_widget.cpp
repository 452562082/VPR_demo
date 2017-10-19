#include "voicelibtable_widget.h"
#include "ui_VoiceLibTable_widget.h"
#include "soundsdata_db.h"
#include <QLabel>
#include <QDebug>
#include <QGridLayout>
#include "registrantitem_widget.h"
#include "utils/rpc_kvp_tool.h"

VoiceLibTable_widget::VoiceLibTable_widget(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::VoiceLibTable_widget)
{
    ui->setupUi(this);
    this->setFixedSize(1120,634);
    this->setStyleSheet("* {background-color: transparent;}");
    m_current_row = 0;
    m_current_column = 0;
    m_centerWidget = new QWidget(this);
    m_centerWidget->setFixedWidth(1100);
    m_centerWidget_main_layout = new QGridLayout;
    m_centerWidget_main_layout->setRowStretch(0,1);
    m_centerWidget_main_layout->setColumnStretch(1,1);
    m_centerWidget->setLayout(m_centerWidget_main_layout);
    this->setWidget(m_centerWidget);
}

VoiceLibTable_widget::~VoiceLibTable_widget()
{
    for(QList<RegistrantInfo*>::iterator it = m_registrantInfoList.begin();it != m_registrantInfoList.end();++it){
        if(*it != nullptr){
            delete *it;
            *it = nullptr;
        }
    }
    m_registrantInfoList.clear();
    delete ui;
}

void VoiceLibTable_widget::clear()
{
    if(!this->widget()->layout()){
        return;
    }

    clearLayout(this->widget()->layout());
    m_current_column = 0;
    m_current_row = 0;
}

void VoiceLibTable_widget::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0)) != 0){
        //删除widget
        if(item->widget()){
            delete item->widget();
            //item->widget()->deleteLater();
        }
        //删除子布局
        QLayout *childLayout = item->layout();
        if(childLayout){
            clearLayout(childLayout);
        }
        delete item;
    }
}

void VoiceLibTable_widget::addWidget(QWidget *widget)
{
    if(m_current_column % 2 == 0){
        m_centerWidget_main_layout->addWidget(widget,m_current_row,m_current_column);
        m_current_column = 1;
        m_centerWidget->setFixedHeight((m_current_row + 1) * 104);
    }else{
        m_centerWidget_main_layout->addWidget(widget,m_current_row++,m_current_column);
        m_current_column = 0;
    }
}

void VoiceLibTable_widget::removeItem(RegistrantInfo *info)
{
    m_registrantInfoList.removeAll(info);
    clear();
    for(int i = 0;i < m_registrantInfoList.count();++i){
        RegistrantItem_widget *item = new RegistrantItem_widget(m_registrantInfoList.at(i),this);
        connect(item,SIGNAL(removeRegistrant(RegistrantInfo*)),this,SLOT(removeRegistrantItem(RegistrantInfo*)));
        addWidget(item);
    }
    this->widget()->update();
}

void VoiceLibTable_widget::bindData()
{
    this->clear();
    for(QList<RegistrantInfo*>::iterator it = m_registrantInfoList.begin();it != m_registrantInfoList.end();++it){
        if(*it != nullptr){
            delete *it;
            *it = nullptr;
        }
    }
    m_registrantInfoList.clear();
    SoundsData_db *db = SoundsData_db::GetInstance();
    QVector<RegistrantInfo> infos;
    if(!db->GetAllRegistrantsInfo(infos)){
        return;
    }

    for(int i = 0;i < infos.size();++i){
        RegistrantInfo *info = new RegistrantInfo(infos[i]);
        RegistrantItem_widget *item = new RegistrantItem_widget(info,this);
        connect(item,SIGNAL(removeRegistrant(RegistrantInfo*)),this,SLOT(removeRegistrantItem(RegistrantInfo*)));
        addWidget(item);
        m_registrantInfoList.append(info);
    }

    this->widget()->update();
}

void VoiceLibTable_widget::removeRegistrantItem(RegistrantInfo* info)
{
    RPC_Kvp_Tool *rpc_kvp = RPC_Kvp_Tool::GetInstance();
    int ret = -1;
    if(!rpc_kvp->KvpModelRemoveBySpkid(ret,"Test_RPC","/tmp/asv/",info->spk_id.toStdString().c_str()) || ret != 0){
        qDebug() << "Error: RPC remove Spkid failed";
        return;
    }
    SoundsData_db *db = SoundsData_db::GetInstance();
    if(!db->DeleteRegistrantInfoBySpkId(info->spk_id)){
        qDebug() << "Error: SQL remove Spkid failed";
        return;
    }
    removeItem(info);
}
