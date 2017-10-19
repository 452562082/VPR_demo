#ifndef REGISTEREDUSERS_WIDGET_H
#define REGISTEREDUSERS_WIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QList>

namespace Ui {
class VoiceLibTable_widget;
}

class QGridLayout;
class RegistrantItem_widget;
class RegistrantInfo;
class VoiceLibTable_widget : public QScrollArea
{
    Q_OBJECT

public:
    explicit VoiceLibTable_widget(QWidget *parent = 0);
    ~VoiceLibTable_widget();
    void bindData();
    void clear();
    void addWidget(QWidget *widget);
    void removeItem(RegistrantInfo *info);

private:
    void clearLayout(QLayout *layout);

private:
    Ui::VoiceLibTable_widget *ui;
    int m_current_row;
    int m_current_column;
    QWidget *m_centerWidget;
    QGridLayout *m_centerWidget_main_layout;
    QList<RegistrantInfo*> m_registrantInfoList;

private slots:
    void removeRegistrantItem(RegistrantInfo* widget);
};

#endif // REGISTEREDUSERS_WIDGET_H
