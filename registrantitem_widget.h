#ifndef REGISTRANTITEM_WIDGET_H
#define REGISTRANTITEM_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "soundsdata_db.h"

namespace Ui {
class RegistrantItem_widget;
}

class MaskLabel;
class RegistrantItem_widget : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrantItem_widget(const RegistrantInfo &registrant_info,QWidget *parent = 0);
    ~RegistrantItem_widget();

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::RegistrantItem_widget *ui;
    QPushButton *m_removeRegistrantBtn;//删除注册用户按钮
    RegistrantInfo m_registrant_info;
    MaskLabel *m_registrantHeadLab;

signals:
    void removeRegistrant();

private slots:
    void removeRegistrantBtn_clicked();
    void updateHeadImg();
};

#endif // REGISTRANTITEM_WIDGET_H
