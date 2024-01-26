#include "LoginSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

using std::string;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::vector;
using std::remove_if;
namespace fs = std::filesystem;

LoginSystem::LoginSystem(const string& filePath) {
    readUsersFromFile(filePath);
}

void LoginSystem::readUsersFromFile(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string username, password;
        if (getline(iss, username, ',') && getline(iss, password)) {
            users.push_back(Person(username, password));
        }
    }

    file.close();
}

void LoginSystem::writeUserToFile(const Person& newUser, const string& filePath) {
    ofstream file(filePath, std::ios::app);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    file << newUser.getUsername() << "," << newUser.getPassword() << "\n";

    file.close();
}

string LoginSystem::getCurrentUsername() const {
    return currentUsername;
}

bool LoginSystem::loginUser(const string& enteredUsername, const string& enteredPassword) {
    for (const auto& user : users) {
        if (user.getUsername() == enteredUsername && user.checkPassword(enteredPassword)) {
            currentUsername = enteredUsername;
            cout << "Login successful! Welcome, " << enteredUsername << "!" << endl;
            return true;
        }
    }
    cout << "Login failed. Invalid username or password." << endl;
    return false;
}

bool LoginSystem::registerUser() {
    string newUsername, newPassword;

    cout << "Enter a new username: ";
    cin >> newUsername;

    for (const auto& user : users) {
        if (user.getUsername() == newUsername) {
            cout << "Username already exists. Please choose a different username." << endl;
            return false;
        }
    }

    cout << "Enter a new password: ";
    cin >> newPassword;

    Person newUser(newUsername, newPassword);

    users.push_back(newUser);
    writeUserToFile(newUser, "users/userList.csv");
    currentUsername = newUsername;

    string userDirectory = "users/userList/User_" + newUsername;
    if (fs::create_directory(userDirectory)) {
        cout << "User directory created: " << userDirectory << endl;
    } else {
        cerr << "Error creating user directory: " << userDirectory << endl;
    }

    cout << "Registration successful! You are now logged in." << endl;

    return true;
}

void LoginSystem::deleteAccount() {
    string currentUsername = getCurrentUsername();
    string userDirectory = "users/userList/User_" + currentUsername;
    string filePath = "users/userList.csv";

    char confirmation;
    cout << "Are you sure you want to delete your account? (y/n): ";
    cin >> confirmation;

    if (confirmation == 'y' || confirmation == 'Y') {
        if (fs::remove_all(userDirectory)) {
            cout << "User directory deleted: " << userDirectory << endl;
        } else {
            cerr << "Error deleting user directory: " << userDirectory << endl;
        }

        users.erase(remove_if(users.begin(), users.end(),
            [currentUsername](const Person& user) {
                return user.getUsername() == currentUsername;
            }), users.end());

        ofstream file(filePath);
        if (file.is_open()) {
            for (const auto& user : users) {
                file << user.getUsername() << "," << user.getPassword() << "\n";
            }
            cout << "Account deleted successfully.\n";
        } else {
            cerr << "Error updating user list.\n";
        }

        currentUsername = "";
    } else {
        cout << "Account deletion canceled. Your account was not deleted.\n";
    }
}