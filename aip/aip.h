#ifndef AIP_H
#define AIP_H

#include <string>

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

    int ASR(std::string data);

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