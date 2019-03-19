

#include "aip.h"
#include <stdio.h>
#include <string.h>

#include "aip-cpp-sdk-0.7.4/speech.h"

Aip::Aip() {

    client = new aip::Speech("15647944", "qkSqfVh0zafsSsB2ZXb6WSlG","egsXHnZGI2BIGKBaY4ekyxzu7TxRqQ1N");
}

Aip::~Aip() {
    delete client;
}

/**
 * ASR语音识别示例
 */
void Aip::ASR(const char* pcm_data, int size) {
    std::map<std::string, std::string> options;
    options["lan"] = "ZH";

    FILE* fp = fopen("../assets/16k_test.pcm", "rb");
    if(nullptr != fp) {

        fseek(fp, 0, SEEK_END);
        size_t pcm_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        char* data = new char[pcm_size];
        memset(data, 0, pcm_size);
        fread(data, 1, pcm_size, fp);

        std::string file_content(data, pcm_size);
        Json::Value result = client->recognize(file_content, "pcm", 16000, options);
        std::cout << "语音识别本地文件结果:" << std::endl << result.toStyledString();

        delete[] data;
        fclose(fp);
    } else {
        printf("### zxj ### open file faild.\n");
    }
}

/**
 * ASR语音识别示例,使用远程文件地址
 */
void Aip::ASR_url() {
    std::map<std::string, std::string> options;
    options["lan"] = "zh";
    Json::Value result =
    client->recognize_url("http://bos.nj.bpc.baidu.com/v1/audio/8k.amr",
                          "http://your_site/dump",
                          "amr", 8000, options);
    std::cout << "语音识别远程文件结果:" << std::endl << result.toStyledString();
}

/**
 * TTS语音合成示例
 */
void Aip::TTS() {
    std::ofstream ofile;
    std::string file_ret;
    std::map<std::string, std::string> options;
    options["spd"] = "5";
    options["per"] = "2";
    ofile.open("../assets/tts.mp3", std::ios::out | std::ios::binary);
    Json::Value result = client->text2audio("百度语音合成测试", options, file_ret);
    // 如果file_ret为不为空则说明合成成功，返回mp3文件内容
    if (!file_ret.empty())
    {
        // 合成成功保存文件
        ofile << file_ret;
        std::cout << "语音合成成功，打开目录下的tts.mp3文件听听看" << std::endl;
    } else {
        // 合成出错，打印错误信息
        std::cout << result.toStyledString();
    }
}