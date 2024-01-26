#include "Menu.h"
#include "Savings.h"
#include <iostream>
#include <stdexcept>
#include <limits> 

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::invalid_argument;
using std::out_of_range;
using std::numeric_limits;

Menu::Menu(LoginSystem& loginSystem) : loginSystem(loginSystem), savings(loginSystem) {}

void Menu::showOptions() {
    string choiceStr;

    while (true) {
        cout << "\nOptions:\n";
        cout << "1. Show savings\n";
        cout << "2. Make payment\n";
        cout << "3. Delete account\n";
        cout << "4. Log out\n";
        cout << "Enter your choice: ";
        cin >> choiceStr;

        try {
            int choice = stoi(choiceStr);

            switch (choice) {
                case 1:
                    showSavingsOptions();
                    break;
                case 2:
                    cout << "Making payment...\n";
                    break;
                case 3:
                    cout << "Deleting account...\n";
                    loginSystem.deleteAccount();
                    return;
                case 4:
                    cout << "Logging out...\n";
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
                    break;
            }
        } catch (const invalid_argument& e) {
            cerr << "Invalid input. Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        } catch (const out_of_range& e) {
            cerr << "Input out of range for integer. Please enter a smaller integer.\n";
        }
    }
}

void Menu::showSavingsOptions() {
    string savingsChoiceStr;

    while (true) {
        cout << "\nSavings Options:\n";
        cout << "1. Create new savings account\n";
        cout << "2. Delete savings account\n";
        cout << "3. Show savings accounts\n";
        cout << "4. Transfer money\n";
        cout << "5. Back to main menu\n";
        cout << "Enter your choice: ";
        cin >> savingsChoiceStr;

        try {
            int savingsChoice = stoi(savingsChoiceStr);

            switch (savingsChoice) {
                case 1:
                    savings.createSavingsAccount(loginSystem);
                    break;
                case 2:
                    savings.deleteSavingsAccount();
                    break;
                case 3:
                    cout << "\nShowing savings accounts:\n";
                    savings.showSavingsAccounts();
                    break;
                case 4:
                    savings.transferMoney();
                    break;                
                case 5:
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
                    break;
            }
        } catch (const invalid_argument& e) {
            cerr << "Invalid input. Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        } catch (const out_of_range& e) {
            cerr << "Input out of range for integer. Please enter a smaller integer.\n";
        }
    }
}

void Menu::showMainMenu() {
    string choice;
        
    while (choice != "0") {
        cout << "1. Log in\n2. Create a new user\n3. Shut off\nEnter your choice: ";
        cin >> choice;

        if (choice == "1") {
            string enteredUsername, enteredPassword;
            cout << "Enter username: ";
            cin >> enteredUsername;
            cout << "Enter password: ";
            cin >> enteredPassword;

            if (loginSystem.loginUser(enteredUsername, enteredPassword)) {
                showOptions();
            }
        } else if (choice == "2") {
            if (loginSystem.registerUser()) {
                showOptions();
            }
        } else if (choice == "3")
        {
            cout << "Shutting off...";
            return;
        }
        
         else {
            cout << "Invalid choice." << endl;
        }
    }
}
