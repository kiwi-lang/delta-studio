#include "../include/yds_audio_file.h"

#include "../include/yds_audio_buffer.h"

ysAudioFile::ysAudioFile() {
    m_format = AudioFormat::Undefined;

    m_sampleCount = 0;
    m_externalBuffer = 0;

    m_maxBufferSamples = 0;
    m_bufferDataSamples = 0;
    m_currentReadingOffset = 0;

    m_fileOpen = false;
}

ysAudioFile::ysAudioFile(Platform platform, AudioFormat format) : ysWindowSystemObject("AUDIO_FILE", platform) {
    m_format = format;

    m_sampleCount = 0;
    m_externalBuffer = 0;

    m_maxBufferSamples = 0;
    m_bufferDataSamples = 0;
    m_currentReadingOffset = 0;

    m_fileOpen = false;
}

ysAudioFile::~ysAudioFile() {
}

ysAudioFile::Error ysAudioFile::OpenFile(const char *fname) {
    if (m_fileOpen) return Error::FileAlreadyOpen;

    return Error::None;
}

ysAudioFile::Error ysAudioFile::CloseFile() {
    if (!m_fileOpen) return Error::NoFileOpen;

    return Error::None;
}

ysAudioFile::Error ysAudioFile::FillBuffer(SampleOffset offset) {
    if (offset > m_sampleCount) return Error::ReadOutOfRange;

    // Use External Buffer
    if (m_externalBuffer != nullptr) {
        if (offset + m_externalBuffer->GetSampleCount() > m_sampleCount) {
            return Error::ReadOutOfRange;
        }

        std::vector<int8_t> target(m_externalBuffer->GetBufferSize());

        Error readError = GenericRead(offset, m_externalBuffer->GetSampleCount(), target.data());
        if (readError != Error::None) {
            return readError;
        }

        m_externalBuffer->EditBuffer(target.data());
    }
    // Use Internal Buffer
    else {
        if (offset + m_maxBufferSamples > m_sampleCount) {
            return Error::ReadOutOfRange;
        }
        m_buffer.resize(m_maxBufferSamples);
            
        Error readError = GenericRead(offset, m_maxBufferSamples, (void *)m_buffer.data());
        if (readError != Error::None) {
            return readError;
        }
    }

    return Error::None;
}

ysAudioFile::Error ysAudioFile::GenericRead(SampleOffset offset, SampleOffset size, void *target) {
    if (!m_fileOpen) return Error::NoFileOpen;

    return Error::None;
}

ysAudioFile::Error ysAudioFile::InitializeInternalBuffer(SampleOffset samples, bool saveData) {
    if (!m_fileOpen) return Error::NoFileOpen;

    int newSize = m_audioParameters.GetSizeFromSamples(samples);
    m_buffer.resize(newSize);

    m_maxBufferSamples = samples;
    m_bufferDataSamples = 0;
    m_externalBuffer = nullptr;

    return Error::None;
}

void ysAudioFile::DestroyInternalBuffer() {
    m_maxBufferSamples = 0;
    m_bufferDataSamples = 0;
}

ysAudioFile::Error ysAudioFile::AttachExternalBuffer(ysAudioBuffer *buffer) {
    DestroyInternalBuffer();

    m_externalBuffer = buffer;

    return Error::None;
}
