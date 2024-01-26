#include <iostream>
#include <fstream>
#include "Savings.h"
#include <iomanip>
#include <filesystem>

using std::string;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::fixed;
using std::setprecision;
namespace fs = std::filesystem;

Savings::Savings(LoginSystem& loginSystem) : loginSystem(loginSystem) {}

Savings::~Savings() {
}

void Savings::showSavingsAccounts() {
    string currentUsername = loginSystem.getCurrentUsername();
    string userDirectory = "users/userList/User_" + currentUsername;

    for (const auto& entry : fs::directory_iterator(userDirectory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            string filePath = entry.path().string();

            ifstream file(filePath);
            if (file.is_open()) {
                double balance;
                if (file >> balance) {
                    cout << "Balance for " << entry.path().stem() << ": " << balance << endl;
                } else {
                    cerr << "Error reading savings account balance from file: " << filePath << "\n";
                }
            } else {
                cerr << "Error opening savings account file: " << filePath << "\n";
            }

            file.close();
        }
    }
}

Savings* Savings::createSavingsAccount(const LoginSystem& loginSystem) {
    string currentUsername = loginSystem.getCurrentUsername();

    string userDirectory = "users/userList/User_" + currentUsername;

    string csvFileName;
    cout << "Enter the name of your savings account: ";
    cin >> csvFileName;

    string filePath = userDirectory + "/" + csvFileName + ".csv";

    ofstream file(filePath);
    if (file.is_open()) {
        file << "0.000000\n";
        cout << "Savings account CSV file created: " << filePath << endl;
    } else {
        cerr << "Error creating savings account CSV file." << endl;
    }
    file.close();

    return new Savings(const_cast<LoginSystem&>(loginSystem));
}

void Savings::addMoney(double amount, const string& recipientFilePath) {
    ifstream recipientFile(recipientFilePath);
    if (!recipientFile.is_open()) {
        cerr << "Error updating recipient's savings account.\n";
        return;
    }

    double currentBalance = 0.0;
    if (recipientFile >> currentBalance) {
        currentBalance += amount;

        ofstream recipientWriteFile(recipientFilePath);
        if (recipientWriteFile.is_open()) {
            recipientWriteFile << fixed << setprecision(6) << currentBalance << "\n";
            cout << "Added " << amount << " to the recipient's savings account. New balance: " << currentBalance << "\n";
        } else {
            cerr << "Error updating recipient's savings account.\n";
        }
        recipientWriteFile.close();
    } else {
        cerr << "Error updating recipient's savings account.\n";
    }

    recipientFile.close();
}

bool Savings::removeMoney(double amount, const string& savingsAccountName) {
    string currentUsername = loginSystem.getCurrentUsername();
    string userDirectory = "users/userList/User_" + currentUsername;
    string filePath = userDirectory + "/" + savingsAccountName + ".csv";

    ifstream readFile(filePath);
    if (!readFile.is_open()) {
        cerr << "Error updating savings account.\n";
        return false;
    }

    double currentBalance = 0.0;
    if (readFile >> currentBalance) {
        if (currentBalance >= amount) {
            currentBalance -= amount;

            ofstream writeFile(filePath);
            if (writeFile.is_open()) {
                writeFile << fixed << setprecision(6) << currentBalance << "\n";
                cout << "Removed " << amount << " from the savings account. New balance: " << currentBalance << "\n";
                return true;
            } else {
                cerr << "Error updating savings account.\n";
            }
            writeFile.close();
        } else {
            cout << "Insufficient funds. Cannot remove " << amount << " from the savings account.\n";
        }
    } else {
        cerr << "Error updating savings account.\n";
    }

    readFile.close();
    return false;
}

void Savings::transferMoney() {
    string currentUsername = loginSystem.getCurrentUsername();
    string userDirectory = "users/userList/User_" + currentUsername;

    string fromAccount, toAccount;
    double amountToBeTransferred;

    cout << "Take the money from: "; cin >> fromAccount;
    cout << "Give the money to: "; cin >> toAccount;
    cout << "Enter amount to be transferred: "; cin >> amountToBeTransferred;

    string recipientFilePath = userDirectory + "/" + toAccount + ".csv";
    if (removeMoney(amountToBeTransferred, fromAccount)) {
        addMoney(amountToBeTransferred, recipientFilePath);
    } else {
        cout << "Transfer canceled.\n";
    }
}

void Savings::deleteSavingsAccount() {
    string currentUsername = loginSystem.getCurrentUsername();
    string userDirectory = "users/userList/User_" + currentUsername;

    string savingsAccountName;
    cout << "Enter the name of the savings account to delete: ";
    cin >> savingsAccountName;

    string filePath = userDirectory + "/" + savingsAccountName + ".csv";

    if (fs::exists(filePath)) {
        char confirmation;
        cout << "Are you sure you want to delete the savings account '" << savingsAccountName << "'? (y/n): ";
        cin >> confirmation;

        if (confirmation == 'y' || confirmation == 'Y') {
            if (fs::remove(filePath)) {
                cout << "Savings account '" << savingsAccountName << "' deleted successfully.\n";
            } else {
                cerr << "Error deleting savings account '" << savingsAccountName << "'.\n";
            }
        } else {
            cout << "Deletion canceled. The savings account '" << savingsAccountName << "' was not deleted.\n";
        }
    } else {
        cerr << "Savings account '" << savingsAccountName << "' not found.\n";
    }
}

void Savings::payment() {
    string currentUsername = loginSystem.getCurrentUsername();
    string currentUserDirectory = "users/userList/User_" + currentUsername;

    string fromAccount, toUsername, toAccount;
    double amountToBePaid;

    cout << "Enter the name of the account to deduct from: ";
    cin >> fromAccount;

    // Ensure that the source account exists
    string sourceFilePath = currentUserDirectory + "/" + fromAccount + ".csv";
    if (!fs::exists(sourceFilePath)) {
        cerr << "Source account '" << fromAccount << "' not found.\n";
        return;
    }

    cout << "Enter the username of the recipient: ";
    cin >> toUsername;

    string toUserDirectory = "users/userList/User_" + toUsername;

    // Ensure that the recipient account exists
    cout << "Enter the name of the account to transfer to: ";
    cin >> toAccount;

    string recipientFilePath = toUserDirectory + "/" + toAccount + ".csv";
    if (!fs::exists(recipientFilePath)) {
        cerr << "Recipient account '" << toAccount << "' not found.\n";
        return;
    }

    cout << "Enter amount to be paid: ";
    cin >> amountToBePaid;

    // Use removeMoney on the source account
    if (removeMoney(amountToBePaid, fromAccount)) {
        // Use addMoney on the recipient's account
    Savings recipientSavings(loginSystem);  // Use the existing user's loginSystem
    recipientSavings.recipientUserDirectory = toUserDirectory;  // Set recipient's directory
    recipientSavings.addMoney(amountToBePaid, recipientFilePath);
    cout << "Payment of " << amountToBePaid << " made successfully to account '" << toAccount << "' of user '" << toUsername << "'.\n";
    } else {
        cout << "Payment canceled.\n";
    }
}
