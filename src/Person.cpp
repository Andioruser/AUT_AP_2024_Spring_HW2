#include "Person.h"
#include "Utils.h"

Person::Person(std::string &name, std::size_t age, std::string &gender,
        std::string &fingerprint, size_t socioeconomic_rank, bool is_alive)
        :name(name),age(age),gender(gender),hashed_fingerprint(std::hash<std::string>{}(fingerprint)),
        socioeconomic_rank(socioeconomic_rank),is_alive(is_alive) {
    if (gender != "Female" && gender != "Male") {
        throw std::invalid_argument("Invalid gender. Only 'Female' or 'Male' are allowed.");
    }
}

// Getters
std::string Person::get_name() const {
    return name;
}

std::size_t Person::get_age() const {
    return age;
}

std::string  Person::get_gender() const {
    return gender;
}

std::size_t  Person::get_hashed_fingerprint() const {
    return hashed_fingerprint;
}

std::size_t  Person::get_socioeconomic_rank() const {
    return socioeconomic_rank;
}

bool Person::get_is_alive() const {
    return is_alive;
}

// Setters
bool Person::set_age(std::size_t age) {
    age = age;
    return true;
}

bool Person::set_socioeconomic_rank(std::size_t rank) {
    if(rank < 0 || rank > 10) {
        throw std::invalid_argument("Invalid rank. Only 1 ~ 10 number are allowed.");
    }
    socioeconomic_rank = rank;
    return true;
}

bool Person::set_is_alive(bool is_alive) {
    is_alive = is_alive;
    return true;
}

void Person::get_info(std::optional<std::string> file_name = std::nullopt) const {
    std::string info = "Name: " + name + "\nAge: " + std::to_string(age) +
                        "\nGender: " + gender + "\nFingerprint Hash: " + std::to_string(hashed_fingerprint) +
                        "\nSocioeconomic Rank: " + std::to_string(socioeconomic_rank) +
                        "\nAlive: " + (is_alive ? "Yes" : "No");
    std::cout<<info<<std::endl;
    if (file_name) {
        std::ofstream file(*file_name);
        file << info;
    } else {
        std::cout<<"No valid filename exists here";
    }
}

std::strong_ordering Person::operator<=>(const Person &other) const {
    return hashed_fingerprint <=> other.hashed_fingerprint;
}