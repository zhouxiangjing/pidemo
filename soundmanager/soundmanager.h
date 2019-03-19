#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <string>
#include <fcntl.h>
#include <unistd.h>

#include "soundstream.h"
#include "soundrecorder.h"

class SoundManager {

public:
    SoundManager();
    ~SoundManager();

    int loadWavFormatInformation();
};

#endif