// UserManager.cpp

#include "UserManager.h"
#include "User.h"

UserManager::UserManager() {
    // You can initialize users with any existing users here
    users["john"] = User("john", "p1");
    users["bob"] = User("bob", "p2");
}

bool UserManager::registerUser(const std::string &username, const std::string &password) {
    // Check if the username already exists
    if (users.find(username) != users.end()) {
        return false; // User already exists
    }

    // Register the new user
    users[username] = User(username, password);
    return true;
}

bool UserManager::loginUser(const std::string &username, const std::string &password) {
    // Check if the username exists
    auto it = users.find(username);
    if (it == users.end()) {
        return false; // User does not exist
    }

    // Check if the password matches
    return it->second.checkPassword(password);
}

void UserManager::setCurrentUsername(const std::string &username) {
    currentUsername = username;
}

User& UserManager::getCurrentUser(){
    // Assume the user exists (you may want to handle the case when the user doesn't exist)
    return users[currentUsername];
}
