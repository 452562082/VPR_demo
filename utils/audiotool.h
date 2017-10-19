#ifndef AUDIOTOOL_H
#define AUDIOTOOL_H

#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioBuffer>
#include <functional>
#include <QTimer>
#include <QFile>

class AudioTool:public QObject
{
    Q_OBJECT

    enum class AudioToolStatus {
        Freetime,
        OnRecording,
        OnPlay
    };
public:
    AudioTool(QObject *parant = 0);
    ~AudioTool();

    /// @brief 获取当前设备支持音频编码格式
    static QStringList GetSupportedCodecs();
    /// @brief 录音
    /// @param timer_msec 限制定时录音
    void record(int timer_msec = -1);

    void setSampleRate(int sampleRate);
    void setChannelCount(int channelCount);
    void setSampleSize(int sampleSize);
    void setCodec(const QString& codec);
    void setByteOrder(QAudioFormat::Endian byteOrder);
    void setSampleType(QAudioFormat::SampleType sampleType);
private:
    bool initAudioFormat();
    /// @brief 开始录音
    bool startRecord(int timer_msec);
    /// @brief 停止录音
    void stopRecord();
    /// @brief 开始播放
    bool startPlay();
    /// @brief 停止播放
    void stopPlay();

private:
//    static AudioTool* m_instance;
    AudioToolStatus m_audioStatus;//当前状态
    QAudioFormat m_audioFormat;
    QIODevice* m_inputDevSound;//输入设备数据流
    QIODevice* m_outputDevSound;//输出设备数据流
    QAudioInput* m_audioInput;
    QAudioOutput* m_audioOutput;
    QTimer *m_audioInput_timer;
    QByteArray m_buffer_input;//缓冲录音数据

signals:
    void recordInput_updateAverageVolume(int);
    void record_timeout(QByteArray buf, int len);

private slots:
    void OnAudioInputRead();
    void OnAudioOutputRead();
    void input_timeout_slot();
};

#endif // AUDIOTOOL_H
