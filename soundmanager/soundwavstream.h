#ifndef SOUND_WAV_STREAM_H
#define SOUND_WAV_STREAM_H

#include "soundstream.h"

class SoundWavStream : public SoundStream {

public:
    SoundWavStream(const std::string &wav_file);
    ~SoundWavStream();

    bool open() override;

    bool is_open() const override;

    void close() override;

    std::string name() const override;

    double total() const override;

    double pos() const override;

    void setPos(double pos) override;

    bool is_end() const override;

    unsigned int channels() const override;

    unsigned int sample_rate() const override;

    unsigned int byte_rate() const override;

    unsigned int block_align() const override;

    unsigned int bits_per_sample() const override;

    int read_pcm(void *data, int length) override;

private:
    void loadWavFormatInformation();

    int m_wav_handle;
    long long m_data_offset, m_end_of_file;
    std::string m_file_path;
    WavHead m_wav_head;
};

#endif // !SOUND_WAV_STREAM_H