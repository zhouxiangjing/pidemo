#ifndef SOUND_PCM_H
#define SOUND_PCM_H

#include <string>
#include "soundpcmenums.h"
#include "soundpcmhwparams.h"

typedef struct _snd_pcm snd_pcm_t;

class SoundPCM
{
    friend class HardwareParams;
public:

public:
    SoundPCM();

    bool open(const std::string& name, stream_t stream, int mode = 0);
    void close();
    bool is_open() const;

    bool prepare();
    bool wait(int ms);

    stream_t stream() const;
    format_t format() const;


    std::string name() const;
    std::string error_msg() const;

    HardwareParams hw_params();

    bool set_hw_params(HardwareParams& hw_params);

    int writei(const void *data, int length);
    int readi(void *data, int length);

    int bytes_to_frames(int bytes);
    int frames_to_bytes(int frames);

    ~SoundPCM();

public:




private:
    snd_pcm_t *_raw_pcm;
    bool _pcm_open;
    std::string _pcm_error_msg;
};

#endif /* SOUND_PCM_H */
