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

//    enum class AudioToolStatus {
//        Freetime,
//        OnRecording,
//        OnPlay
//    };
public:
    AudioTool(QObject *parant = 0);
    ~AudioTool();

    /// @brief 获取当前设备支持音频编码格式
    static QStringList GetSupportedCodecs();
    /// @brief 录音
    bool record();
    /// @brief 定时录音
    /// @param timer_msec 限制定时录音
    bool timing_record(int timing_sec);
    /// @brief 播放
    bool play();

    void setSampleRate(int sampleRate);
    void setChannelCount(int channelCount);
    void setSampleSize(int sampleSize);
    void setCodec(const QString& codec);
    void setByteOrder(QAudioFormat::Endian byteOrder);
    void setSampleType(QAudioFormat::SampleType sampleType);
private:
    bool initAudioFormat();

private:
//    AudioToolStatus m_audioStatus;//当前状态
    QAudioFormat m_audioFormat;
    QIODevice* m_inputDevSound;//输入设备数据流
    QIODevice* m_outputDevSound;//输出设备数据流
    QAudioInput* m_audioInput;
    QAudioOutput* m_audioOutput;
//    QTimer *m_audioInput_timer;
    QByteArray m_buffer_input;//缓冲录音数据
    QFile m_file;
    qint64 m_dataLengthRecord;
//    QByteArray m_stream_buf;
//    int m_record_cycle_sec;

signals:
    void recordInput_updateAverageVolume(int);
    void recordInput_cycleData(QByteArray buf);
//    void recordInput_recordCycleEnd(QByteArray buf);
    void record_timeout(QByteArray buf);

private slots:
    void OnAudioInputRead();
    void OnAudioOutputRead();
    void input_timeout_slot();
};

#endif // AUDIOTOOL_H
