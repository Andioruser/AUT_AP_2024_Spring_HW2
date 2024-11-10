#include "Account.h"
#include "Bank.h"
#include "Person.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <random>
#include <fstream>

Account::Account(const Person *const owner, const Bank *const bank, std::string &password)
    : owner(const_cast<Person *>(owner)), bank(bank), password(password), account_number(random_account()), balance(false), account_status(false)
{
}

// Getters
const Person *Account::get_owner() const
{
    return owner;
}
double Account::get_balance() const
{
    return balance;
}
std::string Account::get_account_number() const
{
    return account_number;
}
bool Account::get_status() const
{
    return account_status;
}

std::string Account::get_CVV2(std::string &owner_fingerprint) const
{
    if (authenticate(owner_fingerprint))
    {
        return CVV2;
    }
}

std::string Account::get_password(std::string &owner_fingerprint) const
{
    if (authenticate(owner_fingerprint))
    {
        return password;
    }
}

std::string Account::get_exp_date(std::string &owner_fingerprint) const
{
    if (authenticate(owner_fingerprint))
    {
        return exp_date;
    }
}

bool Account::authenticate(const std::string &owner_fingerprint) const
{
    if (owner->get_hashed_fingerprint() == std::hash<std::string>{}(owner_fingerprint))
        return true;
    // 抛出的异常类型需要再想想
    throw std::invalid_argument("The fingerprint authenicated failed");
}

const std::string Account::random_account()
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
bool Account::set_password(std::string &password, std::string &owner_fingerprint)
{
    if (authenticate(owner_fingerprint))
    {
        password = password;
    }
    return true;
}

// Spaceship operator for Account comparison
std::strong_ordering Account::operator<=>(const Account &other) const
{
    return account_number <=> other.account_number;
}

// Outputs account information, supports writing to file
void Account::get_info(std::optional<std::string> file_name = std::nullopt) const
{
    std::string info = "Account Number: " + account_number + "\n" +
                       "Balance: $" + std::to_string(balance) + "\n" +
                       "Account Status: " + (account_status ? "Active" : "Inactive") + "\n" +
                       "Expiration Date: " + exp_date + "\n";
    if (file_name)
    {
        std::ofstream file(*file_name);
        file << info;
    }
    else
    {
        std::cout << info << std::endl;
    }
}