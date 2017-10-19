#ifndef VoiceRegistration_win_H
#define VoiceRegistration_win_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

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

private:
    Ui::VoiceRegistration_win *ui;
    MaskLabel *m_registrantHeadLab;//注册用户头像选择控件
    QLineEdit *m_registrantNameEdit;//注册用户名控件
    PCMWaveform_widget *m_PCMWaveform_widget;//声纹波形显示控件
    QPushButton *m_returnBtn;//返回按钮句柄
    QPushButton *m_registerBtn;//声纹注册按钮
    AudioTool *m_audio;//音频工具
private:
    QString m_buf_name;
    QString m_buf_head_path;

signals:
    void switchToVoiceIdentificationWin();

private slots:
    void update_pcmWave(int);
    void record_timeout(QByteArray,int);
    void returnBtn_clicked();
    void registrantHeadLab_clicked();
    void registerBtn_clicked();
};

#endif // VoiceRegistration_win_H
