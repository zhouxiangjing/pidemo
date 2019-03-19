#include "soundrecordstream.h"

#include <stdio.h>
#include <string.h>

SoundRecordStream::SoundRecordStream() {

    m_max_size = 1024*1024;
    m_data = new char[m_max_size];
    
    memset(m_data, 0, m_max_size);

    m_channels = 1;
    m_samples_per_second = 16000;
    m_bits_per_sample = 16;
}

SoundRecordStream::~SoundRecordStream() {

    m_max_size = 0;
    delete[] m_data;
    m_data = nullptr;
}

char* SoundRecordStream::data() {

    if(nullptr == m_data) 
        return nullptr;

    return m_data;
}

size_t SoundRecordStream::size() {

    if(nullptr == m_data) 
        return 0;

    return m_cur_pos;
}

void SoundRecordStream::clear() {

    if(nullptr != m_data) {
        memset(m_data, 0, m_max_size);
    }
}

int SoundRecordStream::append(char* data, size_t size) {

    if(nullptr == m_data || nullptr == data || size <= 0) 
        return -1;

    printf("### zxj ### m_max_size data_size m_cur_pos  %d %d %d\n", m_max_size, size, m_cur_pos);
    if(size + m_cur_pos >= m_max_size) {
        return -2;
    }

    memcpy(m_data + m_cur_pos, data, size);
    m_cur_pos += size;

    return 0;
}

int SoundRecordStream::save_wav_file(const char* wav_file) {

    // m_channels = 1;
    // m_samples_per_second = 44100;
    // m_bits_per_sample = 16;

    WavHead wav_head;
    unsigned int head_size = sizeof(wav_head);
    unsigned int bytes_per_second = m_samples_per_second*m_bits_per_sample/8;
    unsigned int pcm_size = m_cur_pos;
    unsigned int seconds = pcm_size/bytes_per_second;
    
    printf("### zxj ### head_size bytes_per_second pcm_size seconds %u %u %u %u\n", head_size, bytes_per_second, pcm_size, seconds);
    wav_head.riff = 1179011410;
    wav_head.chunk_size = head_size - 8 + pcm_size;
    wav_head.format.wave = 1163280727;
    wav_head.format.fmt_chunk = 544501094;
    wav_head.format.fmt_size = 16;
    wav_head.format.audio_format = 1;
    wav_head.format.channels = m_channels;
    wav_head.format.sample_rate = m_samples_per_second;
    wav_head.format.samples_per_second = m_samples_per_second;
    wav_head.format.byte_rate = bytes_per_second;
    wav_head.format.bytes_per_second = bytes_per_second;
    wav_head.format.block_align = m_bits_per_sample/8;
    wav_head.format.bits_per_sample = m_bits_per_sample;
    wav_head.format.data_chunk.chunck = 1635017060;
    wav_head.format.data_chunk.size = pcm_size;
    wav_head.format.list_chunk.chunck = 1635017060;
    wav_head.format.list_chunk.size = pcm_size;

    FILE* fp = fopen(wav_file, "wb");
    if (nullptr == fp) {
        return -1;
    }

    fwrite(&wav_head, 1, head_size, fp);
    fwrite(m_data, 1, pcm_size, fp);
    fclose(fp);

    return 0;
}

void SoundRecordStream::set_wav_info(unsigned int channels, unsigned int samples_per_second, unsigned int bits_per_sample) {

    m_channels = channels;
    m_samples_per_second = samples_per_second;
    m_bits_per_sample = bits_per_sample;
}

unsigned int SoundRecordStream::get_channels() const {
    return m_channels;
}
unsigned int SoundRecordStream::get_samples_per_second() const {
    return m_samples_per_second;
}
unsigned int SoundRecordStream::get_bits_per_sample() const {
    return m_bits_per_sample;
}