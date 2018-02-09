#ifndef VoiceIdentification_win_H
#define VoiceIdentification_win_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "soundsdata_db.h"
#include "utils/xbusiness_vpr.h"
#include <QFile>

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
    void switch_identificationStatus();

protected:
    void paintEvent(QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent* e);

private:
    Ui::VoiceIdentification_win *ui;
    QPushButton *m_voiceRegistrationWin_showBtn;//声纹注册界面显示句柄
    QPushButton *m_voiceLibWin_showBtn;//声纹库界面显示句柄
    MaskLabel *m_registrantHeadLab;//声纹注册用户头像句柄
    QLabel *m_identifyInfoLab;//声纹识别信息显示句柄
    PCMWaveform_widget* m_PCMWaveform_widget;//波形图显示控件句柄
//    QPushButton *m_identifyBtn;//声纹识别句柄
//    QLabel *m_countDownLab;//倒计时显示控件
//    int m_cur_countDownNum;
//    QTimer *m_countDownTimer;//倒计时计时器
    AudioTool *m_audio;//音频工具
    int m_error_code;
    QString m_local_head_path;
    XVAD *m_xvad_handle;
    QByteArray m_stream_buf;//录音缓冲数据
    QFile m_file;
    QMenu *m_rightMenu;

    void identify(QByteArray buf);

signals:
    void switchToVoiceLibWin();
    void switchToVoiceRegistrationWin();
    void exit();

private slots:
    void update_pcmWave(int);
//    void record_timeout(QByteArray,int);
//    void record_periodOver(QByteArray);
    void record_cycleDataReceived(QByteArray);
    void voiceRegistrationWin_showBtn_clicked();
    void voiceLibWin_showBtn_clicked();
//    void identifyBtn_clicked();
//    void updateCountDownLab();
    void updateRegistantHeadLab();
    void exitAction_triggered();
};

#endif // VoiceIdentification_win_H
