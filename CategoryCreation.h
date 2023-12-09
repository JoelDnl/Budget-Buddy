#ifndef HEADER_H
#define HEADER_H

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLabel.h>
#include <Wt/WComboBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTable.h>

class CategoryCreation : public Wt::WApplication
{
public:
    CategoryCreation(const Wt::WEnvironment &env);

private:
    Wt::WContainerWidget *categoryContainer;
    Wt::WComboBox *categoryComboBox;
    Wt::WLineEdit *expenseAmountInput;
    Wt::WPushButton *submitExpenseButton;
    Wt::WTable *expenseTable;
};

#endif // HEADER_H
