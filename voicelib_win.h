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
    void contextMenuEvent(QContextMenuEvent* e);

private:
    Ui::VoiceLib_win *ui;
    QPushButton *m_returnBtn;//返回按钮句柄
    VoiceLibTable_widget *m_voiceLibTable;//已注册用户列表
    QMenu *m_rightMenu;

signals:
    void switchToVoiceIdentificationWin();
    void exit();

private slots:
    void returnBtn_clicked();
    void exitAction_triggered();
};

#endif // VoiceLib_win_H
