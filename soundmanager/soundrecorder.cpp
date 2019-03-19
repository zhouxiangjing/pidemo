#include "soundrecorder.h"

SoundRecorder::SoundRecorder(const std::string &name)
: m_recorder_alive(true)
, m_recorder_should_record(false)
, m_recorder_recording(false)
, m_recorder_paused(false)
, m_recorder_name(name)
, m_recorder_stream(std::make_shared<SoundRecordStream>())
, m_recorder_pcm(std::make_shared<SoundPCM>()) {

    m_period_size = 0;

    m_recorder_record_thread = std::thread([=]{
        while (m_recorder_alive) {
            runBackgroundTask();
        }
    });
}

SoundRecorder::~SoundRecorder() {

    m_recorder_alive = false;
    m_recorder_should_record = false;
    m_background_task_cv.notify_all();
    if(m_recorder_record_thread.joinable()) {
        m_recorder_record_thread.join();
    }
}

void SoundRecorder::load() {

}

bool SoundRecorder::start() {

    if(!m_recorder_pcm->is_open() && !m_recorder_pcm->open(m_recorder_name, STREAM_CAPTURE)) {
        setErrorMsg(m_recorder_pcm->error_msg());
        return false;
    }

    if(!hw_params_configure()) {
        return false;
    }

    setShouldRecord(true);
    setRecorderRecording(true);

    appendBackgroundTask([=] {
    try {
            recorder_recording_on_new_thread();
        } catch (const std::exception& e) {
            setErrorMsg(e.what());
        } catch (...) {
            setErrorMsg("Unknow Exception");
        }
    });

    setErrorMsg("Success");
    return true;
}

void SoundRecorder::pause() {
    std::lock_guard<std::mutex> lock(m_variant_mutex);
    m_recorder_paused = true;
}

void SoundRecorder::resume() {
    std::lock_guard<std::mutex> lock(m_variant_mutex);
    m_recorder_paused = false;
}

void SoundRecorder::stop() {
    std::lock_guard<std::mutex> lock(m_variant_mutex);
    m_recorder_should_record = false;
}

bool SoundRecorder::should_record() const {
    return m_recorder_should_record;
}

bool SoundRecorder::is_recording() const {
    return m_recorder_recording;
}

bool SoundRecorder::is_paused() const {
    return m_recorder_paused;
}

std::string SoundRecorder::error_msg() const {
    return m_recorder_err_msg;
}

std::string SoundRecorder::push_pcm_data() {

    setRecorderPause(true);

    std::string data(m_recorder_stream->data(), m_recorder_stream->size());
    m_recorder_stream->clear();

    setRecorderPause(false);
    return data;
}

int SoundRecorder::set_wav_info(unsigned int channels, unsigned int samples_per_second, unsigned int bits_per_sample) {

    if(is_recording()) {
        printf("### zxj ### is_recording. \n");
        return -1;
    }

    m_recorder_stream->set_wav_info(channels, samples_per_second, bits_per_sample);
    return 0;
}

int SoundRecorder::save_wav_file(const char* wav_file) {
    return m_recorder_stream->save_wav_file(wav_file);
}

bool SoundRecorder::hw_params_configure() {

    auto hw_params = m_recorder_pcm->hw_params();

    if (!generateHwParams(hw_params)) {
        return false;
    }

    if(!m_recorder_pcm->set_hw_params(hw_params)) {
        setErrorMsg(m_recorder_pcm->error_msg());
        return false;
    }

    return true;
}

bool SoundRecorder::generateHwParams(HardwareParams &hw) {

    int dir = 0;
    unsigned int channels = m_recorder_stream->get_channels();
    unsigned int sample_rate =  m_recorder_stream->get_samples_per_second();
    unsigned int format, bits_per_sample;
    format = bits_per_sample = m_recorder_stream->get_bits_per_sample();

    unsigned int buffer_time = 50000;
    unsigned int period_time = buffer_time / 4;
    
    if (!hw.set_access(ACCESS_RW_INTERLEAVED)) {
        setErrorMsg(hw.error_msg());
        return false;
    }

    if (!hw.set_channels(channels)) {
        setErrorMsg(hw.error_msg());
        return false;
    }

    if (!hw.set_format(format == 8 ? FORMAT_U8 : FORMAT_S16_LE)) {
        setErrorMsg(hw.error_msg());
        return false;
    }

    if (!hw.set_rate_near(&sample_rate, &dir)) {
        setErrorMsg(hw.error_msg());
        return false;
    }
    
    if (!hw.set_buffer_time_near(&buffer_time, &dir)) {
        setErrorMsg(hw.error_msg());
        return false;
    }
    
    if(!hw.set_period_time(&buffer_time, &dir)) {
        setErrorMsg(hw.error_msg());
        return false;
    }

    m_period_size = hw.get_period_size();
    if(m_period_size < 0) {
        setErrorMsg(hw.error_msg());
        return false;
    }

    m_bytes_per_frame = bits_per_sample * channels / 8;
    m_bytes_per_period = m_period_size * m_bytes_per_frame;

    return true;
}

void SoundRecorder::runBackgroundTask() {
    std::function<void()> task;

    {
        std::unique_lock<std::mutex> lock(m_background_task_mutex);

        m_background_task_cv.wait(lock, [=]{
            return !m_recorder_alive || !m_background_task_queue.empty();
        });
        
        if(!m_background_task_queue.empty() ) {
            task = m_background_task_queue.front();
            m_background_task_queue.pop();
        }
    }

    if(task) task();
}

void SoundRecorder::appendBackgroundTask(const std::function<void()> &task) {

    std::unique_lock<std::mutex> lock(m_background_task_mutex);
    m_background_task_queue.push(task);
    m_background_task_cv.notify_all();
}

void SoundRecorder::recorder_recording_on_new_thread() {
    printf("### zxj ### thread start. \n");
    int ret_frames = 0;
    m_recorder_stream->clear();
    char* buffer = new char[m_bytes_per_period];
    printf("### zxj ### m_bytes_per_frame = %d m_bytes_per_period = %d \n", m_bytes_per_frame, m_bytes_per_period);
    m_recorder_pcm->prepare();
    while(should_record()) {
        if (is_paused()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } else {
            memset(buffer, 0, m_bytes_per_period);
            ret_frames = m_recorder_pcm->readi(buffer, m_period_size);
            if (ret_frames == -EAGAIN ) {
                m_recorder_pcm->wait(1000);
                printf("### zxj ### snd_pcm_readi return EAGAIN.\n");
            } else if (ret_frames == -EPIPE) {
                m_recorder_pcm->prepare();
                printf("### zxj ### snd_pcm_readi return EPIPE.\n");
            } else if (ret_frames == -ESTRPIPE) {
                printf("### zxj ### snd_pcm_readi return ESTRPIPE.\n");
            } else if(ret_frames < 0) {
                printf("### zxj ### snd_pcm_readi faild.\n");
                setShouldRecord(false);
                break;
            } else if (ret_frames > 0) {
                m_recorder_stream->append(buffer, ret_frames*m_bytes_per_frame);
            }
        }
    }

    delete[] buffer;
    m_recorder_pcm->close();
    setRecorderRecording(false);
    setRecorderPause(false);
    
    printf("### zxj ### thread end. size = %d \n", m_recorder_stream->size());
}

void SoundRecorder::setShouldRecord(bool yes) {
    m_recorder_should_record = yes;
}

void SoundRecorder::setRecorderRecording(bool yes) {
    std::lock_guard<std::mutex> lock(m_variant_mutex);
    m_recorder_recording = yes;
}

void SoundRecorder::setRecorderPause(bool pause) {
    std::lock_guard<std::mutex> lock(m_variant_mutex);
    m_recorder_paused = pause;
}

void SoundRecorder::setErrorMsg(const std::string &msg) {

    std::lock_guard<std::mutex> lock(m_variant_mutex);
    m_recorder_err_msg = msg;
}