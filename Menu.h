#pragma once

#include "LoginSystem.h"
#include "Savings.h"

class Menu {
public:
    Menu(LoginSystem& loginSystem);

    void showMainMenu();

private:
    LoginSystem& loginSystem;
    Savings savings;

    void showOptions();
    void showSavingsOptions();
};