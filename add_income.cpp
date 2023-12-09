#include "add_income.h"
#include <iostream>

/**
 * @brief Add income to the balance.
 *
 * This function prompts the user to enter an income amount (CAD) and adds it to the current balance.
 *
 * @param balance The current account balance.
 * @return The updated account balance after adding income.
 */
double addIncome(double balance) {
    double income;
    std::cout << "Enter your income amount: $";
    std::cin >> income;
    balance += income;
    std::cout << "Current balance: $" << balance << "\n";
    return balance;
}

/**
 * @brief Display the current account balance.
 *
 * This function displays the current account balance.
 *
 * @param balance The current account balance.
 * @return The current account balance.
 */
double checkBalance(double balance) {
    std::cout << "Current balance: $" << balance << "\n";
    return balance;
}
