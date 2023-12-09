// User.h
#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    User(); // Add the default constructor
    User(const std::string& username, const std::string& password);
    double getBalance() const;
    void addIncome(double amount);
    void addExpense(double amount);
    bool checkPassword(const std::string& password) const;

private:
    std::string username;
    std::string password;
    double balance;  // Add the balance member variable
};

#endif // USER_H
