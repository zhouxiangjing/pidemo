#include "soundplayer.h"

SoundStreamPos SoundStreamPos::fromSeconds(double seconds) {
    SoundStreamPos pos{};
    unsigned long long second = seconds;
    pos.second = second % 60;
    pos.minute = second / 60;
    pos.hour = pos.minute / 60;
    pos.minute = pos.minute % 60;
    pos.millisec = (seconds - second) * 1000;
    return pos;
}

double SoundStreamPos::toSeconds() const {
    return hour * 3600ULL + minute * 60ULL + second + millisec / 1000.0;
}

SoundPlayer::SoundPlayer(const std::string &name)
: m_player_playing(false)
, m_player_paused(false)
, m_player_alive(true)
, m_player_should_play(false)
, m_player_name(name)
, m_pcm(std::make_shared<SoundPCM>()) {

    m_player_play_thread = std::thread([=]{
        while (m_player_alive) {
            runBackgroundTask();
        }
    });
    
}

SoundPlayer::~SoundPlayer() {

    m_player_alive = false;
    m_player_should_play = false;
    m_background_task_cv.notify_all();
    if(m_player_play_thread.joinable()) {
        m_player_play_thread.join();
    }
}

void SoundPlayer::appendBackgroundTask(const std::function<void()> &task) {

    std::unique_lock<std::mutex> lock(m_background_task_mutex);
    m_background_task_queue.push(task);
    m_background_task_cv.notify_all();
}

void SoundPlayer::runBackgroundTask() {

    std::function<void()> task;

    {
        std::unique_lock<std::mutex> lock(m_background_task_mutex);

        m_background_task_cv.wait(lock, [=]{
            return !m_player_alive || !m_background_task_queue.empty();
        });
        
        if(!m_background_task_queue.empty() ) {
            task = m_background_task_queue.front();
            m_background_task_queue.pop();
        }
    }

    if(task) task();
}

void SoundPlayer::load(std::shared_ptr<SoundStream> stream) {

    if(is_playing()) stop();

    {
        std::lock_guard<std::mutex> lock(m_raw_stream_mutext);
        m_raw_stream = stream;
    }

}

std::shared_ptr<SoundStream> SoundPlayer::stream() const {

    std::lock_guard<std::mutex> lock(m_raw_stream_mutext);
    return m_raw_stream;
}

bool SoundPlayer::start() {

    if (m_player_playing) {
        setErrorMsg("Player is Playing Now");
        return false;
    }

    if (stream() == nullptr) {
        setErrorMsg("Sound Stream Not Set");
        return false;
    }

    if(!stream()->is_open() && !stream()->open()) {
        setErrorMsg("Sound Stream Open Failed");
        return false;
    }

    if(!m_pcm->is_open() && !m_pcm->open(m_player_name, STREAM_PLAYBACK)) {
        setErrorMsg(m_pcm->error_msg());
        return false;
    }

    if(!hw_params_configure()) {
        return false;
    }

    setShouldPlay(true);
    setPlayerPlaying(true);

    appendBackgroundTask([=] {
    try {
            player_playing_on_new_thread();
        } catch (const std::exception& e) {
            setErrorMsg(e.what());
        } catch (...) {
            setErrorMsg("Unknow Exception");
        }
    });

    setErrorMsg("Success");
    return true;
}

void SoundPlayer::pause() {
    std::lock_guard<std::mutex> lock(m_variant_mutex);
    if (m_raw_stream) m_player_paused = true;
}

void SoundPlayer::resume() {
    std::lock_guard<std::mutex> lock(m_variant_mutex);
    if (m_raw_stream) m_player_paused = false;
}

void SoundPlayer::stop() {

    std::lock_guard<std::mutex> lock(m_variant_mutex);
    m_player_should_play = false;
}

bool SoundPlayer::should_play() const {
    return m_player_should_play;
}

bool SoundPlayer::is_playing() const {
    return m_player_playing;
}

bool SoundPlayer::is_paused() const {
    return m_player_paused;
}

std::string SoundPlayer::error_msg() const {
    return m_player_err_msg;
}

std::shared_ptr<SoundStream> SoundPlayer::save_stream() {
    if (m_raw_stream) m_stream_stack.push(m_raw_stream);
    return m_raw_stream;
}

void SoundPlayer::restore_stream() {
    if (!m_stream_stack.empty()) {
        m_raw_stream = m_stream_stack.top();
        m_stream_stack.pop();
    }
}

SoundStreamPos SoundPlayer::currentPos() const {
    return SoundStreamPos::fromSeconds(currentSecond());
}

double SoundPlayer::currentSecond() const {
    return m_raw_stream ? m_raw_stream->pos() : 0;
}

void SoundPlayer::setPlayPos(const SoundStreamPos &pos) {
    pause();
    if (m_raw_stream) {
        m_raw_stream->setPos(pos.toSeconds());
    }
    resume();
}

double SoundPlayer::totalSeconds() const {
    return m_raw_stream ? m_raw_stream->total() : 0;
}

bool SoundPlayer::hw_params_configure() {

    auto hw_params = m_pcm->hw_params();

    if (!generateHwParams(hw_params)) {
        return false;
    }

    if(!m_pcm->set_hw_params(hw_params)) {
        setErrorMsg( m_pcm->error_msg());
        return false;
    }
    return true;
}

bool SoundPlayer::generateHwParams(HardwareParams &hw) {

    if (m_raw_stream == nullptr) {
        setErrorMsg("Sound Stream Not Set");
        return false;
    }

    int dir = 0;
    auto channels = m_raw_stream->channels();
    auto sample_rate = m_raw_stream->sample_rate();
    auto format = m_raw_stream->bits_per_sample();

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
    unsigned int buffer_time = 50000;
    if (!hw.set_buffer_time_near(&buffer_time, &dir)) {
        setErrorMsg(hw.error_msg());
        return false;
    }
    unsigned long buffer_size = m_raw_stream->byte_rate();
    if (!hw.set_buffer_size_near(&buffer_size)) {
        setErrorMsg(hw.error_msg());
        return false;
    }

    return true;
}

void SoundPlayer::player_playing_on_new_thread() {

    if (stream() == nullptr)  {
        std::cout << "### zxj ### stream() nullptr." << std::endl;
        return;
    }

    auto pcm_block_align = stream()->block_align();
    auto samples_per_second = stream()->sample_rate();
    auto size = m_pcm->frames_to_bytes(samples_per_second);
    if (samples_per_second == 0)  {
        std::cout << "### zxj ### samples_per_second == 0." << std::endl;
        return;
    }

    std::vector<char> buffer(size);
    long long time_per_sample = 1.0 / samples_per_second * 1000000; // single sample time: us

    m_pcm->prepare();

    while (should_play()) {
        if (is_paused()) { // if paused, give the CPU time to the other threads
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } else {
            auto second_start = std::chrono::system_clock::now();
            auto bytes = m_raw_stream->read_pcm(buffer.data(), buffer.size());

            if (bytes > 0) {
                auto frames = m_pcm->bytes_to_frames(bytes);
                auto buffer_start = buffer.data();
                do {
                    auto frame_start = std::chrono::system_clock::now();
                    auto frame = m_pcm->writei(buffer_start, frames);

                    if (frame == -EPIPE) {
                        m_pcm->prepare();
                    } else if (frame > 0) {

                        buffer_start += frame * pcm_block_align;
                        frames -= frame;

                        auto frame_stop = std::chrono::system_clock::now();
                        long long frame_left = time_per_sample * frame -std::chrono::duration_cast<std::chrono::microseconds>(frame_stop - frame_start).count();
                        if (frame_left > 0) {
                            std::this_thread::sleep_for(std::chrono::microseconds(frame_left));
                        }
                    }
                } while (should_play() && frames > 0);

            }

            if (!should_play() || stream() == nullptr || stream()->is_end()) {
                setShouldPlay(false);
                break;
            }

            auto second_stop = std::chrono::system_clock::now();
            auto left_milli = 1000 - std::chrono::duration_cast<std::chrono::milliseconds>(
                    second_stop - second_start).count();
            if (should_play() && left_milli > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(left_milli));
            }
        }
    }
    //snd_pcm_drain()
    m_pcm->close();
    setPlayerPlaying(false);
    setPlayerPause(false);
}

void SoundPlayer::setErrorMsg(const std::string &msg) {

    std::lock_guard<std::mutex> lock(m_variant_mutex);
    m_player_err_msg = msg;
}

void SoundPlayer::setShouldPlay(bool yes) {
    m_player_should_play = yes;
}

void SoundPlayer::setPlayerPlaying(bool yes) {
    std::lock_guard<std::mutex> lock(m_variant_mutex);
    m_player_playing = yes;
}

void SoundPlayer::setPlayerPause(bool pause) {
    std::lock_guard<std::mutex> lock(m_variant_mutex);
    m_player_paused = pause;
}