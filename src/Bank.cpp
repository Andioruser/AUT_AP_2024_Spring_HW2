#include "Bank.h"
#include "Person.h"
#include "Account.h"
#include "Utils.h"
#include <iostream>

// Constructor with bank name and security fingerprint
Bank::Bank(const std::string& bank_name, const std::string& bank_fingerprint)
    :bank_name(bank_name),hashed_bank_fingerprint(std::hash<std::string>{}(bank_fingerprint)),
    bank_total_balance(0.0),bank_total_loan(0.0){
    
}

Account* Bank::create_account(Person& owner, const std::string& owner_fingerprint, std::string password) {
    Account * account = new Account(const_cast<Person*>(&owner),this,password);
    account->authenticate(owner_fingerprint);
    bank_accounts.push_back(account);
    account_2_customer.insert({account,&owner});
    customer_2_accounts[&owner].push_back(account);
    return account;
}
bool Bank::delete_account(Account& account, const std::string& owner_fingerprint) {
    account.authenticate(owner_fingerprint);
    if(account_2_customer.count(&account)) {
        auto owner_account = customer_2_accounts[account_2_customer[&account]];
        auto it = std::find(bank_accounts.begin(),bank_accounts.end(),&account);
        if(it != bank_accounts.end()) {
            bank_accounts.erase(it);
            return true;
        }
        else {
            return false;
        }  
        account_2_customer.erase(&account);
    }
    auto it = std::find(bank_accounts.begin(),bank_accounts.end(),&account);
    if(it != bank_accounts.end()) {
        bank_accounts.erase(it);
        return true;
    }
    return false;

}
bool Bank::delete_customer(Person& owner, const std::string& owner_fingerprint) {
    if(owner.get_hashed_fingerprint() != std::hash<std::string>{}(owner_fingerprint)) {
        throw std::invalid_argument("The fingerprint authenicated failed");
    }
    if(customer_2_accounts.count(&owner)) {
        auto accounts = customer_2_accounts[&owner];
        for(auto account: accounts) {
            if(account_2_customer.count(account))
                account_2_customer.erase(account);
        }
    }
    auto it = std::find(bank_customers.begin(),bank_customers.end(),&owner);
    if(it != bank_customers.end()) {
        bank_customers.erase(it);
        return true;
    }
    return false;
}

bool Bank::deposit(Account& account, const std::string& owner_fingerprint, double amount) {

}
bool Bank::withdraw(Account& account, const std::string& owner_fingerprint, double amount) {

}
bool Bank::transfer(Account& source, Account& destination, const std::string& owner_fingerprint,
    const std::string& CVV2, const std::string& password, const std::string& exp_date, double amount) {
    
}
bool Bank::take_loan(Account& account, const std::string& owner_fingerprint, double amount) {

}
bool Bank::pay_loan(Account& account, double amount) {

}

// Getters
const std::string& Bank::get_bank_name() const {
    return bank_name;
}

size_t Bank::get_hashed_bank_fingerprint() const {
    return hashed_bank_fingerprint;
}

// Getters requiring bank authentication
const std::vector<Person*>& Bank::get_bank_customers(std::string& bank_fingerprint) const {
    if(authenticate(bank_fingerprint)) {
        return bank_customers;
    }
}

const std::vector<Account*>& Bank::get_bank_accounts(std::string& bank_fingerprint) const {
    if(authenticate(bank_fingerprint)) {
        return bank_accounts;
    }
}

const std::map<Account*, Person*>& Bank::get_account_2_customer_map(std::string& bank_fingerprint) const {
    if(authenticate(bank_fingerprint)) {
        return account_2_customer;
    }
}

const std::map<Person*, std::vector<Account*>>& Bank::get_customer_2_accounts_map(std::string& bank_fingerprint) const {
    if(authenticate(bank_fingerprint)) {
        return customer_2_accounts;
    }
}

const std::map<Person*, double>& Bank::get_customer_2_paid_loan_map(std::string& bank_fingerprint) const {
    if(authenticate(bank_fingerprint)) {
        return customer_2_paid_loan;
    }
}

const std::map<Person*, double>& Bank::get_customer_2_unpaid_loan_map(std::string& bank_fingerprint) const {
 if(authenticate(bank_fingerprint)) {
        return customer_2_unpaid_loan;
    }
}

double Bank::get_bank_total_balance(std::string& bank_fingerprint) const {
 if(authenticate(bank_fingerprint)) {
        return bank_total_balance;
    }
}

double Bank::get_bank_total_loan(std::string& bank_fingerprint) const {
 if(authenticate(bank_fingerprint)) {
        return bank_total_loan;
    }
}

bool Bank::authenticate(const std::string& bank_fingerprint) const {
    if (get_hashed_bank_fingerprint() == std::hash<std::string>{}(bank_fingerprint))
        return true;
    // 抛出的异常类型需要再想想
    throw std::invalid_argument("The fingerprint authenicated failed");
}

// Account Setters requiring owner and bank authentication
bool Bank::set_owner(Account& account, const Person* new_owner, std::string& owner_fingerprint, std::string& bank_fingerprint) {

}

// Account Setters requiring bank authentication
bool Bank::set_account_status(Account& account, bool status, std::string& bank_fingerprint) {

}

bool Bank::set_exp_date(Account& account, std::string& exp_date, std::string& bank_fingerprint) {

}

// Outputs bank information, supports writing to file
void Bank::get_info(std::optional<std::string> file_name = std::nullopt) const {
    
}