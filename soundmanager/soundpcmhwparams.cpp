#include "soundpcmhwparams.h"
#include "soundpcm.h"
#include <alsa/asoundlib.h>

HardwareParams::HardwareParams()
    : _raw_pcm(nullptr), _raw_hw_params(nullptr) {
    snd_pcm_hw_params_malloc(&_raw_hw_params);
}

HardwareParams::HardwareParams(SoundPCM *pcm)
    : _raw_pcm(pcm->_raw_pcm), _raw_hw_params(nullptr) {
    snd_pcm_hw_params_malloc(&_raw_hw_params);
    auto ret = snd_pcm_hw_params_any(_raw_pcm, _raw_hw_params);
    _hw_error_msg = snd_strerror(ret < 0 ? ret : 0);
}


HardwareParams::HardwareParams(const HardwareParams &other)
    : _raw_pcm(other._raw_pcm), _raw_hw_params(nullptr) {
    snd_pcm_hw_params_malloc(&_raw_hw_params);
    snd_pcm_hw_params_copy(_raw_hw_params, other._raw_hw_params);
}

HardwareParams::HardwareParams(HardwareParams &&other)
    : _raw_pcm(other._raw_pcm), _raw_hw_params(other._raw_hw_params) {
    other._raw_hw_params = nullptr;
}

std::string HardwareParams::error_msg() const {
    return _hw_error_msg;
}

bool HardwareParams::any() {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_any(_raw_pcm, _raw_hw_params));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_access(access_t access) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_access(_raw_pcm, _raw_hw_params, (snd_pcm_access_t)access));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_access_first(access_t *access) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_access_first(_raw_pcm, _raw_hw_params, (snd_pcm_access_t *)access));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_access_last(access_t *access) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_access_last(_raw_pcm, _raw_hw_params, (snd_pcm_access_t *)access));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_size(unsigned long val) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_size(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_size_fisrt(unsigned long *val) {
    if(check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_size_first(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_size_last(unsigned long *val) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_size_last(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_size_min(unsigned long *val) {
    if(check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_size_min(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_size_max(unsigned long *val) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_size_max(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_size_near(unsigned long *val) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_size_near(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_size_minmax(unsigned long *min, unsigned long *max) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_size_minmax(_raw_pcm, _raw_hw_params, min, max));

    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_time(unsigned int val, int dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_time(_raw_pcm, _raw_hw_params, val, dir));

    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_time_first(unsigned int *val, int *dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_time_first(_raw_pcm, _raw_hw_params, val, dir));

    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_time_last(unsigned int *val, int *dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_time_last(_raw_pcm, _raw_hw_params, val, dir));

    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_time_min(unsigned int *val, int *dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_time_min(_raw_pcm, _raw_hw_params, val, dir));

    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_time_max(unsigned int *val, int *dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_time_max(_raw_pcm, _raw_hw_params, val, dir));

    return _hw_error_msg == "Success";
}

bool HardwareParams::set_buffer_time_near(unsigned int *val, int *dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_buffer_time_near(_raw_pcm, _raw_hw_params, val, dir));

    return _hw_error_msg == "Success";
}

bool HardwareParams::set_channels(unsigned int val) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_channels(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_channels_first(unsigned int *val) {
    if(!check_pcm())
        return false;
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_channels_first(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_channels_last(unsigned int *val) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_channels_last(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_channels_min(unsigned int *val) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_channels_min(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_channels_max(unsigned int *val) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_channels_max(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_channels_minmax(unsigned int *min, unsigned int *max) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_channels_minmax(_raw_pcm, _raw_hw_params, min, max));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_channels_near(unsigned int *val) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_channels_near(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_export_buffer(unsigned int val) {
    if(!check_pcm())
        return false;
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_export_buffer(_raw_pcm, _raw_hw_params, val));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_format(format_t format) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_format(_raw_pcm, _raw_hw_params, (snd_pcm_format_t)format));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_format_first(format_t *format) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_format_first(_raw_pcm, _raw_hw_params, (snd_pcm_format_t *)format));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_format_last(format_t *format) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_format_last(_raw_pcm, _raw_hw_params, (snd_pcm_format_t *)format));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_format_mask(FormatMask &mask) {
    if(!check_pcm())
        return false;
//    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_format_mask(_raw_pcm, _raw_hw_params, mask._raw_mask));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_period_time(unsigned int *val, int *dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_period_time_near(_raw_pcm, _raw_hw_params, val, dir));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_periods(unsigned int val, int dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_periods(_raw_pcm, _raw_hw_params, val, dir));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_periods_first(unsigned int *val, int *dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_periods_first(_raw_pcm, _raw_hw_params, val, dir));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_periods_last(unsigned int *val, int *dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_periods_last(_raw_pcm, _raw_hw_params, val, dir));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_periods_min(unsigned int *val, int *dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_periods_min(_raw_pcm, _raw_hw_params, val, dir));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_periods_max(unsigned int *val, int *dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_periods_max(_raw_pcm, _raw_hw_params, val, dir));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_periods_near(unsigned int *val, int *dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_periods_near(_raw_pcm, _raw_hw_params, val, dir));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_rate(unsigned int val, int dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_rate(_raw_pcm, _raw_hw_params, val, dir));
    return _hw_error_msg == "Success";
}

bool HardwareParams::set_rate_near(unsigned int *val, int *dir) {
    if(!check_pcm()) {
        return false;
    }
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_set_rate_near(_raw_pcm, _raw_hw_params, val, dir));
    return _hw_error_msg == "Success";
}

size_t HardwareParams::get_period_size() {

    if(!check_pcm()) {
        return -1;
    }

    snd_pcm_uframes_t chunk_size;
    _hw_error_msg = snd_strerror(snd_pcm_hw_params_get_period_size(_raw_hw_params, &chunk_size, 0));
    if(_hw_error_msg == "Success") {
        return (size_t)chunk_size;
    }

    return -2;
}


bool HardwareParams::check_pcm()
{
    if(_raw_pcm == nullptr) {
        _hw_error_msg = "PCM Handle Not Set";
        return false;
    }
    return true;
}

HardwareParams::~HardwareParams() {
    if(_raw_hw_params) {
        snd_pcm_hw_params_free(_raw_hw_params);
        _raw_hw_params = nullptr;
    }
}