#ifndef SOUND_RECORDER_H
#define SOUND_RECORDER_H

#include <string>
#include <iostream>
#include <memory>
#include <thread>
#include <stack>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string.h>

#include "soundrecordstream.h"
#include "soundstream.h"
#include "soundpcm.h"

class SoundRecorder {

public:
    SoundRecorder(const std::string &name);
    ~SoundRecorder();

    void load();

    bool start();

    void pause();

    void resume();

    void stop();

    bool should_record() const;

    bool is_recording() const;

    bool is_paused() const;

    std::string error_msg() const;

    std::string push_pcm_data();
    int set_wav_info(unsigned int channels, unsigned int samples_per_second, unsigned int bits_per_sample);
    int save_wav_file(const char* wav_file);

private:
    bool hw_params_configure();
    bool generateHwParams(HardwareParams &hw);
    void runBackgroundTask();
    void appendBackgroundTask(const std::function<void()> &task);
    void recorder_recording_on_new_thread();
    
    void setShouldRecord(bool yes);
    void setRecorderRecording(bool yes);
    void setRecorderPause(bool pause);

    void setErrorMsg(const std::string& msg);

private:

    size_t m_bytes_per_frame;   // 每帧的字节数
    size_t m_period_size;       // 每次(每个周期)读取的帧数
    size_t m_bytes_per_period;  // 每个周期的读取的字节数

    bool m_recorder_alive;
    bool m_recorder_should_record;
    bool m_recorder_recording;
    bool m_recorder_paused;
    
    std::string m_recorder_name;
    std::string m_recorder_err_msg;

    std::thread m_recorder_record_thread;
    std::mutex m_background_task_mutex;
    std::mutex m_variant_mutex;
    std::condition_variable m_background_task_cv;
    std::queue<std::function<void()>> m_background_task_queue;
    
    std::shared_ptr<SoundRecordStream> m_recorder_stream;
    std::shared_ptr<SoundPCM> m_recorder_pcm;
};

#endif // !SOUND_RECORDER_H