#ifndef AIP_H
#define AIP_H

namespace aip {
    class Speech;
};

class Aip {

public:
    Aip();
    ~Aip();

    /**
     * ASR语音识别示例
     */

    void ASR(const char* pcm_data, int size);

    /**
     * ASR语音识别示例,使用远程文件地址
     */
    void ASR_url();

    /**
     * TTS语音合成示例
     */
    void TTS();

    aip::Speech* client;
};

#endif