#include "Bank.h"
#include "Person.h"
#include "Account.h"
#include "Utils.h"

// Constructor with bank name and security fingerprint
Bank::Bank(const std::string& bank_name, const std::string& bank_fingerprint)
    :bank_name(bank_name),hashed_bank_fingerprint(std::hash<std::string>{}(bank_fingerprint)),
    bank_total_balance(0.0),bank_total_loan(0.0){
    
}

Account* Bank::create_account(Person& owner, const std::string& owner_fingerprint, std::string password) {
    Account * account = new Account(const_cast<Person*>(&owner),this,password);
    authenticate(owner_fingerprint,owner.get_hashed_fingerprint());
    bank_accounts.push_back(account);
    account_2_customer.insert({account,&owner});
    customer_2_accounts[&owner].push_back(account);
    return account;
}
bool Bank::delete_account(Account& account, const std::string& owner_fingerprint) {
    authenticate(owner_fingerprint,account.owner->get_hashed_fingerprint());
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
    authenticate(owner_fingerprint,account.owner->get_hashed_fingerprint());
    if(account.balance < amount) return false;
    account.balance -= amount;
    bank_total_balance += amount;
    return true;
}
bool Bank::withdraw(Account& account, const std::string& owner_fingerprint, double amount) {
    authenticate(owner_fingerprint,account.owner->get_hashed_fingerprint());
    if(bank_total_balance < amount) return false;
    bank_total_balance -= amount;
    account.balance += amount;
    return true;
}
bool Bank::transfer(Account& source, Account& destination, const std::string& owner_fingerprint,
    const std::string& CVV2, const std::string& password, const std::string& exp_date, double amount) {
    authenticate(owner_fingerprint,source.owner->get_hashed_fingerprint());
    authenticate(owner_fingerprint,destination.owner->get_hashed_fingerprint());
    if(source.get_CVV2(const_cast<std::string&>(owner_fingerprint)) == CVV2 && 
        source.get_password(const_cast<std::string&>(owner_fingerprint)) == password &&
        source.get_exp_date(const_cast<std::string&>(owner_fingerprint)) == exp_date)   {
        if(source.balance < amount) return false;
            source.balance -= amount;
            destination.balance += amount;
    } else {
        return false;
    }
    return true;
}
bool Bank::take_loan(Account& account, const std::string& owner_fingerprint, double amount) {
    authenticate(owner_fingerprint,account.owner->get_hashed_fingerprint());
    auto customer = account_2_customer[&account];
    auto accounts = customer_2_accounts[customer];
    double accounts_balance = 0.0;
    auto unload_amount = customer_2_unpaid_loan[customer];
    for(auto it : accounts) {
        accounts_balance += it->get_balance();
    }
    if(amount + unload_amount> accounts_balance*0.1*customer->get_socioeconomic_rank()) {
        return false;
    }
    if(amount < bank_total_loan) return false;
    bank_total_loan -= amount;
    account.balance += amount;
    customer_2_unpaid_loan[customer] += amount;
    return true;
}
bool Bank::pay_loan(Account& account, double amount) {
    //这里的利息计算就简单一点，所以可能有错，后面再改
    auto customer = account_2_customer[&account];
    auto unload_amount = customer_2_unpaid_loan[customer];
    auto interest = unload_amount * (10.0/customer->get_socioeconomic_rank());
    if(interest > amount || amount > account.balance)return false;
    amount -= interest;
    bank_total_balance += interest;
    customer_2_paid_loan[customer] += amount;
    customer_2_unpaid_loan[customer] -= amount;
    account.balance -= amount;
    if(customer_2_paid_loan[customer] >= pow(10,customer->get_socioeconomic_rank())) {
        customer->set_socioeconomic_rank(customer->get_socioeconomic_rank() + 1);
    }
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
    if(authenticate(bank_fingerprint,get_hashed_bank_fingerprint())) {
        return bank_customers;
    }
}

const std::vector<Account*>& Bank::get_bank_accounts(std::string& bank_fingerprint) const {
     if(authenticate(bank_fingerprint,get_hashed_bank_fingerprint())) {
        return bank_accounts;
    }
}

const std::map<Account*, Person*>& Bank::get_account_2_customer_map(std::string& bank_fingerprint) const {
     if(authenticate(bank_fingerprint,get_hashed_bank_fingerprint())) {
        return account_2_customer;
    }
}

const std::map<Person*, std::vector<Account*>>& Bank::get_customer_2_accounts_map(std::string& bank_fingerprint) const {
     if(authenticate(bank_fingerprint,get_hashed_bank_fingerprint())) {
        return customer_2_accounts;
    }
}

const std::map<Person*, double>& Bank::get_customer_2_paid_loan_map(std::string& bank_fingerprint) const {
    if(authenticate(bank_fingerprint,get_hashed_bank_fingerprint())) {
        return customer_2_paid_loan;
    }
}

const std::map<Person*, double>& Bank::get_customer_2_unpaid_loan_map(std::string& bank_fingerprint) const {
    if(authenticate(bank_fingerprint,get_hashed_bank_fingerprint())) {
        return customer_2_unpaid_loan;
    }
}

double Bank::get_bank_total_balance(std::string& bank_fingerprint) const {
  if(authenticate(bank_fingerprint,get_hashed_bank_fingerprint())) {
        return bank_total_balance;
    }
}

double Bank::get_bank_total_loan(std::string& bank_fingerprint) const {
  if(authenticate(bank_fingerprint,get_hashed_bank_fingerprint())) {
        return bank_total_loan;
    }
}

// Account Setters requiring owner and bank authentication
bool Bank::set_owner(Account& account, const Person* new_owner, std::string& owner_fingerprint, std::string& bank_fingerprint) {
    authenticate(owner_fingerprint,account.owner->get_hashed_fingerprint());
    authenticate(bank_fingerprint,get_hashed_bank_fingerprint());
    account.owner = const_cast<Person*>(new_owner);
    return true;
}

// Account Setters requiring bank authentication
bool Bank::set_account_status(Account& account, bool status, std::string& bank_fingerprint) {
    authenticate(bank_fingerprint,get_hashed_bank_fingerprint());
    account.account_status = status;
    return true;
}

bool Bank::set_exp_date(Account& account, std::string& exp_date, std::string& bank_fingerprint) {
    authenticate(bank_fingerprint,get_hashed_bank_fingerprint());
    account.exp_date = exp_date;
    return true;
}

// Outputs bank information, supports writing to file
void Bank::get_info(std::optional<std::string> file_name = std::nullopt) const {
     // Prepare the output
    std::ostringstream output_stream;

    // Output the bank name and fingerprint
    output_stream << "Bank Information:\n";
    output_stream << "Bank Name: " << bank_name << "\n";
    output_stream << "Bank Fingerprint (hashed): " << hashed_bank_fingerprint << "\n";
    output_stream << "Total Balance: " << bank_total_balance << "\n";
    output_stream << "Total Loans Issued: " << bank_total_loan << "\n";

    // Output the customers and accounts information
    output_stream << "\nCustomers:\n";
    for (const auto& customer : bank_customers) {
        output_stream << "  Customer: " << customer << "\n";
    }

    output_stream << "\nAccounts:\n";
    for (const auto& account : bank_accounts) {
        output_stream << "  Account: " << account << "\n";
    }

    // Output the loans information
    output_stream << "\nLoans Information:\n";
    output_stream << "Paid Loans:\n";
    for (const auto& loan : customer_2_paid_loan) {
        output_stream << "  Customer: " << loan.first << ", Paid: " << loan.second << "\n";
    }
    output_stream << "Unpaid Loans:\n";
    for (const auto& loan : customer_2_unpaid_loan) {
        output_stream << "  Customer: " << loan.first << ", Unpaid: " << loan.second << "\n";
    }

    // If a file name is provided, write the output to the file
    if (file_name) {
        std::ofstream file(*file_name);
        if (file.is_open()) {
            file << output_stream.str();
            file.close();
        } else {
            std::cerr << "Error opening file for writing: " << *file_name << "\n";
        }
    } else {
        // Otherwise, print the output to the console
        std::cout << output_stream.str();
    }
}