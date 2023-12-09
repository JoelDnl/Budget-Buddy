// currency_conversion.h
#ifndef CURRENCY_CONVERSION_H
#define CURRENCY_CONVERSION_H

#include <map>
#include <string>

class CurrencyConverter {
public:
    // Static member variable to store exchange rates
    static std::map<std::string, double> exchangeRates;

    // Conversion function
    static double convertCurrency(double amount, const std::string &currency);
};

#endif // CURRENCY_CONVERSION_H
