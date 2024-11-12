#include "Utils.h"

bool authenticate(const std::string& fingerprint,std::size_t hashed_fingerprint) {
    if(hashed_fingerprint == std::hash<std::string>{}(fingerprint))
    return true;
    // 抛出的异常类型需要再想想
    throw std::invalid_argument("The fingerprint authenicated failed");
}