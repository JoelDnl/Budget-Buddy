// UserManager.h

#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include <unordered_map>
#include "User.h"

class UserManager {
public:
    UserManager();

    bool registerUser(const std::string &username, const std::string &password);
    bool loginUser(const std::string &username, const std::string &password);

    void setCurrentUsername(const std::string &username);
    User& getCurrentUser();

private:
    std::unordered_map<std::string, User> users;
    std::string currentUsername;
};

#endif // USERMANAGER_H
