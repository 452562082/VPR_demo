#include "audiotool.h"
#include "utils/logger.h"

namespace{
    struct WAVFILEHEADER
    {
        // RIFF 头
        char RiffName[4];
        unsigned long nRiffLength;

        // 数据类型标识符
        char WavName[4];

        // 格式块中的块头
        char FmtName[4];
        unsigned long nFmtLength;

        // 格式块中的块数据
        unsigned short nAudioFormat;
        unsigned short nChannleNumber;
        unsigned long nSampleRate;
        unsigned long nBytesPerSecond;
        unsigned short nBytesPerSample;
        unsigned short nBitsPerSample;

        // 数据块中的块头
        char    DATANAME[4];
        unsigned long   nDataLength;
    };

    qint64 RawFile2WavFile(const QString& catheFileName ,const QString& wavFileName)
    {
        WAVFILEHEADER WavFileHeader;
        qstrcpy(WavFileHeader.RiffName, "RIFF");
        qstrcpy(WavFileHeader.WavName, "WAVE");
        qstrcpy(WavFileHeader.FmtName, "fmt ");
        qstrcpy(WavFileHeader.DATANAME, "data");

        // 表示 FMT块 的长度
        WavFileHeader.nFmtLength = 16;
        // 表示 按照PCM 编码;
        WavFileHeader.nAudioFormat = 1;
        // 声道数目;
        WavFileHeader.nChannleNumber = 1;
        // 采样频率;
        WavFileHeader.nSampleRate = 8000;

        // nBytesPerSample 和 nBytesPerSecond这两个值通过设置的参数计算得到;
        // 数据块对齐单位(每个采样需要的字节数 = 通道数 × 每次采样得到的样本数据位数 / 8 )
        WavFileHeader.nBytesPerSample = 2;
        // 波形数据传输速率
        // (每秒平均字节数 = 采样频率 × 通道数 × 每次采样得到的样本数据位数 / 8  = 采样频率 × 每个采样需要的字节数 )
        WavFileHeader.nBytesPerSecond = 16000;

        // 每次采样得到的样本数据位数;
        WavFileHeader.nBitsPerSample = 16;

        QFile cacheFile(catheFileName);
        QFile wavFile(wavFileName);

        if (!cacheFile.open(QIODevice::ReadWrite))
        {
            return -1;
        }
        if (!wavFile.open(QIODevice::WriteOnly))
        {
            return -2;
        }

        int nSize = sizeof(WavFileHeader);
        qint64 nFileLen = cacheFile.bytesAvailable();

        WavFileHeader.nRiffLength = nFileLen - 8 + nSize;
        WavFileHeader.nDataLength = nFileLen;

        // 先将wav文件头信息写入，再将音频数据写入;
        wavFile.write((char *)&WavFileHeader, nSize);
        wavFile.write(cacheFile.readAll());

        cacheFile.close();
        wavFile.close();

        return nFileLen;
    }
    const int BufferSize = 4096;
}

AudioTool::AudioTool(QObject *parant) : QObject(parant),
    m_audioStatus(AudioToolStatus::Freetime),
    m_inputDevSound(nullptr),
    m_audioInput(nullptr),
    m_audioOutput(nullptr),
    m_audioInput_timer(nullptr)
{
    // 设置音频文件格式;
    m_audioFormat.setSampleRate(8000);
    m_audioFormat.setChannelCount(1);
    m_audioFormat.setSampleSize(16);
    m_audioFormat.setCodec("audio/pcm");
    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    m_audioFormat.setSampleType(QAudioFormat::UnSignedInt);
}

AudioTool::~AudioTool()
{
    if(m_inputDevSound != nullptr) {
        delete m_inputDevSound;
        m_inputDevSound = nullptr;
    }
    if(m_audioInput != nullptr){
        delete m_audioInput;
        m_audioInput = nullptr;
    }
    if(m_audioOutput != nullptr){
        delete m_audioOutput;
        m_audioOutput = nullptr;
    }
    if(m_audioInput_timer != nullptr){
        delete m_audioInput_timer;
        m_audioInput_timer = nullptr;
    }
}

QStringList AudioTool::GetSupportedCodecs()
{
    QAudioDeviceInfo AudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());
    return AudioDeviceInfo.supportedCodecs();
}

void AudioTool::record(int timer_msec)
{
    switch(m_audioStatus){
    case AudioToolStatus::Freetime:
        startRecord(timer_msec);
        break;
    case AudioToolStatus::OnRecording:
        stopRecord();
        break;
    default:
        Logger::Warning("AUDIO - audio is playing");
        break;
    }
}

bool AudioTool::startRecord(int timer_msec)
{
    if(m_audioStatus != AudioToolStatus::Freetime) {
        Logger::Error("AUDIO - audio is not free");
        return false;
    }

    if(!initAudioFormat()){
        return false;
    }

    m_audioStatus = AudioToolStatus::OnRecording;

    m_buffer_input.clear();
    m_audioInput = new QAudioInput(m_audioFormat, this);
    m_audioInput->setVolume(1.0);
    if(timer_msec > 0){
        if(m_audioInput_timer != nullptr){
            delete m_audioInput_timer;
        }
        m_audioInput_timer = new QTimer;
        connect(m_audioInput_timer,SIGNAL(timeout()), this, SLOT(input_timeout_slot()));
        m_audioInput_timer->start(timer_msec);
    }
    m_inputDevSound = m_audioInput->start();
    connect(m_inputDevSound,SIGNAL(readyRead()),this,SLOT(OnAudioInputRead()));

    return true;
}

void AudioTool::input_timeout_slot()
{
    stopRecord();

    int len = m_buffer_input.length();
    Q_ASSERT(m_audioFormat.sampleSize() % 8 == 0);
    const int channelBytes = m_audioFormat.sampleSize() / 8;
    const int sampleBytes = m_audioFormat.channelCount() * channelBytes;
    Q_ASSERT(len % sampleBytes == 0);
    const int numSamples = len / sampleBytes;
    emit record_timeout(m_buffer_input, numSamples);
}

void AudioTool::OnAudioInputRead()
{
    if(!m_audioInput){
        return;
    }

    qint64 len = m_audioInput->bytesReady();
    if(len > BufferSize){
        len = BufferSize;
    }

    QByteArray _Buffer(BufferSize, 0);
    qint64 l = m_inputDevSound->read(_Buffer.data(), len);

    if(l > 0) {
        m_buffer_input.append(_Buffer.constData(), l);
        short* resultingData = (short*)_Buffer.data();
        int total_value = 0;
        for(int i = 0;i < len;++i){
            total_value += resultingData[i];
        }
        int average_value = total_value / len;
        emit recordInput_updateAverageVolume(average_value);
    }
}

void AudioTool::stopRecord()
{
    if(m_audioStatus == AudioToolStatus::OnRecording)
    {
        m_audioStatus = AudioToolStatus::Freetime;
        if(m_audioInput != nullptr){
            m_audioInput->stop();
            delete m_audioInput;
            m_audioInput = nullptr;
        }
        if(m_inputDevSound != nullptr){
            m_inputDevSound->close();
            delete m_inputDevSound;
            m_inputDevSound = nullptr;
        }
        if(m_audioInput_timer != nullptr){
            m_audioInput_timer->stop();
            delete m_audioInput_timer;
            m_audioInput_timer = nullptr;
        }
        /*QString rawFilename = m_operateFile.fileName();
        QString wavFilename = QString(rawFilename).replace(rawFilename.indexOf('.'),rawFilename.length() - rawFilename.indexOf('.'),".wav");
        RawFile2WavFile(rawFilename, wavFilename);
        QFile::remove(rawFilename);*/
    }
}

bool AudioTool::startPlay()
{
    if(m_audioStatus != AudioToolStatus::Freetime) {
        Logger::Error("AUDIO - audio is not free");
        return false;
    }

    m_audioStatus = AudioToolStatus::OnPlay;

    if(!initAudioFormat()){
        return false;
    }

    m_audioOutput = new QAudioOutput(m_audioFormat, this);
    m_outputDevSound = m_audioOutput->start();
    connect(m_outputDevSound,SIGNAL(readyRead()),this,SLOT(OnAudioOutputRead()));
    return true;
}

void AudioTool::OnAudioOutputRead()
{
//    if(!m_audioOutput){
//        return;
//    }

//    qint64 len = m_audioOutput->bytesReady();
//    QByteArray _Buffer(4096, 0);
//    qint64 l = m_outputDevSound->read(_Buffer.data(), len);
//    if(l > 0) {
//        short* resultingData = (short*)_Buffer.data();
//        int total_value = 0;
//        for(int i = 0;i < len;++i){
//            total_value += resultingData[i];
//        }
//        int average_value = total_value / len;
//        m_record_callback(average_value);
//    }
}

void AudioTool::stopPlay()
{
    if(m_audioStatus == AudioToolStatus::OnPlay){
        m_audioStatus = AudioToolStatus::Freetime;
        if(m_audioOutput != nullptr){
            m_audioOutput->stop();
            delete m_audioOutput;
            m_audioOutput = nullptr;
        }
        if(m_outputDevSound != nullptr){
            m_outputDevSound->close();
            delete m_outputDevSound;
            m_outputDevSound = nullptr;
        }
    }
}

void AudioTool::setSampleRate(int sampleRate)
{
    m_audioFormat.setSampleRate(sampleRate);
}

void AudioTool::setChannelCount(int channelCount)
{
    m_audioFormat.setChannelCount(channelCount);
}

void AudioTool::setSampleSize(int sampleSize)
{
    m_audioFormat.setSampleSize(sampleSize);
}

void AudioTool::setCodec(const QString& codec)
{
    m_audioFormat.setCodec(codec);
}

void AudioTool::setByteOrder(QAudioFormat::Endian byteOrder)
{
    m_audioFormat.setByteOrder(byteOrder);
}

void AudioTool::setSampleType(QAudioFormat::SampleType sampleType)
{
    m_audioFormat.setSampleType(sampleType);
}

bool AudioTool::initAudioFormat()
{
    QAudioDeviceInfo audioDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
    if(audioDeviceInfo.isNull()){
        Logger::Error("AUDIO - No Record Device");
        return false;
    }

    if (!audioDeviceInfo.isFormatSupported(m_audioFormat))
    {
        Logger::Error("AUDIO - Default format not supported");
        return false;
    }
    return true;
}
