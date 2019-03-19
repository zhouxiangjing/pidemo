#include "soundwavstream.h"
#include <fcntl.h>
#include <unistd.h>

SoundWavStream::SoundWavStream(const std::string &file) : 
m_wav_handle(-1),
m_file_path(file),
m_data_offset(0) {

}

SoundWavStream::~SoundWavStream() {
    close();
}

bool SoundWavStream::open() {

    m_wav_handle = ::open(m_file_path.c_str(), O_RDONLY);

    loadWavFormatInformation();
    if(m_wav_handle >= 0) {
        return 0;
    }

    return -1;
}

bool SoundWavStream::is_open() const {
    return m_wav_handle >= 0;
}

void SoundWavStream::close() {
    if (m_wav_handle >= 0) {
        ::close(m_wav_handle);
        m_wav_handle = -1;
    }
}

std::string SoundWavStream::name()  const {
    return m_file_path;
}

void SoundWavStream::loadWavFormatInformation() {
    int head_size = sizeof(m_wav_head);
    char data_chunk[4] = {'d', 'a', 't', 'a'};

    if (::read(m_wav_handle, &m_wav_head, head_size) == head_size) {

        while (m_wav_head.format.data_chunk.chunck != *(int *) data_chunk) {
            lseek(m_wav_handle, m_wav_head.format.list_chunk.size, SEEK_CUR);
            ::read(m_wav_handle, &m_wav_head.format.data_chunk, sizeof(m_wav_head.format.data_chunk));
        }
        m_data_offset = lseek(m_wav_handle, 0, SEEK_CUR);
        m_end_of_file = lseek(m_wav_handle, 0, SEEK_END);
        lseek(m_wav_handle, m_data_offset, SEEK_SET);
    }
}

int SoundWavStream::read_pcm(void *data, int length) {
    return ::read(m_wav_handle, data, length);
}

double SoundWavStream::total() const {
//    std::cout << "bytes: " << m_wav_head.format.data_chunk.size << ", bytes per second: " << m_wav_head.format.bytes_per_second << std::endl;
    return double(m_wav_head.format.data_chunk.size) / m_wav_head.format.bytes_per_second;
}

double SoundWavStream::pos() const {
    return double(lseek(m_wav_handle, 0, SEEK_CUR) - m_data_offset) / m_wav_head.format.bytes_per_second;
}

void SoundWavStream::setPos(double pos) {
    unsigned long long offset = m_wav_head.format.bytes_per_second * pos;
    if (offset < m_wav_head.format.data_chunk.size) {
        lseek(m_wav_handle, m_data_offset + offset, SEEK_SET);
    }
}

bool SoundWavStream::is_end() const {
    return lseek(m_wav_handle, 0, SEEK_CUR) == m_end_of_file;
}

unsigned int SoundWavStream::channels() const {
    return m_wav_head.format.channels;
}

unsigned int SoundWavStream::sample_rate() const {
    return m_wav_head.format.sample_rate;
}

unsigned int SoundWavStream::byte_rate() const {
    return m_wav_head.format.byte_rate;
}

unsigned int SoundWavStream::block_align() const {
    return m_wav_head.format.block_align;
}

unsigned int SoundWavStream::bits_per_sample() const {
    return m_wav_head.format.bits_per_sample;
}