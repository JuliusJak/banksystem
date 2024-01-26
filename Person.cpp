#include "Person.h"

Person::Person(const std::string& username, const std::string& password)
    : username(username), password(password) {}

std::string Person::getUsername() const {
    return username;
}

std::string Person::getPassword() const {
    return password;
}

bool Person::checkPassword(const std::string& enteredPassword) const {
    return password == enteredPassword;
}