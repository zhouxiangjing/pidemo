#ifndef SOUND_STREAM_H
#define SOUND_STREAM_H

#include <string>
#include <iostream>
#include <memory>


struct WavFormat {
    unsigned int wave;  // "WAVE"

    unsigned int fmt_chunk; // "fmt "
    unsigned int fmt_size; // 16
    unsigned short audio_format; //  1: pcm, others: ...
    unsigned short channels; // 1: single, 2: double
    union {
        unsigned int sample_rate;
        unsigned int samples_per_second; //
    };
    union {
        unsigned int byte_rate;
        unsigned int bytes_per_second;
    };
    unsigned short block_align; // channels * bits_per_sample / 8
    unsigned short bits_per_sample;

    union {
        struct {
            unsigned int chunck; // "data"
            unsigned int size;
            unsigned char data[0];
        } data_chunk;
        struct {
            unsigned int chunck; // "LIST"
            unsigned int size;
            unsigned char data[0];
        } list_chunk;
    };
};

struct WavHead {
    unsigned int riff; // "RIFF"
    unsigned int chunk_size;
    WavFormat format;
};

class SoundStream {

public:
    virtual bool open() = 0;

    virtual bool is_open() const = 0;

    virtual void close() = 0;

    virtual std::string name() const = 0;

    virtual double pos() const = 0;

    virtual double total() const = 0;

    virtual void setPos(double pos) = 0;

    virtual bool is_end() const = 0;

    virtual unsigned int channels() const = 0;

    virtual unsigned int sample_rate() const = 0;

    virtual unsigned int byte_rate() const = 0;  // = sample_rate * channels * bits_per_sample / 8
    virtual unsigned int block_align() const = 0; // = channels * bits_persample / 8
    virtual unsigned int bits_per_sample() const = 0;

    virtual int read_pcm(void *data, int length) = 0;
};

class StreamCreator {
public:
    static std::shared_ptr<SoundStream> createSoundStream(const std::string &file);
};

#endif // !SOUND_STREAM_H