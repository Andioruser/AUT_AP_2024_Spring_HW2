#include "Account.h"
#include "Bank.h"
#include "Person.h"
#include "Utils.h"

Account::Account(const Person *const owner, const Bank *const bank, std::string &password)
    :owner(const_cast<Person *>(owner)), bank(bank), password(password), account_number(random_account()), balance(false), account_status(false)
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
    if (authenticate(owner_fingerprint,owner->get_hashed_fingerprint()))
    {
        return CVV2;
    }
}

std::string Account::get_password(std::string &owner_fingerprint) const
{
    if (authenticate(owner_fingerprint,owner->get_hashed_fingerprint()))
    {
        return password;
    }
}

std::string Account::get_exp_date(std::string &owner_fingerprint) const
{
    if (authenticate(owner_fingerprint,owner->get_hashed_fingerprint()))
    {
        return exp_date;
    }
}

bool Account::set_password(std::string &password, std::string &owner_fingerprint)
{
    if (authenticate(owner_fingerprint,owner->get_hashed_fingerprint()))
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