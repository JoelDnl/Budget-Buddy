// currency_conversion.cpp
#include "currency_conversion.h"
#include <map>
#include <string>
#include <iostream>


// Initialize the static member variable
std::map<std::string, double> CurrencyConverter::exchangeRates = {
    {"USD", 1.37},  // 1 CAD to USD
    {"EUR", 1.46},  // 1 CAD to EUR
    {"GBP", 1.69}   // 1 CAD to GBP
};

// Implement the conversion function
double CurrencyConverter::convertCurrency(double amount, const std::string &currency) {
    double balance = amount;
    double convertedAmount = 0.0; // Declare convertedAmount here and initialize it.

    if (currency != "CAD") {
        auto it = exchangeRates.find(currency);
        if (it != exchangeRates.end()) {
            double exchangeRate = it->second;
            convertedAmount = balance * exchangeRate;
        } else {
            // Handle invalid currency
            std::cout << "Unsupported currency pair or invalid input." << std::endl;
        }
    } else {
        // Handle case when the currency is CAD
        std::cout << "Currency is already CAD." << std::endl;
    }

    return convertedAmount;
}
