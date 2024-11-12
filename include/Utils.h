#ifndef UTILS_H // Prevents double inclusion of this header
#define UTILS_H
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>
#include <random>

bool authenticate(const std::string& fingerprint,std::size_t hashed_fingerprint);

const std::string random_account();


#endif // UTILS_H