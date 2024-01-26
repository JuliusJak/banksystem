#pragma once

#include "LoginSystem.h"

class Savings {
public:
    Savings(LoginSystem& loginSystem);
    ~Savings();

    void showSavingsAccounts();
    static Savings* createSavingsAccount(const LoginSystem& loginSystem);
    void addMoney(double amount, const std::string& recipientFilePath);
    bool removeMoney(double amount, const std::string& savingsAccountName);
    void transferMoney();
    void deleteSavingsAccount();
    void payment();

private:
    LoginSystem& loginSystem;
    std::string recipientUserDirectory;
};