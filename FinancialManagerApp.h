// FinancialManagerApp.h

#ifndef FINANCIALMANAGERAPP_H
#define FINANCIALMANAGERAPP_H

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLabel.h>
#include <Wt/WComboBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTable.h>

#include "UserManager.h"
#include "User.h"
#include "currency_conversion.h"

class FinancialManagerApp : public Wt::WApplication
{
public:
    FinancialManagerApp(const Wt::WEnvironment &env);
    Wt::WComboBox *profileComboBox;

private:
    void setupApp();
    void setupAddIncomePage(Wt::WContainerWidget *parent);
    void setupCheckBalancePage(Wt::WContainerWidget *parent);
    void setupConvertCurrencyPage(Wt::WContainerWidget *parent);
    void setupCategoryCreationPage(Wt::WContainerWidget *parent);
    void setupExpenseReminderPage(Wt::WContainerWidget *parent);
    void setupBudgetPage(Wt::WContainerWidget *parent);
    void setupExpenseSummaryPage(Wt::WContainerWidget *parent);

    void copyTableRow(Wt::WTable *sourceTable, Wt::WTable *destTable, int rowIndex);
    Wt::WDate parseDateFromString(const std::string &dateString);

    Wt::WComboBox *createYearDropdown(Wt::WContainerWidget *parent);
    Wt::WComboBox *createMonthDropdown(Wt::WContainerWidget *parent);
    Wt::WComboBox *createDayDropdown(Wt::WContainerWidget *parent);
    Wt::WComboBox *createCategoryDropdown(Wt::WContainerWidget *parent);

    void setupLoginPage();

    UserManager userManager;
    User user;

    void updateBalance(double amount);
    void showBalance();
    void onProfileChange();
    Wt::WComboBox *createProfileDropdown(Wt::WContainerWidget *parent);
    double balance = 0.0;
    double checkingBalance = 0.0;
    double savingBalance = 0.0;
    double investmentBalance = 0.0;

    Wt::WLabel *savingBalanceDisplay;
    Wt::WLabel *investmentBalanceDisplay;

    Wt::WLabel *errorLabel;
    Wt::WContainerWidget *categoryContainer;
    Wt::WComboBox *yearComboBox;
    Wt::WComboBox *monthComboBox;
    Wt::WComboBox *dayComboBox;
    Wt::WLineEdit *reminderTitleInput;
    Wt::WPushButton *submitExpenseButton;
    Wt::WPushButton *deleteExpenseButton;
    Wt::WContainerWidget *reminderContainer;
    Wt::WTable *reminderTable;

    Wt::WLabel *errorLabel2;
    Wt::WContainerWidget *ExpenseContainer;
    Wt::WContainerWidget *checkBalanceContent;
    Wt::WContainerWidget *addIncomeContent;
    Wt::WContainerWidget *convertCurrencyContent;
    Wt::WLabel *checkingBalanceDisplay;
    Wt::WComboBox *categoryComboBox;
    Wt::WLineEdit *expenseAmountInput;
    Wt::WPushButton *submitExpenseSummaryButton;
    Wt::WTable *expenseTable;
    Wt::WTable *expenseSummaryTable;

    Wt::WLineEdit *customCategoryInput;
    Wt::WPushButton *addCustomCategoryButton;
    Wt::WPushButton *editCategoryButton;
    Wt::WPushButton *deleteCategoryButton;
};

#endif // FINANCIALMANAGERAPP_H
