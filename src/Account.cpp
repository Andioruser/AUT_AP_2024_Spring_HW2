#include "Account.h"
#include "Bank.h"
#include "Person.h"
#include "Utils.h"


Account::Account(const Person* const owner, const Bank* const bank, std::string& password)
                :owner(const_cast<Person*>(owner)),bank(bank),password(password){}

// Getters
const Person* Account::get_owner() const {
    return owner;
}
double Account::get_balance() const {
    return balance;
}
std::string Account::get_account_number() const {
    return account_number;
}
bool Account::get_status() const {
    return account_status;
}

std::string Account::get_CVV2(std::string& owner_fingerprint) const {
    
}
std::string Account::get_password(std::string& owner_fingerprint) const {

}
std::string Account::get_exp_date(std::string& owner_fingerprint) const {

}