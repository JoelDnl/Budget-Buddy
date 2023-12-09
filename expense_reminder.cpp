// expense_reminder.cpp
#include "expense_reminder.h"
#include <iostream>
#include <chrono>
#include <iomanip>

std::tm Reminder::addDays(const std::tm& timeinfo, int days) {
    std::tm result = timeinfo;
    std::time_t time = std::mktime(&result);
    time += days * 24 * 60 * 60;  // Convert days to seconds
    return *std::localtime(&time);
}

void Reminder::showPaymentReminder(int daysUntilPayment) {
    // Get the current date and time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* current_timeinfo = std::localtime(&now_time);

    // Set the upcoming payment date
    std::tm paymentDate = addDays(*current_timeinfo, daysUntilPayment);

    // Display the reminder
    std::cout << "Upcoming Payment Reminder:\n";
    std::cout << "Payment is due on: " << std::put_time(&paymentDate, "%Y-%m-%d") << std::endl;
}

