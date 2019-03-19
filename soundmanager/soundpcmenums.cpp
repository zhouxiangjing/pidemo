#include "soundpcmenums.h"
#include <alsa/asoundlib.h>

std::string type_name(type_t type) {
    return snd_pcm_type_name((snd_pcm_type_t)type);
}

std::string stream_name(stream_t stream) {
    return snd_pcm_stream_name((snd_pcm_stream_t)stream);
}

std::string access_name(access_t access) {
    return snd_pcm_access_name((snd_pcm_access_t)access);
}

std::string format_name(format_t format) {
    return snd_pcm_format_name((snd_pcm_format_t)format);
}

std::string format_description(format_t format) {
    return snd_pcm_format_description((snd_pcm_format_t)format);
}

std::string subformat_name(subformat_t subformat) {
    return snd_pcm_subformat_name((snd_pcm_subformat_t)subformat);
}

format_t format_value(const std::string &name) {
    return (format_t) snd_pcm_format_value(name.c_str());
}

std::string tstamp_mode_name(tstamp_t mode) {
    return snd_pcm_tstamp_mode_name((snd_pcm_tstamp_t)mode);
}

std::string state_name(state_t state) {
    return snd_pcm_state_name((snd_pcm_state_t)state);
}