// FinancialManagerApp.cpp

#include "FinancialManagerApp.h"
#include "UserManager.h"
#include "User.h"
#include "currency_conversion.h"
#include "expense_reminder.h"

// Add any other necessary includes for your specific User and UserManager implementations

#include <Wt/WBreak.h>
#include <Wt/WComboBox.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WDate.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WTable.h>
#include <Wt/WText.h>
#include <Wt/WString.h>
#include <Wt/WMessageBox.h>

#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_set>

FinancialManagerApp::FinancialManagerApp(const Wt::WEnvironment &env) : Wt::WApplication(env)
{
	setTitle("Budget Buddy");
	useStyleSheet("styles.css");
	setupLoginPage();
}

void FinancialManagerApp::setupLoginPage()
{
	root()->clear(); // Clear existing content
	root()->setStyleClass("body");

	Wt::WText *titleLabel = root()->addWidget(std::make_unique<Wt::WText>("Budget Buddy"));
	titleLabel->setStyleClass("big-title");

	Wt::WContainerWidget *loginPage = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
	loginPage->addWidget(std::make_unique<Wt::WBreak>());
	// Add labels for username and password
	loginPage->addWidget(std::make_unique<Wt::WText>("Username:"));
	Wt::WLineEdit *usernameInput = loginPage->addWidget(std::make_unique<Wt::WLineEdit>());

	loginPage->addWidget(std::make_unique<Wt::WBreak>()); // Add a line break between username and password

	loginPage->addWidget(std::make_unique<Wt::WText>("Password:"));
	Wt::WLineEdit *passwordInput = loginPage->addWidget(std::make_unique<Wt::WLineEdit>());
	passwordInput->setEchoMode(Wt::EchoMode::Password);

	loginPage->addWidget(std::make_unique<Wt::WBreak>());
	Wt::WPushButton *loginButton = loginPage->addWidget(std::make_unique<Wt::WPushButton>("Login"));
	loginButton->setStyleClass("custom-button");
	Wt::WPushButton *createAccountButton = loginPage->addWidget(std::make_unique<Wt::WPushButton>("Create Account"));
	createAccountButton->setStyleClass("custom-button");

	Wt::WText *errorLabel = nullptr;

	// Connect the login button click event
	loginButton->clicked().connect([=, &errorLabel, &createAccountButton]() mutable
								   {
		// Check if errorLabel already exists, remove it if it does
		if (errorLabel)
		{
			loginPage->removeWidget(errorLabel);
			errorLabel = nullptr; // Set errorLabel to nullptr after removing
		}

		// Check login credentials
		if (userManager.loginUser(usernameInput->text().toUTF8(), passwordInput->text().toUTF8()))
		{
			// Successful login
			userManager.setCurrentUsername(usernameInput->text().toUTF8());

			// Initialize 'user' here (replace the comment with actual initialization)
			user = userManager.getCurrentUser();

			// Switch to main content
			setupApp();
		}
		else
		{
			// Failed login, show an error message (you can customize this)
			errorLabel = loginPage->addWidget(std::make_unique<Wt::WText>("Invalid credentials. Please try again."));
			errorLabel->setStyleClass("error-message");
		} });

	// Connect the create account button click event
	createAccountButton->clicked().connect([=]() mutable
										   {
        // Check if the username or password is empty
        if (usernameInput->text().empty() || passwordInput->text().empty())
        {
            errorLabel = loginPage->addWidget(std::make_unique<Wt::WText>("Please enter a username and password."));
            errorLabel->setStyleClass("error-message");
            return;
        }

        // Attempt to register the new user
        if (userManager.registerUser(usernameInput->text().toUTF8(), passwordInput->text().toUTF8()))
        {
            // Successful registration
            errorLabel = loginPage->addWidget(std::make_unique<Wt::WText>("Account created successfully. You can now log in."));
            errorLabel->setStyleClass("success-message");
        }
        else
        {
            // User already exists
            errorLabel = loginPage->addWidget(std::make_unique<Wt::WText>("Username already exists. Please choose a different username."));
            errorLabel->setStyleClass("error-message");
        } });
}

Wt::WComboBox *FinancialManagerApp::createYearDropdown(Wt::WContainerWidget *parent)
{
	static const std::vector<std::string> years = {
		"2023", "2022", "2021", "2020", "2019", "2018",
		"2017", "2016", "2015", "2014", "2013"};

	auto yearComboBox = parent->addWidget(std::make_unique<Wt::WComboBox>());
	yearComboBox->setStyleClass("custom-combobox");
	for (const auto &year : years)
	{
		yearComboBox->addItem(year);
	}

	return yearComboBox;
}

Wt::WComboBox *FinancialManagerApp::createMonthDropdown(Wt::WContainerWidget *parent)
{
	static const std::vector<std::string> months = {
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"};

	auto monthComboBox = parent->addWidget(std::make_unique<Wt::WComboBox>());
	monthComboBox->setStyleClass("custom-combobox");
	for (const auto &month : months)
	{
		monthComboBox->addItem(month);
	}

	return monthComboBox;
}

Wt::WComboBox *FinancialManagerApp::createDayDropdown(Wt::WContainerWidget *parent)
{
	auto dayComboBox = parent->addWidget(std::make_unique<Wt::WComboBox>());
	dayComboBox->setStyleClass("custom-combobox");
	for (int day = 1; day <= 31; ++day)
	{
		dayComboBox->addItem(std::to_string(day));
	}

	return dayComboBox;
}

Wt::WComboBox *FinancialManagerApp::createCategoryDropdown(Wt::WContainerWidget *parent)
{
	static const std::vector<std::string> categories = {
		"Housing", "Utilities", "Transportation", "Groceries", "Savings", "Investments",
		"Entertainment", "Other"};

	auto categoryComboBox = parent->addWidget(std::make_unique<Wt::WComboBox>());
	categoryComboBox->setStyleClass("custom-combobox");
	for (const auto &category : categories)
	{
		categoryComboBox->addItem(category);
	}

	return categoryComboBox;
}

Wt::WComboBox *FinancialManagerApp::createProfileDropdown(Wt::WContainerWidget *parent)
{
	static const std::vector<std::string> profiles = {
		"Checking", "Savings", "Investments"};

	auto profileComboBox = parent->addWidget(std::make_unique<Wt::WComboBox>());
	profileComboBox->setStyleClass("custom-combobox");
	for (const auto &profile : profiles)
	{
		profileComboBox->addItem(profile);
	}

	return profileComboBox;
}

void FinancialManagerApp::updateBalance(double amount)
{
	const Wt::WString profileText = profileComboBox->currentText();
	std::string profile = profileText.toUTF8();

	balance += amount;

	if (profile == "Checking" || profile.empty())
	{
		checkingBalance += amount;
	}
	else if (profile == "Savings")
	{
		savingBalance += amount;
	}
	else
	{
		investmentBalance += amount;
	}

	showBalance();
}

void FinancialManagerApp::onProfileChange()
{
	const Wt::WString profileText = profileComboBox->currentText();
	std::string profile = profileText.toUTF8();

	if (profile == "Checking")
	{
		balance = checkingBalance;
	}
	else if (profile == "Savings")
	{
		balance = savingBalance;
	}
	else
	{
		balance = investmentBalance;
	}
}

void FinancialManagerApp::showBalance()
{
	std::ostringstream checkingBalanceText;
	checkingBalanceText << "Checking Balance: $" << std::fixed << std::setprecision(2) << checkingBalance;
	checkingBalanceDisplay->setText(checkingBalanceText.str());

	std::ostringstream savingBalanceText;
	savingBalanceText << "Savings Balance: $" << std::fixed << std::setprecision(2) << savingBalance;
	savingBalanceDisplay->setText(savingBalanceText.str());

	std::ostringstream investmentBalanceText;
	investmentBalanceText << "Investment Balance: $" << std::fixed << std::setprecision(2) << investmentBalance;
	investmentBalanceDisplay->setText(investmentBalanceText.str());
}

/*
 * setupApp sets up each section with containers.
 */

void FinancialManagerApp::setupApp()
{
	root()->clear();
	root()->setStyleClass("body");

	Wt::WText *titleLabel = root()->addWidget(std::make_unique<Wt::WText>("Budget Buddy"));
	titleLabel->setStyleClass("big-title");

	Wt::WContainerWidget *mainContent = root()->addWidget(std::make_unique<Wt::WContainerWidget>());

	mainContent->addWidget(std::make_unique<Wt::WBreak>());

	Wt::WPushButton *logoutButton = mainContent->addWidget(std::make_unique<Wt::WPushButton>("Logout"));
	logoutButton->setStyleClass("custom-button");

	// Connect the logout button click event
	logoutButton->clicked().connect([=]
									{
        // Return to the login page
        userManager.setCurrentUsername(""); // Clear current username on logout
        setupLoginPage(); });

	// Add Income Page
	setupAddIncomePage(mainContent);

	// Check Balance Page
	setupCheckBalancePage(mainContent);

	// Convert Currency Page
	setupConvertCurrencyPage(mainContent);

	// Category Creation Page
	setupCategoryCreationPage(mainContent);

	// Reminder Page
	setupExpenseReminderPage(mainContent);

	// Budget Page
	setupBudgetPage(mainContent);

	// Expense Summary Page
	setupExpenseSummaryPage(mainContent);
}

void FinancialManagerApp::setupCategoryCreationPage(Wt::WContainerWidget *parent)
{

	/**
	 * Creates the layout and the dropdown box for the categories and dates.
	 */

	Wt::WContainerWidget *categoryContainer = root()->addWidget(std::make_unique<Wt::WContainerWidget>());

	Wt::WContainerWidget *titleContainer = categoryContainer->addWidget(std::make_unique<Wt::WContainerWidget>());
	titleContainer->addWidget(std::make_unique<Wt::WLabel>("Expenses"))->setStyleClass("title");
	categoryContainer->addWidget(std::make_unique<Wt::WBreak>());

	// addIncomeContent = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
	// addIncomeContent->addWidget(std::make_unique<Wt::WLabel>("Income"));
	// addIncomeContent->setStyleClass("title");
	// addIncomeContent->addWidget(std::make_unique<Wt::WBreak>());
	// addIncomeContent->addWidget(std::make_unique<Wt::WBreak>());

	// categoryContainer = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
	// categoryContainer->addWidget(std::make_unique<Wt::WBreak>());
	// categoryContainer->addWidget(std::make_unique<Wt::WBreak>());

	// categoryContainer->addWidget(std::make_unique<Wt::WLabel>("Expenses"));
	// categoryContainer->setStyleClass("title");
	// categoryContainer->addWidget(std::make_unique<Wt::WBreak>());
	// categoryContainer->addWidget(std::make_unique<Wt::WBreak>());

	// Initialize default categories
	std::unordered_set<std::string> defaultCategories = {
		"Housing", "Utilities", "Transportation", "Groceries",
		"Savings", "Investments", "Entertainment", "Other"};

	categoryContainer->addWidget(std::make_unique<Wt::WLabel>("Select Category: "));

	categoryComboBox = categoryContainer->addWidget(std::make_unique<Wt::WComboBox>());
	categoryComboBox->setStyleClass("custom-combobox");

	for (const auto &category : defaultCategories)
	{
		categoryComboBox->addItem(category);
	}

	categoryContainer->addWidget(std::make_unique<Wt::WBreak>());

	categoryContainer->addWidget(std::make_unique<Wt::WLabel>("Enter Custom Category: "));
	customCategoryInput = categoryContainer->addWidget(std::make_unique<Wt::WLineEdit>());

	addCustomCategoryButton = categoryContainer->addWidget(std::make_unique<Wt::WPushButton>("Add Custom Category"));
	addCustomCategoryButton->setStyleClass("custom-button");

	addCustomCategoryButton->clicked().connect([=]
											   {
        const Wt::WString customCategory = customCategoryInput->valueText();
        if (!customCategory.empty() && defaultCategories.find(customCategory.toUTF8()) == defaultCategories.end()) {
            if (categoryComboBox->findText(customCategory) == -1) {
                categoryComboBox->addItem(customCategory);
            } else {
                // Error message: Category already exists
                Wt::WMessageBox::show("Error", "Custom category already exists.", Wt::StandardButton::Ok);
            }
        } else {
            // Error message: Cannot add a default category as a custom category
            Wt::WMessageBox::show("Error", "Cannot add a default category as a custom category.", Wt::StandardButton::Ok);
        } });

	// Additional buttons for editing and deleting custom categories
	editCategoryButton = categoryContainer->addWidget(std::make_unique<Wt::WPushButton>("Edit Category"));
	editCategoryButton->setStyleClass("custom-button");
	deleteCategoryButton = categoryContainer->addWidget(std::make_unique<Wt::WPushButton>("Delete Category"));
	deleteCategoryButton->setStyleClass("custom-button");

	editCategoryButton->clicked().connect([=]
										  {
        const Wt::WString selectedCategory = categoryComboBox->currentText();
        if (defaultCategories.find(selectedCategory.toUTF8()) == defaultCategories.end()) {
            // Allow editing only for custom categories
            Wt::WDialog dialog("Edit Category");
            Wt::WLineEdit* editCategoryInput = dialog.contents()->addWidget(std::make_unique<Wt::WLineEdit>(selectedCategory));
            Wt::WPushButton* saveButton = dialog.footer()->addWidget(std::make_unique<Wt::WPushButton>("Save"));
			saveButton->setStyleClass("custom-button");

            saveButton->clicked().connect([=, &dialog] {
                const Wt::WString newCategoryName = editCategoryInput->valueText();
                if (!newCategoryName.empty() && categoryComboBox->findText(newCategoryName) == -1) {
                    categoryComboBox->setItemText(categoryComboBox->currentIndex(), newCategoryName);
                    dialog.accept();
                } else {
                    // Error message: New category name is empty or already exists
                    Wt::WMessageBox::show("Error", "Invalid or duplicate category name.", Wt::StandardButton::Ok);
                }
            });

            dialog.exec();
        } });

	deleteCategoryButton->clicked().connect([=]
											{
        const Wt::WString selectedCategory = categoryComboBox->currentText();
        if (defaultCategories.find(selectedCategory.toUTF8()) == defaultCategories.end()) {
            // Allow deletion only for custom categories
            categoryComboBox->removeItem(categoryComboBox->currentIndex());
        } else {
            // Error message: Cannot delete a default category
            Wt::WMessageBox::show("Error", "Cannot delete a default category.", Wt::StandardButton::Ok);
        } });

	categoryContainer->addWidget(std::make_unique<Wt::WBreak>());
	categoryContainer->addWidget(std::make_unique<Wt::WLabel>("Enter Expense Date: "));

	Wt::WComboBox *yearComboBox = createYearDropdown(categoryContainer);
	Wt::WComboBox *monthComboBox = createMonthDropdown(categoryContainer);
	Wt::WComboBox *dayComboBox = createDayDropdown(categoryContainer);

	categoryContainer->addWidget(std::make_unique<Wt::WBreak>());

	categoryContainer->addWidget(std::make_unique<Wt::WLabel>("Enter Expense Amount: "));

	Wt::WLineEdit *expenseAmountInput = categoryContainer->addWidget(std::make_unique<Wt::WLineEdit>());
	// expenseAmountInput = categoryContainer->addWidget(std::make_unique<Wt::WLineEdit>());

	submitExpenseButton = categoryContainer->addWidget(std::make_unique<Wt::WPushButton>("Submit Expense"));
	submitExpenseButton->setStyleClass("custom-button");
	categoryContainer->addWidget(std::make_unique<Wt::WBreak>());
	errorLabel = categoryContainer->addWidget(std::make_unique<Wt::WLabel>(""));

	/**
	 * Creates an expense table to hold all the user inputs of their expenses and display them on the screen.
	 */

	expenseTable = categoryContainer->addWidget(std::make_unique<Wt::WTable>());
	expenseTable->setHeaderCount(1);

	expenseTable->elementAt(0, 0)->addWidget(std::make_unique<Wt::WText>("Date"));
	expenseTable->elementAt(0, 1)->addWidget(std::make_unique<Wt::WText>("Category"));
	expenseTable->elementAt(0, 2)->addWidget(std::make_unique<Wt::WText>("Amount"));

	submitExpenseButton->clicked().connect([=]
										   {
		// Get the current date
		Wt::WDate currentDate = Wt::WDate::currentDate();

		// Gets the selected year, month, and day from the combo boxes
		int selectedYear = std::stoi(yearComboBox->currentText().toUTF8()); //Converts to an integer
		int selectedMonth = monthComboBox->currentIndex() + 1; // Adds one because the index is 0-based
		int selectedDay = std::stoi(dayComboBox->currentText().toUTF8()); //Converts to an integer

		// Create a WDate for the selected date
		Wt::WDate selectedDate(selectedYear, selectedMonth, selectedDay);

		/**
		* Error handling displays an error if users input an invalid date (in the future or non existent)
		*/
		if (selectedDate > currentDate) {
			errorLabel->setText("Please choose a valid date.");
		} 
		
		else if ((selectedMonth==4 || selectedMonth==6 || selectedMonth==9 || selectedMonth==11) && selectedDay>30) {
			errorLabel->setText("Please choose a valid date.");
		}

		else if (selectedMonth==2 && selectedDay>28) {
			errorLabel->setText("Please choose a valid date.");
		}

		else {
			// Date is valid, continue with adding the expense
			errorLabel->setText("");
			const Wt::WString category = categoryComboBox->currentText();
			const Wt::WString amount = expenseAmountInput->valueText();
			const Wt::WString year = yearComboBox->valueText();
			const Wt::WString month = monthComboBox->valueText();
			const Wt::WString day = dayComboBox->valueText();
			const Wt::WString date = year + L"-" + month + L"-" + day;
			double amountValue;

			/**
			 * Attempts to convert the amount to an integer - if not a number input, it returns an error
			*/
			try {
				amountValue = std::stod(expenseAmountInput->text().toUTF8());
			} catch (const std::invalid_argument& e) {
				std::string error_message = "Please enter a valid expense amount. (Cannot convert amount to number. Error: ";
				error_message += e.what(); // Add the exception's error message.
				error_message += ")";
				errorLabel->setText(error_message);
			}
			
		
			/**
			 * Returns errors if the expense is greater than the user's balance, or if the expense is
			 * negative, 0 or blank
			*/
			if (amountValue > balance) {
				errorLabel->setText("You do not have enough balance for this expense.");
			}
			else if (amountValue <= 0) {
				errorLabel->setText("Please enter a valid expense amount. (Amount is less than 0)");
			}
			else {
				updateBalance(-amountValue);
			
				if (!category.empty() && !amount.empty() && !date.empty()) {
					// Add the expense to the SQLite database and update the table
					int row = expenseTable->rowCount();
					expenseTable->elementAt(row, 0)->addWidget(std::make_unique<Wt::WText>(date));
					expenseTable->elementAt(row, 1)->addWidget(std::make_unique<Wt::WText>(category));
					expenseTable->elementAt(row, 2)->addWidget(std::make_unique<Wt::WText>(amount));
				}
			} 
		} });
}

/**
 * Check balance page handles the user checking their balance.
 */
void FinancialManagerApp::setupCheckBalancePage(Wt::WContainerWidget *parent)
{
	checkBalanceContent = root()->addWidget(std::make_unique<Wt::WContainerWidget>());

	checkBalanceContent->addWidget(std::make_unique<Wt::WText>("Click the button to check your balance"));
	checkBalanceContent->addWidget(std::make_unique<Wt::WBreak>());
	Wt::WPushButton *checkBalanceButton = checkBalanceContent->addWidget(std::make_unique<Wt::WPushButton>("Check Balance"));
	checkBalanceButton->setStyleClass("custom-button");

	checkBalanceContent->addWidget(std::make_unique<Wt::WBreak>());

	checkingBalanceDisplay = checkBalanceContent->addWidget(std::make_unique<Wt::WLabel>("")); // Empty text initially
	checkBalanceContent->addWidget(std::make_unique<Wt::WBreak>());
	savingBalanceDisplay = checkBalanceContent->addWidget(std::make_unique<Wt::WLabel>("")); // Empty text initially
	checkBalanceContent->addWidget(std::make_unique<Wt::WBreak>());
	investmentBalanceDisplay = checkBalanceContent->addWidget(std::make_unique<Wt::WLabel>("")); // Empty text initially

	checkBalanceButton->clicked().connect([=]
										  { updateBalance(0.0); });
}

/**
 * Add income page handles all of the user's income inputs.
 */
void FinancialManagerApp::setupAddIncomePage(Wt::WContainerWidget *parent)
{

	Wt::WContainerWidget *addIncomeContent = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
	addIncomeContent->addWidget(std::make_unique<Wt::WBreak>());
	// addIncomeContent->setStyleClass("add-income-content");

	Wt::WContainerWidget *titleContainer = addIncomeContent->addWidget(std::make_unique<Wt::WContainerWidget>());
	titleContainer->addWidget(std::make_unique<Wt::WLabel>("Income"))->setStyleClass("title");

	// addIncomeContent = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
	// addIncomeContent->addWidget(std::make_unique<Wt::WLabel>("Income"));
	// addIncomeContent->setStyleClass("title");
	// addIncomeContent->addWidget(std::make_unique<Wt::WBreak>());
	// addIncomeContent->addWidget(std::make_unique<Wt::WBreak>());

	profileComboBox = createProfileDropdown(addIncomeContent);
	profileComboBox->changed().connect(this, &FinancialManagerApp::onProfileChange);
	addIncomeContent->addWidget(std::make_unique<Wt::WBreak>());

	Wt::WText *addIncomeHint = addIncomeContent->addWidget(std::make_unique<Wt::WText>("Enter the income amount:"));
	Wt::WLineEdit *addIncomeInput = addIncomeContent->addWidget(std::make_unique<Wt::WLineEdit>());

	addIncomeContent->addWidget(std::make_unique<Wt::WBreak>());
	Wt::WPushButton *addIncomeButton = addIncomeContent->addWidget(std::make_unique<Wt::WPushButton>("Add Income"));
	addIncomeButton->setStyleClass("custom-button");

	addIncomeButton->clicked().connect([=]
									   {
		double income = std::stod(addIncomeInput->text().toUTF8());
		updateBalance(income); });
}

/**
 * Convert currency page handles the user converting their currency to USD, EUR or GBP.
 */

void FinancialManagerApp::setupConvertCurrencyPage(Wt::WContainerWidget *parent)
{
	Wt::WContainerWidget *convertCurrencyContent = root()->addWidget(std::make_unique<Wt::WContainerWidget>());

	std::unordered_set<std::string> defaultCurrencies = {
		"USD", "EUR", "GBP"};

	convertCurrencyContent->addWidget(std::make_unique<Wt::WLabel>("Enter the Amount: "));
	Wt::WLineEdit *amountInput = convertCurrencyContent->addWidget(std::make_unique<Wt::WLineEdit>());
	convertCurrencyContent->addWidget(std::make_unique<Wt::WBreak>()); // Add a line break

	// Add a line for entering the currency and a dropdown for selecting the currency to convert to CAD
	convertCurrencyContent->addWidget(std::make_unique<Wt::WLabel>("Enter the Currency to Convert from to CAD:"));
	Wt::WComboBox *currencyComboBox = convertCurrencyContent->addWidget(std::make_unique<Wt::WComboBox>());
	currencyComboBox->setStyleClass("custom-combobox");
	convertCurrencyContent->addWidget(std::make_unique<Wt::WBreak>()); // Add a line break

	for (const auto &currency : defaultCurrencies)
	{
		currencyComboBox->addItem(currency);
	}

	Wt::WPushButton *convertCurrencyButton = convertCurrencyContent->addWidget(std::make_unique<Wt::WPushButton>("Convert Currency"));
	convertCurrencyButton->setStyleClass("custom-button");
	Wt::WText *convertCurrencyResult = convertCurrencyContent->addWidget(std::make_unique<Wt::WText>());

	convertCurrencyButton->clicked().connect([=]
											 {
                                                 double amount = std::stod(amountInput->text().toUTF8());
                                                 std::string currency = currencyComboBox->currentText().toUTF8();
                                                 double convertedAmount = CurrencyConverter::convertCurrency(amount, currency);
                                                 userManager.getCurrentUser().addIncome(convertedAmount);

                                                 std::ostringstream convertCurrencyResultText;
                                                 convertCurrencyResultText << "Converted amount: $" << std::fixed << std::setprecision(2) << convertedAmount;

                                                 convertCurrencyResult->setText(convertCurrencyResultText.str()); });
}

void FinancialManagerApp::setupBudgetPage(Wt::WContainerWidget *parent)
{

	Wt::WContainerWidget *addBudgetContent = root()->addWidget(std::make_unique<Wt::WContainerWidget>());

	Wt::WContainerWidget *titleContainer = addBudgetContent->addWidget(std::make_unique<Wt::WContainerWidget>());
	titleContainer->addWidget(std::make_unique<Wt::WLabel>("Budget"))->setStyleClass("title");

	// Add labels and input boxes with line breaks after each pair
	addBudgetContent->addWidget(std::make_unique<Wt::WBreak>());

	addBudgetContent->addWidget(std::make_unique<Wt::WLabel>("Set the amount you want to budget: "));
	Wt::WLineEdit *addFullBudget = addBudgetContent->addWidget(std::make_unique<Wt::WLineEdit>());
	addFullBudget->setPlaceholderText("Total Budget");
	addBudgetContent->addWidget(std::make_unique<Wt::WBreak>());

	addBudgetContent->addWidget(std::make_unique<Wt::WLabel>("Housing: "));
	Wt::WLineEdit *addHousingInput = addBudgetContent->addWidget(std::make_unique<Wt::WLineEdit>());
	addHousingInput->setPlaceholderText("Housing");
	addBudgetContent->addWidget(std::make_unique<Wt::WBreak>());

	addBudgetContent->addWidget(std::make_unique<Wt::WLabel>("Utilities: "));
	Wt::WLineEdit *addUtilitiesInput = addBudgetContent->addWidget(std::make_unique<Wt::WLineEdit>());
	addUtilitiesInput->setPlaceholderText("Utilities");
	addBudgetContent->addWidget(std::make_unique<Wt::WBreak>());

	addBudgetContent->addWidget(std::make_unique<Wt::WLabel>("Transportation: "));
	Wt::WLineEdit *addTransportationInput = addBudgetContent->addWidget(std::make_unique<Wt::WLineEdit>());
	addTransportationInput->setPlaceholderText("Transportation");
	addBudgetContent->addWidget(std::make_unique<Wt::WBreak>());

	addBudgetContent->addWidget(std::make_unique<Wt::WLabel>("Groceries: "));
	Wt::WLineEdit *addGroceriesInput = addBudgetContent->addWidget(std::make_unique<Wt::WLineEdit>());
	addGroceriesInput->setPlaceholderText("Groceries");
	addBudgetContent->addWidget(std::make_unique<Wt::WBreak>());

	addBudgetContent->addWidget(std::make_unique<Wt::WLabel>("Savings: "));
	Wt::WLineEdit *addSavingsInput = addBudgetContent->addWidget(std::make_unique<Wt::WLineEdit>());
	addSavingsInput->setPlaceholderText("Savings");
	addBudgetContent->addWidget(std::make_unique<Wt::WBreak>());

	addBudgetContent->addWidget(std::make_unique<Wt::WLabel>("Investments: "));
	Wt::WLineEdit *addInvestmentsInput = addBudgetContent->addWidget(std::make_unique<Wt::WLineEdit>());
	addInvestmentsInput->setPlaceholderText("Investments");
	addBudgetContent->addWidget(std::make_unique<Wt::WBreak>());

	addBudgetContent->addWidget(std::make_unique<Wt::WLabel>("Entertainment: "));
	Wt::WLineEdit *addEntertainmentInput = addBudgetContent->addWidget(std::make_unique<Wt::WLineEdit>());
	addEntertainmentInput->setPlaceholderText("Entertainment");
	addBudgetContent->addWidget(std::make_unique<Wt::WBreak>());

	addBudgetContent->addWidget(std::make_unique<Wt::WLabel>("Other: "));
	Wt::WLineEdit *addOtherInput = addBudgetContent->addWidget(std::make_unique<Wt::WLineEdit>());
	addOtherInput->setPlaceholderText("Other");
	addBudgetContent->addWidget(std::make_unique<Wt::WBreak>());

	Wt::WPushButton *addBudgets = addBudgetContent->addWidget(std::make_unique<Wt::WPushButton>("Set Budget"));
	addBudgets->setStyleClass("custom-button");

	addBudgets->clicked().connect([=]
								  {
									  double fullBudget = std::stod(addFullBudget->text().toUTF8());
									  double housingBudget = addHousingInput->text().empty() ? 0.0 : std::stod(addHousingInput->text().toUTF8());
									  double utilitiesBudget = addUtilitiesInput->text().empty() ? 0.0 : std::stod(addUtilitiesInput->text().toUTF8());
									  double transportationBudget = addTransportationInput->text().empty() ? 0.0 : std::stod(addTransportationInput->text().toUTF8());
									  double groceriesBudget = addGroceriesInput->text().empty() ? 0.0 : std::stod(addGroceriesInput->text().toUTF8());
									  double savingsBudget = addSavingsInput->text().empty() ? 0.0 : std::stod(addSavingsInput->text().toUTF8());
									  double investmentsBudget = addInvestmentsInput->text().empty() ? 0.0 : std::stod(addInvestmentsInput->text().toUTF8());
									  double entertainmentBudget = addEntertainmentInput->text().empty() ? 0.0 : std::stod(addEntertainmentInput->text().toUTF8());
									  double otherBudget = addOtherInput->text().empty() ? 0.0 : std::stod(addOtherInput->text().toUTF8());

									  double totalBudget = housingBudget + utilitiesBudget + transportationBudget + groceriesBudget + savingsBudget + investmentsBudget + entertainmentBudget + otherBudget;
									  if (totalBudget > fullBudget)
									  {
										  Wt::WMessageBox::show(Wt::WString("Budget is too high!"), Wt::WString("Please adjust your budget."), Wt::StandardButton::Ok);
									  }
									  else if (totalBudget < fullBudget)
									  {
										  Wt::WMessageBox::show(Wt::WString("Budget is too low!"), Wt::WString("Please adjust your budget."), Wt::StandardButton::Ok);
									  }
									  else
									  {
										  Wt::WMessageBox::show(Wt::WString("Budget matches!"), Wt::WString("Budget has been set."), Wt::StandardButton::Ok);
									  }
									  // Update balance directly from the user manager
								  });
}

void FinancialManagerApp::setupExpenseReminderPage(Wt::WContainerWidget *parent)
{

	/**
	 * Creates the layout and the dropdown box for the categories and dates.
	 */
	// reminderContainer = root()->addWidget(std::make_unique<Wt::WContainerWidget>());

	Wt::WContainerWidget *reminderContainer = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
	reminderContainer->addWidget(std::make_unique<Wt::WBreak>());

	Wt::WContainerWidget *titleContainer = reminderContainer->addWidget(std::make_unique<Wt::WContainerWidget>());
	titleContainer->addWidget(std::make_unique<Wt::WLabel>("Reminders"))->setStyleClass("title");

	reminderContainer->addWidget(std::make_unique<Wt::WBreak>());

	reminderContainer->addWidget(std::make_unique<Wt::WLabel>("Enter Reminder Date: "));

	Wt::WComboBox *yearComboBox = createYearDropdown(reminderContainer);
	yearComboBox->setStyleClass("custom-combobox");
	Wt::WComboBox *monthComboBox = createMonthDropdown(reminderContainer);
	monthComboBox->setStyleClass("custom-combobox");
	Wt::WComboBox *dayComboBox = createDayDropdown(reminderContainer);
	monthComboBox->setStyleClass("custom-combobox");

	reminderContainer->addWidget(std::make_unique<Wt::WBreak>());

	reminderContainer->addWidget(std::make_unique<Wt::WLabel>("Enter Reminder Title: "));
	reminderTitleInput = reminderContainer->addWidget(std::make_unique<Wt::WLineEdit>());

	reminderContainer->addWidget(std::make_unique<Wt::WBreak>());

	reminderContainer->addWidget(std::make_unique<Wt::WLabel>("Enter Reminder Amount: "));
	expenseAmountInput = reminderContainer->addWidget(std::make_unique<Wt::WLineEdit>());

	submitExpenseButton = reminderContainer->addWidget(std::make_unique<Wt::WPushButton>("Set Reminder"));
	submitExpenseButton->setStyleClass("custom-button");
	reminderContainer->addWidget(std::make_unique<Wt::WBreak>());
	errorLabel2 = reminderContainer->addWidget(std::make_unique<Wt::WLabel>(""));

	/**
	 * Creates an expense table to hold all the user inputs of their expenses and display them on the screen.
	 */

	reminderTable = reminderContainer->addWidget(std::make_unique<Wt::WTable>());
	reminderTable->setHeaderCount(1);

	reminderTable->elementAt(0, 0)->addWidget(std::make_unique<Wt::WText>("Days Until Payment is Due"));
	reminderTable->elementAt(0, 1)->addWidget(std::make_unique<Wt::WText>("Expense"));
	reminderTable->elementAt(0, 2)->addWidget(std::make_unique<Wt::WText>("Amount"));
	reminderTable->elementAt(0, 3)->addWidget(std::make_unique<Wt::WText>(""));

	submitExpenseButton->clicked().connect([=]
										   {
    // Get the current date
    Wt::WDate currentDate = Wt::WDate::currentDate();

    // Gets the selected year, month, and day from the combo boxes
    int selectedYear = std::stoi(yearComboBox->currentText().toUTF8()); //Converts to an integer
    int selectedMonth = monthComboBox->currentIndex() + 1; // Adds one because the index is 0-based
    int selectedDay = std::stoi(dayComboBox->currentText().toUTF8()); //Converts to an integer

    // Create a WDate for the selected date
    Wt::WDate selectedDate(selectedYear, selectedMonth, selectedDay);

	int daysUntilDue = currentDate.daysTo(selectedDate);

/**
 * Error handling displays an error if users input an invalid date (in the future or non existent)
*/
    if (selectedDate < currentDate) {
		errorLabel2->setText("Please choose a valid future date.");
    }

	else if ((selectedMonth==4 || selectedMonth==6 || selectedMonth==9 || selectedMonth==11) && selectedDay>30) {
		errorLabel2->setText("Please choose a valid date.");
	}

	else if (selectedMonth==2 && selectedDay>28) {
		errorLabel2->setText("Please choose a valid date.");
	}

	else {
        // Date is valid, continue with adding the expense
		errorLabel2->setText("");
        const Wt::WString amount = expenseAmountInput->valueText();
		const Wt::WString title = reminderTitleInput->valueText();
		const Wt::WString daysUntilDueString = Wt::WString::fromUTF8(std::to_string(daysUntilDue));
		double amountValue;
/**
 * Attempts to convert the amount to an integer - if not a number input, it returns an error
*/
		try {
    		amountValue = std::stod(amount.toUTF8());
		} catch (const std::invalid_argument& e) {
    		errorLabel2->setText("Please enter a valid reminder amount.");
		}


        if (!title.empty() && !amount.empty() && !daysUntilDueString.empty()) {

            // Display the Reminder in the reminder table
            int row = reminderTable->rowCount();
            reminderTable->elementAt(row, 0)->addWidget(std::make_unique<Wt::WText>(daysUntilDueString));
            reminderTable->elementAt(row, 1)->addWidget(std::make_unique<Wt::WText>(title));
            reminderTable->elementAt(row, 2)->addWidget(std::make_unique<Wt::WText>(amount));
        }
    } });
}

void FinancialManagerApp::copyTableRow(Wt::WTable *sourceTable, Wt::WTable *destTable, int rowIndex)
{
	// std::cout << "Starting copyTableRow, rowIndex: " << rowIndex << std::endl;

	if (!sourceTable || !destTable)
	{
		// std::cerr << "Source or destination table is null." << std::endl;
		return;
	}

	if (rowIndex < 0 || rowIndex >= sourceTable->rowCount())
	{
		// std::cerr << "Invalid row index." << std::endl;
		return;
	}

	int newRow = destTable->rowCount();
	destTable->insertRow(newRow);
	// std::cout << "New row inserted at index: " << newRow << std::endl;

	for (int col = 0; col < sourceTable->columnCount(); ++col)
	{
		// std::cout << "Processing column: " << col << std::endl;

		Wt::WWidget *widget = sourceTable->elementAt(rowIndex, col)->widget(0);
		if (widget)
		{
			auto text = dynamic_cast<Wt::WText *>(widget);
			if (text)
			{
				destTable->elementAt(newRow, col)->addWidget(std::make_unique<Wt::WText>(text->text()));
			}
			else
			{
				// std::cerr << "Widget is not Wt::WText." << std::endl;
			}
		}
		else
		{
			// std::cerr << "No widget found in cell." << std::endl;
			destTable->elementAt(newRow, col)->addWidget(std::make_unique<Wt::WText>("null"));
		}
	}
	// std::cout << "Row copied successfully." << std::endl;
}

Wt::WDate FinancialManagerApp::parseDateFromString(const std::string &dateString)
{
	std::istringstream iss(dateString);
	std::vector<std::string> parts;
	std::string part;
	while (std::getline(iss, part, '-'))
	{
		parts.push_back(part);
	}

	if (parts.size() != 3)
	{
		throw std::runtime_error("Invalid date format");
	}

	// Convert month name to number
	std::map<std::string, int> monthMap = {
		{"January", 1}, {"February", 2}, {"March", 3}, {"April", 4}, {"May", 5}, {"June", 6}, {"July", 7}, {"August", 8}, {"September", 9}, {"October", 10}, {"November", 11}, {"December", 12}};

	int year = std::stoi(parts[0]);
	int month = monthMap[parts[1]];
	int day = std::stoi(parts[2]);

	if (month == 0)
	{
		throw std::runtime_error("Invalid month name");
	}

	Wt::WDate date(year, month, day);
	if (!date.isValid())
	{
		throw std::runtime_error("Invalid date");
	}

	return date;
}

void FinancialManagerApp::setupExpenseSummaryPage(Wt::WContainerWidget *parent)
{

	// Create Layout

	ExpenseContainer = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
	ExpenseContainer->addWidget(std::make_unique<Wt::WBreak>());

	Wt::WContainerWidget *ExpenseContainer = root()->addWidget(std::make_unique<Wt::WContainerWidget>());

	Wt::WContainerWidget *titleContainer = ExpenseContainer->addWidget(std::make_unique<Wt::WContainerWidget>());
	titleContainer->addWidget(std::make_unique<Wt::WLabel>("Expense Summary"))->setStyleClass("title");

	ExpenseContainer->addWidget(std::make_unique<Wt::WBreak>());

	// Date from start to end, empty initially

	ExpenseContainer->addWidget(std::make_unique<Wt::WLabel>("Enter Start Date: "));

	Wt::WComboBox *startYearComboBox = createYearDropdown(ExpenseContainer);
	startYearComboBox->setStyleClass("custom-combobox");
	Wt::WComboBox *startMonthComboBox = createMonthDropdown(ExpenseContainer);
	startMonthComboBox->setStyleClass("custom-combobox");
	Wt::WComboBox *startDayComboBox = createDayDropdown(ExpenseContainer);
	startDayComboBox->setStyleClass("custom-combobox");

	ExpenseContainer->addWidget(std::make_unique<Wt::WBreak>());

	ExpenseContainer->addWidget(std::make_unique<Wt::WLabel>("Enter End Date: "));

	Wt::WComboBox *endYearComboBox = createYearDropdown(ExpenseContainer);
	endYearComboBox->setStyleClass("custom-combobox");
	Wt::WComboBox *endMonthComboBox = createMonthDropdown(ExpenseContainer);
	endMonthComboBox->setStyleClass("custom-combobox");
	Wt::WComboBox *endDayComboBox = createDayDropdown(ExpenseContainer);
	endDayComboBox->setStyleClass("custom-combobox");

	ExpenseContainer->addWidget(std::make_unique<Wt::WBreak>());
	submitExpenseSummaryButton = ExpenseContainer->addWidget(std::make_unique<Wt::WPushButton>("Submit Dates"));
	submitExpenseSummaryButton->setStyleClass("custom-button");

	ExpenseContainer->addWidget(std::make_unique<Wt::WBreak>());
	errorLabel2 = ExpenseContainer->addWidget(std::make_unique<Wt::WLabel>(""));

	expenseSummaryTable = ExpenseContainer->addWidget(std::make_unique<Wt::WTable>());

	submitExpenseSummaryButton->clicked().connect([=]
												  {	
		expenseSummaryTable->clear();

		expenseSummaryTable->setHeaderCount(1);

		expenseSummaryTable->elementAt(0, 0)->addWidget(std::make_unique<Wt::WText>("Date"));
		expenseSummaryTable->elementAt(0, 1)->addWidget(std::make_unique<Wt::WText>("Category"));
		expenseSummaryTable->elementAt(0, 2)->addWidget(std::make_unique<Wt::WText>("Amount"));
	

		// Gets the selected year, month, and day from the combo boxes
		int startYear = std::stoi(startYearComboBox->currentText().toUTF8()); //Converts to an integer
		int startMonth = startMonthComboBox->currentIndex() + 1; // Adds one because the index is 0-based
		int startDay = std::stoi(startDayComboBox->currentText().toUTF8()); //Converts to an integer

		// Create a WDate for the selected date
		Wt::WDate startDate(startYear, startMonth, startDay);

				// Gets the selected year, month, and day from the combo boxes
		int endYear = std::stoi(endYearComboBox->currentText().toUTF8()); //Converts to an integer
		int endMonth = endMonthComboBox->currentIndex() + 1; // Adds one because the index is 0-based
		int endDay = std::stoi(endDayComboBox->currentText().toUTF8()); //Converts to an integer

		// Create a WDate for the selected date
		Wt::WDate endDate(endYear, endMonth, endDay);

		if ((startMonth==4 || startMonth==6 || startMonth==9 || startMonth==11) && startDay>30) {
			errorLabel2->setText("Please choose a valid start date.");
		}
		else if ((endMonth==4 || endMonth==6 || endMonth==9 || endMonth==11) && endDay>30) {
			errorLabel2->setText("Please choose a valid end date.");
		}
		else if (startMonth==2 && startDay>28) {
			errorLabel2->setText("Please choose a valid start date.");
		}
		else if (endMonth==2 && endDay>28) {
			errorLabel2->setText("Please choose a valid end date.");
		}
		else if (startDate > endDate)
		{
			errorLabel2->setText("Start Date cannot be greater than End Date, Please enter Valid Date");
		}
		else
		{
			errorLabel2->setText("");

			for (int row = 1; row < expenseTable->rowCount(); ++row) {
			
				Wt::WText* dateTextWidget = dynamic_cast<Wt::WText*>(expenseTable->elementAt(row, 0)->widget(0));
				if (!dateTextWidget) {
					// Handle error: widget is not a Wt::WText or does not exist
					continue; // Skip to the next row
				}
				std::string dateString = dateTextWidget->text().toUTF8();

				try {
					Wt::WDate extractedDate = parseDateFromString(dateString);
					if (extractedDate >= startDate && extractedDate <= endDate) {
						copyTableRow(expenseTable, expenseSummaryTable, row);
					}
				} catch (const std::runtime_error& e) {
					// Handle parsing error
					std::cerr << "Error parsing date on row " << row << ": " << e.what() << std::endl;
					continue; // Skip to the next row
				}
			}
		} });
}
