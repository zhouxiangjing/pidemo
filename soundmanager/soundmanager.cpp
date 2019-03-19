#include "soundmanager.h"

SoundManager::SoundManager() {

}

SoundManager::~SoundManager() {

}

int SoundManager::loadWavFormatInformation() {

    std::string wav_file = "/home/pi/ai/16k.wav";

    WavHead wav_head;
    int head_size = sizeof(wav_head);
    char data_chunk[4] = {'d', 'a', 't', 'a'};
    
    int wav_handle = ::open(wav_file.c_str(), O_RDONLY);
    if(wav_handle < 0) {
        printf("### zxj ### open file faild.\n");
        return -1;
    }
        
    if (::read(wav_handle, &wav_head, head_size) == head_size) {
        while (wav_head.format.data_chunk.chunck != *(int *) data_chunk) {
            lseek(wav_handle, wav_head.format.list_chunk.size, SEEK_CUR);
            ::read(wav_handle, &wav_head.format.data_chunk, sizeof(wav_head.format.data_chunk));
        }
    }

    ::close(wav_handle);

    std::cout << "### zxj ### riff: " << wav_head.riff << std::endl;
    std::cout << "### zxj ### chunk_size: " << wav_head.chunk_size << std::endl;

    std::cout << "### zxj ### format.wave: " << wav_head.format.wave << std::endl;
    std::cout << "### zxj ### format.fmt_chunk: " << wav_head.format.fmt_chunk << std::endl;
    std::cout << "### zxj ### format.fmt_size: " << wav_head.format.fmt_size << std::endl;
    std::cout << "### zxj ### format.audio_format: " << wav_head.format.audio_format << std::endl;
    std::cout << "### zxj ### format.channels: " << wav_head.format.channels << std::endl;

    std::cout << "### zxj ### <same 1> format.sample_rate: " << wav_head.format.sample_rate << std::endl;
    std::cout << "### zxj ### <same 1> format.samples_per_second: " << wav_head.format.samples_per_second << std::endl;

    std::cout << "### zxj ### <same 2> format.byte_rate: " << wav_head.format.byte_rate << std::endl;
    std::cout << "### zxj ### <same 2> format.bytes_per_second: " << wav_head.format.bytes_per_second << std::endl;

    std::cout << "### zxj ### format.block_align: " << wav_head.format.block_align << std::endl;
    std::cout << "### zxj ### format.bits_per_sample: " << wav_head.format.bits_per_sample << std::endl;

    std::cout << "### zxj ### <same 1.1> format.data_chunk.chunck: " << wav_head.format.data_chunk.chunck << std::endl;
    std::cout << "### zxj ### <same 1.2>format.data_chunk.size: " << wav_head.format.data_chunk.size << std::endl;

    std::cout << "### zxj ### <same 1.1>format.list_chunk.chunck: " << wav_head.format.list_chunk.chunck << std::endl;
    std::cout << "### zxj ### <same 1.2>format.list_chunk.size: " << wav_head.format.list_chunk.size << std::endl;
    return 0;
}