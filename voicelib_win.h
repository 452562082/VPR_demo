#ifndef VoiceLib_win_H
#define VoiceLib_win_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>

namespace Ui {
class VoiceLib_win;
}

class VoiceLibTable_widget;
class VoiceLib_win : public QWidget
{
    Q_OBJECT

public:
    explicit VoiceLib_win(QWidget *parent = 0);
    ~VoiceLib_win();
    void bindData();

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::VoiceLib_win *ui;
    QPushButton *m_returnBtn;//返回按钮句柄
    VoiceLibTable_widget *m_voiceLibTable;//已注册用户列表

signals:
    void switchToVoiceIdentificationWin();

private slots:
    void returnBtn_clicked();
};

#endif // VoiceLib_win_H
