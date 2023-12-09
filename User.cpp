// User.cpp

#include "User.h"

User::User() : balance(0.0) {} // Implement the default constructor

User::User(const std::string& username, const std::string& password)
    : username(username), password(password), balance(0.0) {}

double User::getBalance() const {
    return balance;
}

void User::addIncome(double amount) {
    balance += amount;
}

void User::addExpense(double amount) {
    // You might want to add additional logic here (e.g., check if the expense is valid)
    balance -= amount;
}

bool User::checkPassword(const std::string& password) const {
    return this->password == password;
}
