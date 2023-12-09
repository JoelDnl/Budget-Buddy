#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include "CategoryCreation.h"

CategoryCreation::CategoryCreation(const Wt::WEnvironment &env)
    : WApplication(env)
{
    setTitle("Budget Buddy - Category Creation");

    categoryContainer = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
    categoryContainer->addWidget(std::make_unique<Wt::WLabel>("Select Category: "));

    categoryComboBox = categoryContainer->addWidget(std::make_unique<Wt::WComboBox>());
    categoryComboBox->addItem("Housing");
    categoryComboBox->addItem("Utilities");
    categoryComboBox->addItem("Transportation");
    categoryComboBox->addItem("Groceries");
    categoryComboBox->addItem("Savings");
    categoryComboBox->addItem("Investments");
    categoryComboBox->addItem("Entertainment");
    categoryComboBox->addItem("Other");

    categoryContainer->addWidget(std::make_unique<Wt::WBreak>());

    categoryContainer->addWidget(std::make_unique<Wt::WLabel>("Enter Expense Amount: "));
    expenseAmountInput = categoryContainer->addWidget(std::make_unique<Wt::WLineEdit>());

    submitExpenseButton = categoryContainer->addWidget(std::make_unique<Wt::WPushButton>("Submit Expense"));

    expenseTable = categoryContainer->addWidget(std::make_unique<Wt::WTable>());
    expenseTable->setHeaderCount(1);
    expenseTable->elementAt(0, 0)->addWidget(std::make_unique<Wt::WText>("Category"));
    expenseTable->elementAt(0, 1)->addWidget(std::make_unique<Wt::WText>("Amount"));

    submitExpenseButton->clicked().connect([=]
                                           {
        const Wt::WString category = categoryComboBox->currentText();
        const Wt::WString amount = expenseAmountInput->valueText();
        if (!category.empty() && !amount.empty()) {
            // Add the expense to the SQLite database and update the table
            int row = expenseTable->rowCount();
            expenseTable->elementAt(row, 0)->addWidget(std::make_unique<Wt::WText>(category));
            expenseTable->elementAt(row, 1)->addWidget(std::make_unique<Wt::WText>(amount));
        } });
}

int main(int argc, char **argv)
{
    return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env)
                    { return std::make_unique<CategoryCreation>(env); });
}
