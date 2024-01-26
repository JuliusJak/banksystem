#pragma once

#include <string>
#include <vector>
#include "Person.h"

class LoginSystem {
public:
    LoginSystem(const std::string& filePath);
    void readUsersFromFile(const std::string& filePath);
    void writeUserToFile(const Person& newUser, const std::string& filePath);
    bool loginUser(const std::string& enteredUsername, const std::string& enteredPassword);
    bool registerUser();
    void deleteAccount();

    std::string getCurrentUsername() const;

private:
    std::vector<Person> users;
    std::string currentUsername;
};