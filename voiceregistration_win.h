#ifndef VoiceRegistration_win_H
#define VoiceRegistration_win_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#pragma execution_character_set("utf-8")
namespace Ui {
class VoiceRegistration_win;
}

class PCMWaveform_widget;
class AudioTool;
class MaskLabel;
class VoiceRegistration_win : public QWidget
{
    Q_OBJECT

public:
    explicit VoiceRegistration_win(QWidget *parent = 0);
    ~VoiceRegistration_win();

protected:
    void paintEvent(QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent* e);

private:
    Ui::VoiceRegistration_win *ui;
    MaskLabel *m_registrantHeadLab;//注册用户头像选择控件
    QLineEdit *m_registrantNameEdit;//注册用户名控件
    PCMWaveform_widget *m_PCMWaveform_widget;//声纹波形显示控件
    QPushButton *m_returnBtn;//返回按钮句柄
    QPushButton *m_registerBtn;//声纹注册按钮
    QLabel *m_countDownLab;//倒计时显示控件
    int m_cur_countDownNum;
    QTimer *m_countDownTimer;//倒计时计时器
    AudioTool *m_audio;//音频工具
    QMenu *m_rightMenu;
private:
    QString m_buf_name;
    QString m_buf_head_path;

signals:
    void switchToVoiceIdentificationWin();
    void exit();

private slots:
    void update_pcmWave(int);
    void record_timeout(QByteArray);
    void returnBtn_clicked();
    void registrantHeadLab_clicked();
    void registerBtn_clicked();
    void updateCountDownLab();
    void exitAction_triggered();
};

#endif // VoiceRegistration_win_H
