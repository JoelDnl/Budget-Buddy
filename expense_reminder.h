// expense_reminder.h
#ifndef EXPENSE_REMINDER_H
#define EXPENSE_REMINDER_H

#include <ctime>

class Reminder {
public:
    static std::tm addDays(const std::tm& timeinfo, int days);
    static void showPaymentReminder(int daysUntilPayment);
};

#endif // EXPENSE_REMINDER_H 
