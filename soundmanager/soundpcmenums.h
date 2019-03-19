#ifndef SOUND_PCM_ENUMS_H
#define SOUND_PCM_ENUMS_H

#include <string>

typedef enum  {
    /** Kernel level PCM */
    TYPE_HW = 0,
    /** Hooked PCM */
    TYPE_HOOKS,
    /** One or more linked PCM with exclusive access to selected
        channels */
    TYPE_MULTI,
    /** File writing plugin */
    TYPE_FILE,
    /** Null endpoint PCM */
    TYPE_NULL,
    /** Shared memory client PCM */
    TYPE_SHM,
    /** INET client PCM (not yet implemented) */
    TYPE_INET,
    /** Copying plugin */
    TYPE_COPY,
    /** Linear format conversion PCM */
    TYPE_LINEAR,
    /** A-Law format conversion PCM */
    TYPE_ALAW,
    /** Mu-Law format conversion PCM */
    TYPE_MULAW,
    /** IMA-ADPCM format conversion PCM */
    TYPE_ADPCM,
    /** Rate conversion PCM */
    TYPE_RATE,
    /** Attenuated static route PCM */
    TYPE_ROUTE,
    /** Format adjusted PCM */
    TYPE_PLUG,
    /** Sharing PCM */
    TYPE_SHARE,
    /** Meter plugin */
    TYPE_METER,
    /** Mixing PCM */
    TYPE_MIX,
    /** Attenuated dynamic route PCM (not yet implemented) */
    TYPE_DROUTE,
    /** Loopback server plugin (not yet implemented) */
    TYPE_LBSERVER,
    /** Linear Integer <-> Linear Float format conversion PCM */
    TYPE_LINEAR_FLOAT,
    /** LADSPA integration plugin */
    TYPE_LADSPA,
    /** Direct Mixing plugin */
    TYPE_DMIX,
    /** Jack Audio Connection Kit plugin */
    TYPE_JACK,
    /** Direct Snooping plugin */
    TYPE_DSNOOP,
    /** Direct Sharing plugin */
    TYPE_DSHARE,
    /** IEC958 subframe plugin */
    TYPE_IEC958,
    /** Soft volume plugin */
    TYPE_SOFTVOL,
    /** External I/O plugin */
    TYPE_IOPLUG,
    /** External filter plugin */
    TYPE_EXTPLUG,
    /** Mmap-emulation plugin */
    TYPE_MMAP_EMUL,
    TYPE_LAST = TYPE_MMAP_EMUL
} type_t;

typedef enum  {
    /** Playback stream */
    STREAM_PLAYBACK = 0,
    /** Capture stream */
    STREAM_CAPTURE,
    STREAM_LAST = STREAM_CAPTURE
} stream_t;

typedef enum  {
    /** mmap access with simple interleaved channels */
    ACCESS_MMAP_INTERLEAVED = 0,
    /** mmap access with simple non interleaved channels */
    SACCESS_MMAP_NONINTERLEAVED,
    /** mmap access with complex placement */
    ACCESS_MMAP_COMPLEX,
    /** snd_pcm_readi/snd_pcm_writei access */
    ACCESS_RW_INTERLEAVED,
    /** snd_pcm_readn/snd_pcm_writen access */
    ACCESS_RW_NONINTERLEAVED,
    ACCESS_LAST = ACCESS_RW_NONINTERLEAVED
} access_t;

typedef enum  {
    /** Unknown */
    FORMAT_UNKNOWN = -1,
    /** Signed 8 bit */
    FORMAT_S8 = 0,
    /** Unsigned 8 bit */
    FORMAT_U8,
    /** Signed 16 bit Little Endian */
    FORMAT_S16_LE,
    /** Signed 16 bit Big Endian */
    FORMAT_S16_BE,
    /** Unsigned 16 bit Little Endian */
    FORMAT_U16_LE,
    /** Unsigned 16 bit Big Endian */
    FORMAT_U16_BE,
    /** Signed 24 bit Little Endian using low three bytes in 32-bit word */
    FORMAT_S24_LE,
    /** Signed 24 bit Big Endian using low three bytes in 32-bit word */
    FORMAT_S24_BE,
    /** Unsigned 24 bit Little Endian using low three bytes in 32-bit word */
    FORMAT_U24_LE,
    /** Unsigned 24 bit Big Endian using low three bytes in 32-bit word */
    FORMAT_U24_BE,
    /** Signed 32 bit Little Endian */
    FORMAT_S32_LE,
    /** Signed 32 bit Big Endian */
    FORMAT_S32_BE,
    /** Unsigned 32 bit Little Endian */
    FORMAT_U32_LE,
    /** Unsigned 32 bit Big Endian */
    FORMAT_U32_BE,
    /** Float 32 bit Little Endian, Range -1.0 to 1.0 */
    FORMAT_FLOAT_LE,
    /** Float 32 bit Big Endian, Range -1.0 to 1.0 */
    FORMAT_FLOAT_BE,
    /** Float 64 bit Little Endian, Range -1.0 to 1.0 */
    FORMAT_FLOAT64_LE,
    /** Float 64 bit Big Endian, Range -1.0 to 1.0 */
    FORMAT_FLOAT64_BE,
    /** IEC-958 Little Endian */
    FORMAT_IEC958_SUBFRAME_LE,
    /** IEC-958 Big Endian */
    FORMAT_IEC958_SUBFRAME_BE,
    /** Mu-Law */
    FORMAT_MU_LAW,
    /** A-Law */
    FORMAT_A_LAW,
    /** Ima-ADPCM */
    FORMAT_IMA_ADPCM,
    /** MPEG */
    FORMAT_MPEG,
    /** GSM */
    FORMAT_GSM,
    /** Special */
    FORMAT_SPECIAL = 31,
    /** Signed 24bit Little Endian in 3bytes format */
    FORMAT_S24_3LE = 32,
    /** Signed 24bit Big Endian in 3bytes format */
    FORMAT_S24_3BE,
    /** Unsigned 24bit Little Endian in 3bytes format */
    FORMAT_U24_3LE,
    /** Unsigned 24bit Big Endian in 3bytes format */
    FORMAT_U24_3BE,
    /** Signed 20bit Little Endian in 3bytes format */
    FORMAT_S20_3LE,
    /** Signed 20bit Big Endian in 3bytes format */
    FORMAT_S20_3BE,
    /** Unsigned 20bit Little Endian in 3bytes format */
    FORMAT_U20_3LE,
    /** Unsigned 20bit Big Endian in 3bytes format */
    FORMAT_U20_3BE,
    /** Signed 18bit Little Endian in 3bytes format */
    FORMAT_S18_3LE,
    /** Signed 18bit Big Endian in 3bytes format */
    FORMAT_S18_3BE,
    /** Unsigned 18bit Little Endian in 3bytes format */
    FORMAT_U18_3LE,
    /** Unsigned 18bit Big Endian in 3bytes format */
    FORMAT_U18_3BE,
    /* G.723 (ADPCM) 24 kbit/s, 8 samples in 3 bytes */
    FORMAT_G723_24,
    /* G.723 (ADPCM) 24 kbit/s, 1 sample in 1 byte */
    FORMAT_G723_24_1B,
    /* G.723 (ADPCM) 40 kbit/s, 8 samples in 3 bytes */
    FORMAT_G723_40,
    /* G.723 (ADPCM) 40 kbit/s, 1 sample in 1 byte */
    FORMAT_G723_40_1B,
    /* Direct Stream Digital (DSD) in 1-byte samples (x8) */
    FORMAT_DSD_U8,
    /* Direct Stream Digital (DSD) in 2-byte samples (x16) */
    FORMAT_DSD_U16_LE,
    /* Direct Stream Digital (DSD) in 4-byte samples (x32) */
    FORMAT_DSD_U32_LE,
    /* Direct Stream Digital (DSD) in 2-byte samples (x16) */
    FORMAT_DSD_U16_BE,
    /* Direct Stream Digital (DSD) in 4-byte samples (x32) */
    FORMAT_DSD_U32_BE,
    FORMAT_LAST = FORMAT_DSD_U32_BE
} format_t;

typedef enum  {
    /** Standard */
    SUBFORMAT_STD = 0,
    SUBFORMAT_LAST = SUBFORMAT_STD
} subformat_t;

typedef enum  {
    /** Open */
    STATE_OPEN = 0,
    /** Setup installed */
    STATE_SETUP,
    /** Ready to start */
    STATE_PREPARED,
    /** Running */
    STATE_RUNNING,
    /** Stopped: underrun (playback) or overrun (capture) detected */
    STATE_XRUN,
    /** Draining: running (playback) or stopped (capture) */
    STATE_DRAINING,
    /** Paused */
    STATE_PAUSED,
    /** Hardware is suspended */
    STATE_SUSPENDED,
    /** Hardware is disconnected */
    STATE_DISCONNECTED,
    STATE_LAST = STATE_DISCONNECTED
} state_t;

typedef enum {
    /** No timestamp */
    TSTAMP_NONE = 0,
    /** Update timestamp at every hardware position update */
    TSTAMP_ENABLE,
    /** Equivalent with #TSTAMP_ENABLE,
     * just for compatibility with older versions
     */
    TSTAMP_MMAP = TSTAMP_ENABLE,
    TSTAMP_LAST = TSTAMP_ENABLE
} tstamp_t;

typedef enum  {
    /** Automatic start on data read/write */
    START_DATA = 0,
    /** Explicit start */
    START_EXPLICIT,
    START_LAST = START_EXPLICIT
} start_t;

/** PCM xrun mode */
typedef enum  {
    /** Xrun detection disabled */
    XRUN_NONE = 0,
    /** Stop on xrun detection */
    XRUN_STOP,
    XRUN_LAST = XRUN_STOP
} xrun_t;

std::string type_name(type_t type);                         // type: snd_pcm_type_t
std::string stream_name(stream_t stream);                   // stream: snd_pcm_stream_t
std::string access_name(access_t access);                   // access: snd_pcm_access_t
std::string format_name(format_t format);                   // format: snd_pcm_format_t
std::string format_description(format_t format);            // format: snd_pcm_format_t
std::string subformat_name(subformat_t subformat);          // subformat: snd_pcm_subformat_t
std::string subformat_description(subformat_t subformat);   // subformat: snd_pcm_subformat_t
format_t format_value(const std::string& name);
std::string tstamp_mode_name(tstamp_t mode);        // mode: snd_pcm_tstamp_t
std::string state_name(state_t state);              // state: snd_pcm_state_t

#endif /* SOUND_PCM_ENUMS_H */
