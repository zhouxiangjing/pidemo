#ifndef SOUND_PCM_HARDWARE_PARAMS_H
#define SOUND_PCM_HARDWARE_PARAMS_H

#include <string>
#include "soundpcmenums.h"

typedef struct _snd_pcm snd_pcm_t;
typedef struct _snd_pcm_hw_params snd_pcm_hw_params_t;

class SoundPCM;
class AccessMask;
class FormatMask;
class HardwareParams
{
    friend class SoundPCM;
public:
    HardwareParams();
    HardwareParams(const HardwareParams& other);
    HardwareParams(HardwareParams&& other);
    HardwareParams(SoundPCM* pcm);

    std::string error_msg() const;

    bool any();
    bool set_access(access_t access);
    bool set_access_first(access_t *access);
    bool set_access_last(access_t *access);
    bool set_access_mark(AccessMask& mask);


    bool set_buffer_size(unsigned long val);
    bool set_buffer_size_fisrt(unsigned long *val);
    bool set_buffer_size_last(unsigned long *val);
    bool set_buffer_size_min(unsigned long *val);
    bool set_buffer_size_max(unsigned long *val);
    bool set_buffer_size_near(unsigned long *val);
    bool set_buffer_size_minmax(unsigned long *min, unsigned long *max);

    bool set_buffer_time(unsigned int val, int dir);
    bool set_buffer_time_first(unsigned int *val, int *dir);
    bool set_buffer_time_last(unsigned int *val, int *dir);
    bool set_buffer_time_min(unsigned int *val, int *dir);
    bool set_buffer_time_max(unsigned int *val, int *dir);
    bool set_buffer_time_near(unsigned int *val, int *dir);

    bool set_channels(unsigned int val);
    bool set_channels_first(unsigned int *val);
    bool set_channels_last(unsigned int *val);
    bool set_channels_min(unsigned int *val);
    bool set_channels_max(unsigned int *val);
    bool set_channels_minmax(unsigned int *min, unsigned int *max);
    bool set_channels_near(unsigned int *val);

    bool set_export_buffer(unsigned int val);

    bool set_format(format_t format);
    bool set_format_first(format_t *format);
    bool set_format_last(format_t *format);
    bool set_format_mask(FormatMask& mask);

    bool set_period_time(unsigned int *val, int *dir);

    bool set_periods(unsigned int val, int dir);
    bool set_periods_first(unsigned int *val, int *dir);
    bool set_periods_last(unsigned int *val, int *dir);
    bool set_periods_min(unsigned int *val, int *dir);
    bool set_periods_max(unsigned int *val, int *dir);
    bool set_periods_near(unsigned int *val, int *dir);

    bool set_rate(unsigned int val, int dir);
    bool set_rate_near(unsigned int *val, int *dir);

    size_t get_period_size();

    ~HardwareParams();

private:
    bool check_pcm();
private:
    snd_pcm_t * _raw_pcm;
    snd_pcm_hw_params_t *_raw_hw_params;
    std::string _hw_error_msg;
};

#endif /* SOUND_PCM_HARDWARE_PARAMS_H */
