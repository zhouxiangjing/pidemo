#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include <string>
#include <iostream>
#include <memory>
#include <thread>
#include <stack>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "soundstream.h"
#include "soundpcm.h"

struct SoundStreamPos {
    unsigned hour;
    unsigned minute;
    unsigned second;
    unsigned millisec;

    SoundStreamPos() : hour(0), minute(0), second(0), millisec(0) {}

    SoundStreamPos(unsigned h, unsigned m, unsigned s = 0, unsigned mi = 0) : hour(h), minute(m), second(s), millisec(mi) {}

    static SoundStreamPos fromSeconds(double seconds);

    double toSeconds() const;
};

class SoundPlayer {

public:
    SoundPlayer(const std::string &name);
    ~SoundPlayer();

    void load(std::shared_ptr<SoundStream> stream);

    std::shared_ptr<SoundStream> stream() const;

    bool start();

    void pause();

    void resume();

    void stop();

    bool should_play() const;

    bool is_playing() const;

    bool is_paused() const;
    
    std::string error_msg() const;

    std::shared_ptr<SoundStream> save_stream();
    void restore_stream();

    void reset() {
        auto playing = is_playing();
        if (playing) stop();
        setPlayPos(0, 0, 0);
        if (playing) start();
    }
    SoundStreamPos currentPos() const;
    double currentSecond() const;
    void setPlayPos(const SoundStreamPos &pos);
    void setPlayPos(unsigned h, unsigned m, unsigned s) { setPlayPos(SoundStreamPos(h, m, s)); }
    double totalSeconds() const;

private:
    bool hw_params_configure();
    bool generateHwParams(HardwareParams &hw);
    void player_playing_on_new_thread();
    void appendBackgroundTask(const std::function<void()> &task);
    void runBackgroundTask();
    void setErrorMsg(const std::string& msg);

    void setShouldPlay(bool yes);
    void setPlayerPlaying(bool yes);
    void setPlayerPause(bool pause);
private:
    bool m_player_paused;
    bool m_player_playing;
    bool m_player_alive;
    bool m_player_should_play;
    std::string m_player_name;
    std::string m_player_err_msg;

    mutable std::mutex m_raw_stream_mutext;
    std::shared_ptr<SoundStream> m_raw_stream;

    std::thread m_player_play_thread;
    std::mutex m_background_task_mutex;
    std::mutex m_variant_mutex;
    std::condition_variable m_background_task_cv;
    std::queue<std::function<void()>> m_background_task_queue;
    std::stack<std::shared_ptr<SoundStream>> m_stream_stack;  // 后进先出的容器 只允许一端进出数据
    std::shared_ptr<SoundPCM> m_pcm;

};

#endif // !SOUND_PLAYER_H