
#include <stdio.h>

#include "aip/aip.h"
#include "soundmanager/soundmanager.h"

int main()
{

    SoundRecorder recorder("plughw:1,0");
    recorder.set_wav_info(1, 16000, 16);
    if(!recorder.start()) {
        printf("recorder start faild.\n");
        return -1;
    }
    
    Aip aip;
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        if( -1 == aip.ASR(recorder.push_pcm_data())) {
            printf("### zxj ### 语音命令退出。\n");
            break;
        }
    } while(true);
    
    recorder.stop();

    // Aip aip;
    // aip.ASR(nullptr, 0);
    // aip.ASR_url();
    // aip.TTS();

    // SoundManager sound_manager;
    // sound_manager.loadWavFormatInformation();

    return 0;
}


