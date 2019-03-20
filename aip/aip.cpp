

#include "aip.h"
#include <stdio.h>
#include <string.h>

#include "aip-cpp-sdk-0.7.4/speech.h"

Aip::Aip() {

    client = new aip::Speech("", "qkSqfVh0zafsSsB2ZXb6WSlG","egsXHnZGI2BIGKBaY4ekyxzu7TxRqQ1N");
}

Aip::~Aip() {
    delete client;
}

/**
 * ASR语音识别示例
 */
int Aip::ASR(std::string data) {

    printf("### zxj ### data size = %d\n", data.size());
    if(data.size() < 10000) {
        return 0;
    }

    std::map<std::string, std::string> options;
    options["lan"] = "ZH";
    Json::Value result = client->recognize(data, "pcm", 16000, options);
    if(result["err_no"] == 0) {
        std::string ret = result["result"][0].asString();
        std::cout << "语音识别结果 : " <<  ret << std::endl;

        if(ret.find("退出") != std::string::npos || ret.find("结束") != std::string::npos) {
            return -1;
        }
    } else {
        std::cout << "语音识别错误 : " << result["err_no"] << " " << result["err_msg"].asString() << std::endl;
    }
    
    return 0;
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
