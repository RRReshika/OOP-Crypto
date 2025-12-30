#include "RMenu.h"
#include "RCSVReader.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include "RUIUtils.h"

Menu::Menu(UserManager& userManager, TransactionManager& transactionManager, TradeSimulator& tradeSimulator)
    : userManager(userManager), transactionManager(transactionManager), tradeSimulator(tradeSimulator), currentUser("") {}

void Menu::init() {
    while (true) {
        
        if (currentUser == "") {
            UI::printHeader("CRYPTO SIMULATOR - WELCOME");
            std::cout << " " << UI::BOLD << UI::YELLOW << "1:" << UI::RESET << " Register\n";
            std::cout << " " << UI::BOLD << UI::YELLOW << "2:" << UI::RESET << " Login\n";
            std::cout << " " << UI::BOLD << UI::RED << "3:" << UI::RESET << " Exit\n";
            std::cout << "\nType your choice: ";
            int option = getUserOption();
            if (option == 1) handleRegister();
            else if (option == 2) handleLogin();
            else if (option == 3) break;
        } else {
            printMenu();
            int option = getUserOption();
            processOption(option);
            if (option == 11) currentUser = ""; // Logout
        }
    }
}

void Menu::printMenu() {
    
    UI::printHeader("CRYPTO SIMULATOR - MAIN MENU");
    std::cout << " " << UI::BOLD << "Logged in as: " << UI::GREEN << currentUser << UI::RESET << "\n";
    std::cout << "------------------------------------\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "1:" << UI::RESET << " Exchange Stats\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "2:" << UI::RESET << " Make Offer (Ask)\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "3:" << UI::RESET << " Make Bid\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "4:" << UI::RESET << " Wallet\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "5:" << UI::RESET << " Transactions\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "6:" << UI::RESET << " Deposit\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "7:" << UI::RESET << " Withdraw\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "8:" << UI::RESET << " Market Stats\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "9:" << UI::RESET << " Continue (Simulate Step)\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "10:" << UI::RESET << " Market Analysis\n";
    std::cout << " " << UI::BOLD << UI::RED << "11:" << UI::RESET << " Logout\n";
    std::cout << "------------------------------------\n";
    std::cout << "Type your choice: ";
}

int Menu::getUserOption() {
    int option = -1;
    std::string line;
    if (std::getline(std::cin, line)) {
        try {
            option = std::stoi(line);
        } catch (...) {
            option = -1;
        }
    }
    return option;
}

std::string Menu::promptProductSelection() {
    std::vector<std::string> products = tradeSimulator.getMarketData().getProducts();
    std::cout << "\nAvailable Products:\n";
    for (size_t i = 0; i < products.size(); ++i) {
        std::cout << i + 1 << ": " << products[i] << "\n";
    }
    std::cout << "Select product by number: ";
    int choice = getUserOption();
    if (choice > 0 && choice <= static_cast<int>(products.size())) {
        return products[choice - 1];
    }
    return "";
}

void Menu::processOption(int option) {
    switch (option) {
        case 1: handleExchangeStats(); break;
        case 2: handleMakeOffer(); break;
        case 3: handleMakeBid(); break;
        case 4: handleWallet(); break;
        case 5: handleTransactions(); break;
        case 6: handleDeposit(); break;
        case 7: handleWithdraw(); break;
        case 8: handleStats(); break;
        case 9: handleContinue(); break;
        case 10: handleMarketAnalysis(); break;
        case 11: break; // Logout handled in init()
        default: std::cout << "Invalid option\n"; break;
    }
}

void Menu::handleRegister() {
    std::string fullName, email, password, username, customUsername;
    UI::printHeader("REGISTER NEW USER");
    
    std::cout << "Full Name: ";
    std::getline(std::cin, fullName);
    
    std::cout << "Email: ";
    std::getline(std::cin, email);
    
    std::cout << "Password: ";
    std::getline(std::cin, password);
    
    std::cout << "Do you want to create a custom username? (y/n): ";
    std::string choice;
    std::getline(std::cin, choice);
    if (choice == "y" || choice == "Y") {
        std::cout << "Enter custom username: ";
        std::getline(std::cin, customUsername);
    }
    
    if (fullName.empty() || email.empty() || password.empty()) {
        UI::printError("All fields are required.");
    } else if (userManager.registerUser(fullName, email, password, username, customUsername)) {
        UI::printSuccess("Registered successfully!");
        std::cout << "Your 10-digit ID is: " << UI::BOLD << UI::CYAN << username << UI::RESET << "\n";
        if (!customUsername.empty()) {
            std::cout << "Your custom username is: " << UI::BOLD << UI::CYAN << customUsername << UI::RESET << "\n";
        }
        std::cout << "You can use either to log in.\n";
    } else {
        UI::printError("Registration failed.");
        std::cout << "Custom username might be taken, or you already have an account.\n";
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handleLogin() {
    std::string u, p;
    
    UI::printHeader("LOGIN");
    
    std::cout << "Username: ";
    std::getline(std::cin, u);
    
    std::cout << "Password: ";
    std::getline(std::cin, p);
    
    if (userManager.loginUser(u, p)) {
        currentUser = u;
        UI::printSuccess("Logged in!");
    } else {
        UI::printError("Invalid credentials.");
        std::cout << "\nOptions:\n";
        std::cout << " 1: Re-enter credentials\n";
        std::cout << " 2: Forgot username? (Search by email + full name)\n";
        std::cout << " 3: Reset password\n";
        std::cout << " 4: Back to welcome screen\n";
        std::cout << "Choice: ";
        int choice = getUserOption();
        if (choice == 1) handleLogin();
        else if (choice == 2) handleForgotUsername();
        else if (choice == 3) handlePasswordReset();
    }
    if (currentUser != "") {
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
}

void Menu::handleForgotUsername() {
    std::string fullName, email;
    UI::printHeader("FORGOT USERNAME");
    std::cout << "Enter Full Name: ";
    std::getline(std::cin, fullName);
    std::cout << "Enter Email: ";
    std::getline(std::cin, email);
    
    std::string username = userManager.findUsername(fullName, email);
    if (username != "") {
        UI::printSuccess("User found!");
        std::cout << "Your username is: " << UI::BOLD << UI::CYAN << username << UI::RESET << "\n";
    } else {
        UI::printError("No user found with these details.");
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handlePasswordReset() {
    std::string fullName, email, newPassword;
    UI::printHeader("PASSWORD RESET");
    std::cout << "Enter Full Name: ";
    std::getline(std::cin, fullName);
    std::cout << "Enter Email: ";
    std::getline(std::cin, email);
    std::cout << "Enter New Password: ";
    std::getline(std::cin, newPassword);
    
    if (userManager.resetPassword(fullName, email, newPassword)) {
        UI::printSuccess("Password updated successfully!");
    } else {
        UI::printError("Verification failed. No user found with these details.");
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handleExchangeStats() {
    
    UI::printHeader("EXCHANGE STATISTICS (LATEST ORDERS)");
    std::vector<Order> orders = tradeSimulator.getLatestOrders();
    if (orders.empty()) {
        std::cout << "No orders available.\n";
    } else {
        std::cout << std::left << std::setw(20) << "Timestamp" 
                  << std::setw(12) << "Product" 
                  << std::setw(10) << "Type" 
                  << std::setw(12) << "Price" 
                  << std::setw(12) << "Amount" << "\n";
        std::cout << std::string(66, '-') << "\n";
        for (const auto& o : orders) {
            std::string typeColor = (o.type == "ask") ? UI::RED : UI::GREEN;
            std::cout << std::left << std::setw(20) << o.timestamp 
                      << std::setw(12) << o.product 
                      << typeColor << std::setw(10) << o.type << UI::RESET
                      << std::fixed << std::setprecision(4)
                      << std::setw(12) << o.price 
                      << std::setw(12) << o.amount << "\n";
        }
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Menu::handleMakeOffer() {
    std::string product = promptProductSelection();
    if (product == "") {
        std::cout << "\n[!] Invalid product selection.\n";
        return;
    }

    double amount, price;
    std::cout << "Enter amount to sell: ";
    std::string amountStr;
    std::getline(std::cin, amountStr);
    try { amount = std::stod(amountStr); } catch (...) { std::cout << "[!] Invalid amount.\n"; return; }

    std::cout << "Enter price: ";
    std::string priceStr;
    std::getline(std::cin, priceStr);
    try { price = std::stod(priceStr); } catch (...) { std::cout << "[!] Invalid price.\n"; return; }

    Wallet& wallet = userManager.getWallet(currentUser);
    std::string base = product.substr(0, product.find('/'));
    
    if (wallet.removeCurrency(base, amount)) {
        transactionManager.saveTransaction(Transaction(CSVReader::getCurrentTimestamp(), currentUser, TransactionType::ask, product, amount, price, wallet.getBalance(base)));
        userManager.saveWallets();
        std::cout << "\n[SUCCESS] Offer placed for " << amount << " " << base << " at " << price << ".\n";
    } else {
        std::cout << "\n[ERROR] Insufficient balance. You need " << amount << " " << base << " but only have " << wallet.getBalance(base) << ".\n";
    }
}

void Menu::handleMakeBid() {
    std::string product = promptProductSelection();
    if (product == "") {
        std::cout << "\n[!] Invalid product selection.\n";
        return;
    }

    double amount, price;
    std::cout << "Enter amount to buy: ";
    std::string amountStr;
    std::getline(std::cin, amountStr);
    try { amount = std::stod(amountStr); } catch (...) { std::cout << "[!] Invalid amount.\n"; return; }

    std::cout << "Enter price: ";
    std::string priceStr;
    std::getline(std::cin, priceStr);
    try { price = std::stod(priceStr); } catch (...) { std::cout << "[!] Invalid price.\n"; return; }

    Wallet& wallet = userManager.getWallet(currentUser);
    std::string quote = product.substr(product.find('/') + 1);
    double totalCost = amount * price;

    if (wallet.removeCurrency(quote, totalCost)) {
        transactionManager.saveTransaction(Transaction(CSVReader::getCurrentTimestamp(), currentUser, TransactionType::bid, product, amount, price, wallet.getBalance(quote)));
        userManager.saveWallets();
        std::cout << "\n[SUCCESS] Bid placed for " << amount << " " << product.substr(0, product.find('/')) << " at total cost of " << totalCost << " " << quote << ".\n";
    } else {
        std::cout << "\n[ERROR] Insufficient balance. Total cost is " << totalCost << " " << quote << " but you only have " << wallet.getBalance(quote) << ".\n";
    }
}


void Menu::handleContinue() {
    tradeSimulator.generateOrders();
    UI::printSuccess("Market updated with new orders for the next step.");
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Menu::handleMarketAnalysis() {
    std::string product = promptProductSelection();
    if (product == "") {
        UI::printWarning("Invalid product selection.");
        return;
    }

    
    UI::printHeader("MARKET ANALYSIS - " + product);
    std::cout << "Select timeframe:\n";
    std::cout << " " << UI::BOLD << UI::YELLOW << "1:" << UI::RESET << " Yearly\n";
    std::cout << " " << UI::BOLD << UI::YELLOW << "2:" << UI::RESET << " Monthly\n";
    std::cout << " " << UI::BOLD << UI::YELLOW << "3:" << UI::RESET << " Daily\n";
    std::cout << "Choice: ";
    int tfChoice = getUserOption();
    std::string timeframe = "yearly";
    if (tfChoice == 2) timeframe = "monthly";
    else if (tfChoice == 3) timeframe = "daily";
    else if (tfChoice != 1) {
        UI::printWarning("Invalid choice, defaulting to Yearly.");
    }

    std::string startDate, endDate;
    std::cout << "\nEnter start date (YYYY-MM-DD HH:MM:SS) or press enter to skip: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, startDate);
    std::cout << "Enter end date (YYYY-MM-DD HH:MM:SS) or press enter to skip: ";
    std::getline(std::cin, endDate);

    auto printTable = [](std::string title, const std::vector<Candlestick>& candles, std::string color) {
        std::cout << "\n" << UI::BOLD << color << "--- " << title << " ---" << UI::RESET << "\n";
        if (candles.empty()) {
            std::cout << "No data available for this selection.\n";
            return;
        }
        std::cout << std::left << std::setw(20) << "Time" 
                  << std::setw(12) << "Open" 
                  << std::setw(12) << "High" 
                  << std::setw(12) << "Low" 
                  << std::setw(12) << "Close" << "\n";
        std::cout << std::string(68, '-') << "\n";
        for (const auto& c : candles) {
            std::cout << std::left << std::setw(20) << c.timestamp 
                      << std::fixed << std::setprecision(4)
                      << std::setw(12) << c.open 
                      << std::setw(12) << c.high 
                      << std::setw(12) << c.low 
                      << std::setw(12) << c.close << "\n";
        }
    };

    std::vector<Order> askOrders = tradeSimulator.getMarketData().getOrders(product, "ask");
    std::vector<Order> bidOrders = tradeSimulator.getMarketData().getOrders(product, "bid");

    std::vector<Candlestick> asks = MarketData::computeCandlesticks(askOrders, timeframe, startDate, endDate);
    std::vector<Candlestick> bids = MarketData::computeCandlesticks(bidOrders, timeframe, startDate, endDate);

    printTable("ASKS (SELL ORDERS)", asks, UI::RED);
    printTable("BIDS (BUY ORDERS)", bids, UI::GREEN);

    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
void Menu::handleWallet() {
    UI::printHeader("WALLET BALANCE");
    Wallet& wallet = userManager.getWallet(currentUser);
    std::cout << wallet.toString() << "\n";
    
    UI::printHeader("LAST 5 TRANSACTIONS");
    std::vector<Transaction> recent = transactionManager.getRecentTransactions(currentUser, 5);
    if (recent.empty()) {
        std::cout << " No transactions found.\n";
    } else {
        std::cout << std::left << std::setw(20) << "Timestamp" 
                  << std::setw(12) << "Type" 
                  << std::setw(15) << "Product" 
                  << std::setw(10) << "Amount" 
                  << std::setw(10) << "Price" 
                  << std::setw(10) << "Balance" << "\n";
        std::cout << std::string(77, '-') << "\n";
        for (const auto& t : recent) {
            std::string typeColor = (t.type == TransactionType::deposit || t.type == TransactionType::bid) ? UI::GREEN : UI::RED;
            std::cout << std::left << std::setw(20) << t.timestamp 
                      << typeColor << std::setw(12) << Transaction::typeToString(t.type) << UI::RESET
                      << std::setw(15) << t.product 
                      << std::setw(10) << t.amount 
                      << std::setw(10) << t.price 
                      << std::setw(10) << t.balanceAfter << "\n";
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handleTransactions() {
    UI::printHeader("YOUR TRANSACTIONS");
    std::cout << "Filter by product (leave empty for all): ";
    std::string product;
    std::getline(std::cin, product);
    
    std::vector<Transaction> ts = transactionManager.getTransactions(currentUser);
    if (ts.empty()) {
        std::cout << " No transactions found.\n";
    } else {
        std::cout << std::left << std::setw(20) << "Timestamp" 
                  << std::setw(12) << "Type" 
                  << std::setw(15) << "Product" 
                  << std::setw(10) << "Amount" 
                  << std::setw(10) << "Price" 
                  << std::setw(10) << "Balance" << "\n";
        std::cout << std::string(77, '-') << "\n";
        for (const auto& t : ts) {
            if (product != "" && t.product != product) continue;
            std::string typeColor = (t.type == TransactionType::deposit || t.type == TransactionType::bid) ? UI::GREEN : UI::RED;
            std::cout << std::left << std::setw(20) << t.timestamp 
                      << typeColor << std::setw(12) << Transaction::typeToString(t.type) << UI::RESET
                      << std::setw(15) << t.product 
                      << std::setw(10) << t.amount 
                      << std::setw(10) << t.price 
                      << std::setw(10) << t.balanceAfter << "\n";
        }
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handleDeposit() {
    UI::printHeader("DEPOSIT FUNDS");
    std::cout << "Enter currency (e.g., USDT): ";
    std::string type;
    std::getline(std::cin, type);
    std::cout << "Enter amount: ";
    std::string amountStr;
    std::getline(std::cin, amountStr);
    
    try {
        double amount = std::stod(amountStr);
        if (amount <= 0) throw std::runtime_error("Amount must be positive.");
        
        Wallet& wallet = userManager.getWallet(currentUser);
        wallet.deposit(type, amount);
        userManager.saveWallets();
        
        transactionManager.saveTransaction(Transaction(
            CSVReader::getCurrentTimestamp(), currentUser, TransactionType::deposit, type, amount, 1.0, wallet.getBalance(type)
        ));
        
        UI::printSuccess("Deposit successful!");
    } catch (const std::exception& e) {
        UI::printError(e.what());
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handleWithdraw() {
    UI::printHeader("WITHDRAW FUNDS");
    std::cout << "Enter currency (e.g., USDT): ";
    std::string type;
    std::getline(std::cin, type);
    std::cout << "Enter amount: ";
    std::string amountStr;
    std::getline(std::cin, amountStr);
    
    try {
        double amount = std::stod(amountStr);
        if (amount <= 0) throw std::runtime_error("Amount must be positive.");
        
        Wallet& wallet = userManager.getWallet(currentUser);
        if (wallet.withdraw(type, amount)) {
            userManager.saveWallets();
            transactionManager.saveTransaction(Transaction(
                CSVReader::getCurrentTimestamp(), currentUser, TransactionType::withdrawal, type, amount, 1.0, wallet.getBalance(type)
            ));
            UI::printSuccess("Withdrawal successful!");
        } else {
            UI::printError("Insufficient balance.");
        }
    } catch (const std::exception& e) {
        UI::printError(e.what());
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handleStats() {
    UI::printHeader("USER STATISTICS");
    std::cout << "Enter timeframe (YYYY-MM-DD or leave empty for all): ";
    std::string timeframe;
    std::getline(std::cin, timeframe);
    
    std::vector<Transaction> all = transactionManager.getTransactions(currentUser);
    TradeSimulator::Stats stats = tradeSimulator.getStats(currentUser, all, timeframe);
    
    std::cout << " Statistics for " << (timeframe == "" ? "All Time" : timeframe) << ":\n";
    std::cout << " ------------------------------------\n";
    std::cout << " Total Asks: " << UI::BOLD << UI::YELLOW << stats.asks << UI::RESET << "\n";
    std::cout << " Total Bids: " << UI::BOLD << UI::GREEN << stats.bids << UI::RESET << "\n";
    std::cout << " Total Spent: " << UI::BOLD << UI::CYAN << stats.totalSpent << " USDT" << UI::RESET << "\n";
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
