#include "Utils.h"

bool authenticate(const std::string& fingerprint,std::size_t hashed_fingerprint) {
    if(hashed_fingerprint == std::hash<std::string>{}(fingerprint))
    return true;
    // 抛出的异常类型需要再想想
    throw std::invalid_argument("The fingerprint authenicated failed");
}

const std::string random_account()
{
    // 使用随机数生成器生成一个16位的数字
    std::random_device rd;                     // 获取硬件生成的随机数种子
    std::mt19937 gen(rd());                    // 随机数生成器
    std::uniform_int_distribution<> dis(0, 9); // 每个位置生成一个0-9之间的数字

    std::string random_number;

    // 生成16位数字并将其添加到字符串流中
    for (int i = 0; i < 16; ++i)
    {
        random_number += dis(gen); // 将随机生成的数字加到stream中
    }

    return random_number;
}