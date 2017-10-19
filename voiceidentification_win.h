#ifndef VoiceIdentification_win_H
#define VoiceIdentification_win_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class VoiceIdentification_win;
}
class PCMWaveform_widget;
class AudioTool;
class MaskLabel;
class VoiceIdentification_win : public QWidget
{
    Q_OBJECT

public:
    explicit VoiceIdentification_win(QWidget *parent = 0);
    ~VoiceIdentification_win();

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::VoiceIdentification_win *ui;
    QPushButton *m_voiceRegistrationWin_showBtn;//声纹注册界面显示句柄
    QPushButton *m_voiceLibWin_showBtn;//声纹库界面显示句柄
    MaskLabel *m_registrantHeadLab;//声纹注册用户头像句柄
    QLabel *m_identifyInfoLab;//声纹识别信息显示句柄
    PCMWaveform_widget* m_PCMWaveform_widget;//波形图显示控件句柄
    QPushButton *m_identifyBtn;//声纹识别句柄
    AudioTool *m_audio;//音频工具
    int m_error_code;

signals:
    void switchToVoiceLibWin();
    void switchToVoiceRegistrationWin();

private slots:
    void update_pcmWave(int);
    void record_timeout(QByteArray,int);
    void voiceRegistrationWin_showBtn_clicked();
    void voiceLibWin_showBtn_clicked();
    void identifyBtn_clicked();
};

#endif // VoiceIdentification_win_H
