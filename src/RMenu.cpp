#include "RMenu.h"
#include "RCSVReader.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include "RUIUtils.h"
#include "RMarketAnalysis.h"

Menu::Menu(UserManager& userManager, TransactionManager& transactionManager, TradeSimulator& tradeSimulator)
    : userManager(userManager), transactionManager(transactionManager), tradeSimulator(tradeSimulator), currentUser("") {}

void Menu::init() {
    while (true) {
        if (currentUser == "") {
            std::vector<std::string> options = {"Register", "Login", "Exit"};
            int choice = UI::interactiveMenu("CRYPTO TOOLKIT - WELCOME", options);
            
            if (choice == 0) handleRegister();
            else if (choice == 1) handleLogin();
            else if (choice == 2) break;
        } else {
            std::vector<std::string> options = {
                "Market Analysis (Candlesticks)",
                "Wallet (Deposit/Withdraw)",
                "View Transactions",
                "View Stats",
                "Simulate Trades (Task 4)",
                "Logout"
            };
            
            printStatusBar();
            int choice = UI::interactiveMenu("CRYPTO TOOLKIT - MAIN MENU", options);
            
            if (choice == 5) {
                currentUser = ""; // Logout
                UI::printSuccess("Logged out successfully.");
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
            } else {
                processOption(choice + 1);
            }
        }
    }
}

void Menu::printStatusBar() {
    Wallet& wallet = userManager.getWallet(currentUser);
    std::string balanceStr = "Balance: " + std::to_string(wallet.getBalance("USDT")) + " USDT";
    
    std::cout << UI::BG_BLUE << UI::WHITE << UI::BOLD;
    std::cout << " User: " << currentUser << " | " << balanceStr;
    std::cout << std::string(std::max(0, 40 - (int)currentUser.length() - (int)balanceStr.length() - 8), ' ');
    std::cout << UI::RESET << "\n";
}

// Removed printMenu() as it's replaced by interactiveMenu

int Menu::getUserOption() {
    int option = -1;
    std::string line;
    // Step 5.2: Use getline(cin, line) for all inputs
    if (std::getline(std::cin, line)) {
        if (line.empty()) return -1;
        try {
            // Step 5.2: Validate numeric using try/catch with stoi
            option = std::stoi(line);
        } catch (const std::invalid_argument& e) {
            option = -1; // Not a number
        } catch (const std::out_of_range& e) {
            option = -1; // Number too large
        }
    }
    return option;
}

std::string Menu::promptProductSelection() {
    std::vector<std::string> products = tradeSimulator.getMarketData().getProducts();
    int choice = UI::interactiveMenu("SELECT PRODUCT", products);
    return products[choice];
}

void Menu::processOption(int option) {
    switch (option) {
        case 1: handleMarketAnalysis(); break;
        case 2: {
            UI::printHeader("WALLET MANAGEMENT");
            Wallet& wallet = userManager.getWallet(currentUser);
            std::cout << "Current Balance:\n" << wallet.toString() << "\n";
            std::cout << "------------------------------------\n";
            std::cout << " 1: Deposit\n";
            std::cout << " 2: Withdraw\n";
            std::cout << " 0: Back\n";
            std::cout << "Choice: ";
            int sub = getUserOption();
            if (sub == 1) handleDeposit();
            else if (sub == 2) handleWithdraw();
            break;
        }
        case 3: handleTransactions(); break;
        case 4: handleStats(); break;
        case 5: handleSimulateTrading(); break;
        case 0: break; // Logout handled in init()
        default: std::cout << "[!] Invalid option\n"; break;
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
        UI::printWarning("Invalid product selection.");
        return;
    }

    double amount, price;
    // Step 5.2: Validate numeric amount using try/catch and stod
    std::cout << "Enter amount to sell: ";
    std::string amountStr;
    std::getline(std::cin, amountStr);
    try { 
        amount = std::stod(amountStr); 
        if (amount <= 0) throw std::runtime_error("Amount must be positive.");
    } catch (...) { 
        UI::printError("Invalid amount. Please enter a positive number."); 
        return; 
    }

    // Step 5.2: Validate numeric price using try/catch and stod
    std::cout << "Enter price: ";
    std::string priceStr;
    std::getline(std::cin, priceStr);
    try { 
        price = std::stod(priceStr); 
        if (price <= 0) throw std::runtime_error("Price must be positive.");
    } catch (...) { 
        UI::printError("Invalid price. Please enter a positive number."); 
        return; 
    }

    Wallet& wallet = userManager.getWallet(currentUser);
    std::string base = product.substr(0, product.find('/'));
    
    if (wallet.removeCurrency(base, amount)) {
        transactionManager.saveTransaction(Transaction(CSVReader::getCurrentTimestamp(), currentUser, TransactionType::ask, product, amount, price, wallet.getBalance(base)));
        userManager.saveWallets();
        UI::printSuccess("Offer placed for " + std::to_string(amount) + " " + base + " at " + std::to_string(price) + ".");
    } else {
        UI::printError("Insufficient balance. You need " + std::to_string(amount) + " " + base + " but only have " + std::to_string(wallet.getBalance(base)) + ".");
    }
}

void Menu::handleMakeBid() {
    std::string product = promptProductSelection();
    if (product == "") {
        UI::printWarning("Invalid product selection.");
        return;
    }

    double amount, price;
    // Step 5.2: Validate numeric amount using try/catch and stod
    std::cout << "Enter amount to buy: ";
    std::string amountStr;
    std::getline(std::cin, amountStr);
    try { 
        amount = std::stod(amountStr); 
        if (amount <= 0) throw std::runtime_error("Amount must be positive.");
    } catch (...) { 
        UI::printError("Invalid amount. Please enter a positive number."); 
        return; 
    }

    // Step 5.2: Validate numeric price using try/catch and stod
    std::cout << "Enter price: ";
    std::string priceStr;
    std::getline(std::cin, priceStr);
    try { 
        price = std::stod(priceStr); 
        if (price <= 0) throw std::runtime_error("Price must be positive.");
    } catch (...) { 
        UI::printError("Invalid price. Please enter a positive number."); 
        return; 
    }

    Wallet& wallet = userManager.getWallet(currentUser);
    std::string quote = product.substr(product.find('/') + 1);
    double totalCost = amount * price;

    if (wallet.removeCurrency(quote, totalCost)) {
        transactionManager.saveTransaction(Transaction(CSVReader::getCurrentTimestamp(), currentUser, TransactionType::bid, product, amount, price, wallet.getBalance(quote)));
        userManager.saveWallets();
        UI::printSuccess("Bid placed for " + std::to_string(amount) + " " + product.substr(0, product.find('/')) + " at total cost of " + std::to_string(totalCost) + " " + quote + ".");
    } else {
        UI::printError("Insufficient balance. Total cost is " + std::to_string(totalCost) + " " + quote + " but you only have " + std::to_string(wallet.getBalance(quote)) + ".");
    }
}


void Menu::handleContinue() {
    tradeSimulator.generateOrders();
    UI::printSuccess("Market updated with new orders for the next step.");
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

#include "MarketOrder.h"

void Menu::handleMarketAnalysis() {
    std::string product = promptProductSelection();
    if (product == "" || !isValidProduct(product)) {
        UI::printWarning("Invalid product selection.");
        return;
    }

    std::vector<std::string> timeframes = {"Daily", "Monthly", "Yearly"};
    int tfChoice = UI::interactiveMenu("SELECT TIMEFRAME", timeframes);
    std::string timeframe = (tfChoice == 0) ? "daily" : (tfChoice == 1) ? "monthly" : "yearly";

    // Load all market orders
    std::vector<MarketOrder> orders = CSVReader::readMarketOrders("data/20200317.csv");

    // Compute candlesticks using RMarketAnalysis
    std::vector<Candlestick> asks = RMarketAnalysis::computeCandlesticks(orders, product, "ask", timeframe);
    std::vector<Candlestick> bids = RMarketAnalysis::computeCandlesticks(orders, product, "bid", timeframe);

    auto printTable = [](std::string title, const std::vector<Candlestick>& candles, std::string color) {
        std::cout << "\n" << UI::BOLD << color << " " << UI::TL << std::string(68, UI::H[0]) << UI::TR << UI::RESET << "\n";
        std::cout << UI::BOLD << color << " " << UI::V << " " << title << std::string(67 - title.length(), ' ') << UI::V << UI::RESET << "\n";
        std::cout << UI::BOLD << color << " " << UI::ML << std::string(68, UI::H[0]) << UI::MR << UI::RESET << "\n";
        
        if (candles.empty()) {
            std::cout << " " << UI::V << " No data available for this selection." << std::string(32, ' ') << UI::V << "\n";
        } else {
            std::cout << " " << UI::V << " " << std::left << std::setw(20) << "Date" 
                      << std::setw(12) << "Open" 
                      << std::setw(12) << "High" 
                      << std::setw(12) << "Low" 
                      << std::setw(11) << "Close" << UI::V << "\n";
            std::cout << " " << UI::ML << std::string(68, UI::H[0]) << UI::MR << "\n";
            
            for (const auto& c : candles) {
                std::string trendColor = (c.close >= c.open) ? UI::GREEN : UI::RED;
                std::cout << " " << UI::V << " " << trendColor << std::left << std::setw(20) << c.date 
                          << std::fixed << std::setprecision(4)
                          << std::setw(12) << c.open 
                          << std::setw(12) << c.high 
                          << std::setw(12) << c.low 
                          << std::setw(11) << c.close << UI::RESET << UI::V << "\n";
            }
        }
        std::cout << " " << UI::BL << std::string(68, UI::H[0]) << UI::BR << "\n";
    };

    printTable("Asks Candlestick Summary", asks, UI::RED);
    printTable("Bids Candlestick Summary", bids, UI::GREEN);

    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
void Menu::handleWallet() {
    UI::printHeader("WALLET BALANCE");
    Wallet& wallet = userManager.getWallet(currentUser);
    std::cout << "Current Balance:\n" << wallet.toString() << "\n";
    
    UI::printHeader("LAST 5 TRANSACTIONS");
    std::vector<Transaction> recent = transactionManager.getRecentTransactions(currentUser, 5);
    
    auto printTransTable = [](const std::vector<Transaction>& ts) {
        std::cout << " " << UI::TL << std::string(72, UI::H[0]) << UI::TR << "\n";
        std::cout << " " << UI::V << " " << std::left << std::setw(20) << "Date & Time" 
                  << std::setw(12) << "Type" 
                  << std::setw(15) << "Product" 
                  << std::setw(10) << "Amount" 
                  << std::setw(11) << "Balance After" << UI::V << "\n";
        std::cout << " " << UI::ML << std::string(72, UI::H[0]) << UI::MR << "\n";
        
        if (ts.empty()) {
            std::cout << " " << UI::V << " No transactions found." << std::string(48, ' ') << UI::V << "\n";
        } else {
            for (const auto& t : ts) {
                std::string typeColor = (t.type == TransactionType::deposit || t.type == TransactionType::bid) ? UI::GREEN : UI::RED;
                std::cout << " " << UI::V << " " << std::left << std::setw(20) << t.timestamp 
                          << typeColor << std::setw(12) << Transaction::typeToString(t.type) << UI::RESET
                          << std::setw(15) << t.product 
                          << std::setw(10) << t.amount 
                          << std::setw(11) << t.balanceAfter << UI::V << "\n";
            }
        }
        std::cout << " " << UI::BL << std::string(72, UI::H[0]) << UI::BR << "\n";
    };

    printTransTable(recent);
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handleTransactions() {
    UI::printHeader("YOUR TRANSACTIONS");
    std::cout << "Filter by product (leave empty for all): ";
    std::string product;
    std::getline(std::cin, product);
    
    std::vector<Transaction> ts = transactionManager.getTransactions(currentUser);
    std::vector<Transaction> filtered;
    for (const auto& t : ts) {
        if (product == "" || t.product == product) filtered.push_back(t);
    }

    auto printTransTable = [](const std::vector<Transaction>& ts) {
        std::cout << " " << UI::TL << std::string(72, UI::H[0]) << UI::TR << "\n";
        std::cout << " " << UI::V << " " << std::left << std::setw(20) << "Date & Time" 
                  << std::setw(12) << "Type" 
                  << std::setw(15) << "Product" 
                  << std::setw(10) << "Amount" 
                  << std::setw(11) << "Balance After" << UI::V << "\n";
        std::cout << " " << UI::ML << std::string(72, UI::H[0]) << UI::MR << "\n";
        
        if (ts.empty()) {
            std::cout << " " << UI::V << " No transactions found." << std::string(48, ' ') << UI::V << "\n";
        } else {
            for (const auto& t : ts) {
                std::string typeColor = (t.type == TransactionType::deposit || t.type == TransactionType::bid) ? UI::GREEN : UI::RED;
                std::cout << " " << UI::V << " " << std::left << std::setw(20) << t.timestamp 
                          << typeColor << std::setw(12) << Transaction::typeToString(t.type) << UI::RESET
                          << std::setw(15) << t.product 
                          << std::setw(10) << t.amount 
                          << std::setw(11) << t.balanceAfter << UI::V << "\n";
            }
        }
        std::cout << " " << UI::BL << std::string(72, UI::H[0]) << UI::BR << "\n";
    };

    printTransTable(filtered);
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handleDeposit() {
    UI::printHeader("DEPOSIT FUNDS");
    // Step 5.2: Validate currency type
    std::cout << "Enter currency (e.g., USDT): ";
    std::string type;
    std::getline(std::cin, type);
    if (type.empty()) {
        UI::printError("Currency type cannot be empty.");
        return;
    }

    // Step 5.2: Validate numeric amount using try/catch and stod
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
        UI::printError("Invalid amount. Please enter a positive number.");
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handleWithdraw() {
    UI::printHeader("WITHDRAW FUNDS");
    // Step 5.2: Validate currency type
    std::cout << "Enter currency (e.g., USDT): ";
    std::string type;
    std::getline(std::cin, type);
    if (type.empty()) {
        UI::printError("Currency type cannot be empty.");
        return;
    }

    // Step 5.2: Validate numeric amount using try/catch and stod
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
        UI::printError("Invalid amount. Please enter a positive number.");
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handleStats() {
    UI::printHeader("USER STATISTICS");
    
    // Task 3: Refined stats output
    std::cout << "Enter date (YYYY-MM-DD or leave empty for all): ";
    std::string dateStr;
    std::getline(std::cin, dateStr);
    
    if (!dateStr.empty() && (dateStr.length() < 4 || dateStr.length() > 10)) {
        UI::printWarning("Invalid date format. Showing all-time stats.");
        dateStr = "";
    }
    
    std::vector<Transaction> all = transactionManager.getTransactions(currentUser);
    TradeSimulator::Stats stats = tradeSimulator.getStats(currentUser, all, dateStr);
    
    std::cout << "\n " << UI::TL << std::string(38, UI::H[0]) << UI::TR << "\n";
    std::cout << " " << UI::V << " User Activity Summary (" << (dateStr == "" ? "All Time" : dateStr) << ")" << std::string(38 - 24 - (dateStr == "" ? 8 : (int)dateStr.length()), ' ') << UI::V << "\n";
    std::cout << " " << UI::ML << std::string(38, UI::H[0]) << UI::MR << "\n";
    std::cout << " " << UI::V << " Total Asks: " << UI::BOLD << UI::YELLOW << std::setw(24) << stats.totalAsks << UI::RESET << " " << UI::V << "\n";
    std::cout << " " << UI::V << " Total Bids: " << UI::BOLD << UI::GREEN << std::setw(24) << stats.totalBids << UI::RESET << " " << UI::V << "\n";
    std::cout << " " << UI::ML << std::string(38, UI::H[0]) << UI::MR << "\n";
    
    for (const auto& pair : stats.productBreakdown) {
        std::string line = pair.first + ": A:" + std::to_string(pair.second.asks) + " B:" + std::to_string(pair.second.bids);
        std::cout << " " << UI::V << " " << line << std::string(37 - line.length(), ' ') << UI::V << "\n";
    }
    
    std::cout << " " << UI::ML << std::string(38, UI::H[0]) << UI::MR << "\n";
    std::cout << " " << UI::V << " Total Spent: " << UI::BOLD << UI::CYAN << std::setw(18) << std::fixed << std::setprecision(2) << stats.totalSpent << " USDT" << UI::RESET << " " << UI::V << "\n";
    std::cout << " " << UI::BL << std::string(38, UI::H[0]) << UI::BR << "\n";
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handleSimulateTrading() {
    UI::printHeader("SIMULATING TRADING ACTIVITIES");
    // Task 4: Refined simulation output
    std::cout << "Simulating trades for all products...\n\n";
    std::vector<TradeSimulator::SimulationResult> results = tradeSimulator.simulateTradingActivities(currentUser, userManager, transactionManager);
    
    std::cout << " " << UI::TL << std::string(38, UI::H[0]) << UI::TR << "\n";
    for (const auto& res : results) {
        std::string line = res.product + ": B:" + std::to_string(res.bidsCreated) + " A:" + std::to_string(res.asksCreated);
        std::cout << " " << UI::V << " " << line << std::string(37 - line.length(), ' ') << UI::V << "\n";
    }
    std::cout << " " << UI::BL << std::string(38, UI::H[0]) << UI::BR << "\n";
    
    UI::printSuccess("Trades saved successfully.");
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

// Step 5.2: Validation helpers implementation
bool Menu::isValidProduct(const std::string& product) {
    std::vector<std::string> products = tradeSimulator.getMarketData().getProducts();
    for (const std::string& p : products) {
        if (p == product) return true;
    }
    return false;
}

bool Menu::isValidTimeframe(const std::string& timeframe) {
    // Step 5.2: Validate timeframe (D/M/Y)
    return (timeframe == "daily" || timeframe == "monthly" || timeframe == "yearly" || 
            timeframe == "D" || timeframe == "M" || timeframe == "Y");
}

bool Menu::isValidDate(const std::string& date) {
    // Step 5.2: Validate date format (basic check: YYYY-MM-DD HH:MM:SS)
    // Format: 0123456789012345678
    //         YYYY-MM-DD HH:MM:SS
    if (date.length() != 19) return false;
    if (date[4] != '-' || date[7] != '-' || date[10] != ' ' || date[13] != ':' || date[16] != ':') return false;
    
    for (int i = 0; i < 19; ++i) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(date[i])) return false;
    }
    return true;
}
