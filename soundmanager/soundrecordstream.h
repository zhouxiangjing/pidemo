#ifndef SOUND_RECORD_STREAM_H
#define SOUND_RECORD_STREAM_H

#include "soundstream.h"

class SoundRecordStream {

public:
    SoundRecordStream();
    ~SoundRecordStream();

    char* data();
    size_t size();

    void clear();
    int append(char* data, size_t size);
    void set_wav_info(unsigned int channels, unsigned int samples_per_second, unsigned int bits_per_sample);
    int save_wav_file(const char* wav_file);

    unsigned int get_channels() const;
    unsigned int get_samples_per_second() const;
    unsigned int get_bits_per_sample() const;

private:
    char*   m_data;
    size_t  m_max_size;
    size_t  m_cur_pos;

    unsigned int m_channels;
    unsigned int m_samples_per_second;
    unsigned int m_bits_per_sample;
};

#endif // !SOUND_RECORD_STREAM_H