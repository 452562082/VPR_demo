#include "voicelibtable_widget.h"
#include "ui_VoiceLibTable_widget.h"
#include "soundsdata_db.h"
#include <QLabel>
#include <QGridLayout>
#include <QDir>
#include "registrantitem_widget.h"
#include "utils/httpsender.h"
#include "utils/logger.h"
#include "rpc/rpcproxy.h"

VoiceLibTable_widget::VoiceLibTable_widget(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::VoiceLibTable_widget)
{
    ui->setupUi(this);
    this->setFixedSize(1120,634);
    this->setStyleSheet("* {background-color: transparent;border:0px;}");
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

void VoiceLibTable_widget::removeItem(RegistrantItem_widget *item)
{
    m_registrantItemMap.remove(item);
    QList<RegistrantInfo> registrantInfoList = m_registrantItemMap.values();
    m_registrantItemMap.clear();
    clear();
    for(int i = 0;i < registrantInfoList.size();++i){
        RegistrantItem_widget *item = new RegistrantItem_widget(registrantInfoList[i],this);
        connect(item,SIGNAL(removeRegistrant()),this,SLOT(removeRegistrantItem()));
        addWidget(item);
        m_registrantItemMap.insert(item,registrantInfoList[i]);
    }
    this->widget()->update();
}

void VoiceLibTable_widget::bindData()
{
    this->clear();
    m_registrantItemMap.clear();
    SoundsData_db *db = SoundsData_db::GetInstance();
    QVector<RegistrantInfo> infos;
    if(!db->GetAllRegistrantsInfo(infos)){
        return;
    }    

    QString head_dir_name = QDir::currentPath() + "/headImages/";
    QDir head_dir;
    if(!head_dir.exists(head_dir_name)){
        head_dir.mkdir(head_dir_name);
    }

    HttpSender *http_sender = HttpSender::GetInstance();
    for(int i = 0;i < infos.size();++i){
        QString local_head_path = head_dir_name + infos[i].spk_id + infos[i].head_path.right(infos[i].head_path.length() - infos[i].head_path.lastIndexOf('.'));
        infos[i].local_head_path = local_head_path;
        RegistrantItem_widget *item = new RegistrantItem_widget(infos[i],this);
        if(!QFile::exists(local_head_path)){
            http_sender->downloadFile(infos[i].head_path,local_head_path);
            connect(http_sender,SIGNAL(finished()),item,SLOT(updateHeadImg()));
        }

        connect(item,SIGNAL(removeRegistrant()),this,SLOT(removeRegistrantItem()));
        addWidget(item);
        m_registrantItemMap.insert(item,infos[i]);
    }

    this->widget()->update();
}

void VoiceLibTable_widget::removeRegistrantItem()
{
    RegistrantItem_widget *item = qobject_cast<RegistrantItem_widget*>(sender());
    RegistrantInfo info(m_registrantItemMap.value(item));
    if(RpcProxy::GetInstance()->KvpModelRemoveBySpkid("Test_RPC",info.spk_id.toStdString())){
        Logger::Error(QString("RPC - remove Spkid %1 failed.").arg(info.spk_id));
        return;
    }
    Logger::Info(QString("RPC - remove Spkid %1 success.").arg(info.spk_id));
    SoundsData_db *db = SoundsData_db::GetInstance();
    if(!db->DeleteRegistrantInfoBySpkId(info.spk_id)){
        Logger::Error(QString("SQL - remove Spkid %1 failed.").arg(info.spk_id));
        return;
    }
    Logger::Info(QString("SQL - remove Spkid %1 success.").arg(info.spk_id));
    HttpSender *http_sender = HttpSender::GetInstance();
    http_sender->removeFile(info.head_path);
    if(QFile::exists(info.local_head_path)){
        QFile::remove(info.local_head_path);
    }
    removeItem(item);
}
