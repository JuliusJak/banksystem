#pragma once

#include <string>

class Person {
public:
    Person(const std::string& username, const std::string& password);
    std::string getUsername() const;
    std::string getPassword() const;
    bool checkPassword(const std::string& enteredPassword) const;

private:
    std::string username;
    std::string password;
};