#include "Menu.h"

int main() {
    std::string filePath = "users/userList.csv";
    LoginSystem loginSystem(filePath);
    Menu menu(loginSystem);

    menu.showMainMenu();

    return 0;
}
