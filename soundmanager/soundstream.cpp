
#include "soundstream.h"
#include "soundwavstream.h"

std::shared_ptr<SoundStream> StreamCreator::createSoundStream(const std::string &file) {
    if (file.find(".wav") != file.npos) {
        return std::make_shared<SoundWavStream>(file);
    }

    return nullptr;
}