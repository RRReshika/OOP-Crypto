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
            // Welcome Menu (Step 5.1)
            UI::printHeader("CRYPTO SIMULATOR - WELCOME");
            std::cout << " " << UI::BOLD << UI::YELLOW << "1:" << UI::RESET << " Register\n";
            std::cout << " " << UI::BOLD << UI::YELLOW << "2:" << UI::RESET << " Login\n";
            std::cout << " " << UI::BOLD << UI::RED << "0:" << UI::RESET << " Exit\n";
            std::cout << "\nType your choice: ";
            
            int option = getUserOption();
            if (option == 1) handleRegister();
            else if (option == 2) handleLogin();
            else if (option == 0) break;
            else {
                UI::printWarning("Invalid option. Please choose 1, 2, or 0.");
            }
        } else {
            // After Login Menu (Step 5.1)
            printMenu();
            int option = getUserOption();
            if (option == 0) {
                currentUser = ""; // Logout
                UI::printSuccess("Logged out successfully.");
            } else {
                processOption(option);
            }
        }
    }
}

void Menu::printMenu() {
    UI::printHeader("CRYPTO SIMULATOR - MAIN MENU");
    std::cout << " " << UI::BOLD << "Logged in as: " << UI::GREEN << currentUser << UI::RESET << "\n";
    std::cout << "------------------------------------\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "1:" << UI::RESET << " Market Analysis (Candlesticks)\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "2:" << UI::RESET << " Wallet (Deposit/Withdraw)\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "3:" << UI::RESET << " View Transactions\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "4:" << UI::RESET << " View Stats\n";
    std::cout << " " << UI::BOLD << UI::CYAN << "5:" << UI::RESET << " Simulate Trades (Task 4)\n";
    std::cout << " " << UI::BOLD << UI::RED << "0:" << UI::RESET << " Logout\n";
    std::cout << "------------------------------------\n";
    std::cout << "Type your choice: ";
}

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
    std::cout << "\nAvailable Products:\n";
    for (size_t i = 0; i < products.size(); ++i) {
        std::cout << i + 1 << ": " << products[i] << "\n";
    }
    std::cout << "Enter product (e.g. ETH/USDT) by number: ";
    int choice = getUserOption();
    if (choice > 0 && choice <= static_cast<int>(products.size())) {
        std::string product = products[choice - 1];
        // Step 5.2: Validate product exists
        if (isValidProduct(product)) {
            return product;
        }
    }
    return "";
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

    UI::printHeader("MARKET ANALYSIS - " + product);
    // Task 1: Refined timeframe selection
    std::cout << "Select timeframe:\n";
    std::cout << "1. Daily\n";
    std::cout << "2. Monthly\n";
    std::cout << "3. Yearly\n";
    std::cout << "Choice: ";
    
    std::string tfInput;
    std::getline(std::cin, tfInput);
    
    std::string timeframe = "";
    if (tfInput == "1" || tfInput == "daily") timeframe = "daily";
    else if (tfInput == "2" || tfInput == "monthly") timeframe = "monthly";
    else if (tfInput == "3" || tfInput == "yearly") timeframe = "yearly";
    else {
        UI::printWarning("Invalid choice, defaulting to Yearly.");
        timeframe = "yearly";
    }

    // Load all market orders
    std::vector<MarketOrder> orders = CSVReader::readMarketOrders("data/20200317.csv");

    // Compute candlesticks using RMarketAnalysis
    std::vector<Candlestick> asks = RMarketAnalysis::computeCandlesticks(orders, product, "ask", timeframe);
    std::vector<Candlestick> bids = RMarketAnalysis::computeCandlesticks(orders, product, "bid", timeframe);

    auto printTable = [](std::string title, const std::vector<Candlestick>& candles, std::string color) {
        std::cout << "\n" << UI::BOLD << color << "--- " << title << " ---" << UI::RESET << "\n";
        if (candles.empty()) {
            std::cout << "No data available for this selection.\n";
            return;
        }
        // Task 1: Refined table headers
        std::cout << std::left << std::setw(20) << "Date" 
                  << std::setw(12) << "Open" 
                  << std::setw(12) << "High" 
                  << std::setw(12) << "Low" 
                  << std::setw(12) << "Close" << "\n";
        std::cout << std::string(68, '-') << "\n";
        for (const auto& c : candles) {
            std::cout << std::left << std::setw(20) << c.date 
                      << std::fixed << std::setprecision(4)
                      << std::setw(12) << c.open 
                      << std::setw(12) << c.high 
                      << std::setw(12) << c.low 
                      << std::setw(12) << c.close << "\n";
        }
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
    if (recent.empty()) {
        std::cout << " No transactions found.\n";
    } else {
        // Task 3: Refined table headers
        std::cout << std::left << std::setw(20) << "Date & Time" 
                  << std::setw(12) << "Type" 
                  << std::setw(15) << "Product" 
                  << std::setw(10) << "Amount" 
                  << std::setw(15) << "Balance After" << "\n";
        std::cout << std::string(72, '-') << "\n";
        for (const auto& t : recent) {
            std::string typeColor = (t.type == TransactionType::deposit || t.type == TransactionType::bid) ? UI::GREEN : UI::RED;
            std::cout << std::left << std::setw(20) << t.timestamp 
                      << typeColor << std::setw(12) << Transaction::typeToString(t.type) << UI::RESET
                      << std::setw(15) << t.product 
                      << std::setw(10) << t.amount 
                      << std::setw(15) << t.balanceAfter << "\n";
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
        // Task 3: Refined table headers
        std::cout << std::left << std::setw(20) << "Date & Time" 
                  << std::setw(12) << "Type" 
                  << std::setw(15) << "Product" 
                  << std::setw(10) << "Amount" 
                  << std::setw(15) << "Balance After" << "\n";
        std::cout << std::string(72, '-') << "\n";
        for (const auto& t : ts) {
            if (product != "" && t.product != product) continue;
            std::string typeColor = (t.type == TransactionType::deposit || t.type == TransactionType::bid) ? UI::GREEN : UI::RED;
            std::cout << std::left << std::setw(20) << t.timestamp 
                      << typeColor << std::setw(12) << Transaction::typeToString(t.type) << UI::RESET
                      << std::setw(15) << t.product 
                      << std::setw(10) << t.amount 
                      << std::setw(15) << t.balanceAfter << "\n";
        }
    }
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
    
    std::cout << "\nUser Activity Summary (" << (dateStr == "" ? "All Time" : dateStr) << "):\n";
    std::cout << "------------------------------------\n";
    std::cout << "Total Asks: " << UI::BOLD << UI::YELLOW << stats.totalAsks << UI::RESET << "\n";
    std::cout << "Total Bids: " << UI::BOLD << UI::GREEN << stats.totalBids << UI::RESET << "\n";
    std::cout << "\nPer Product Breakdown:\n";
    for (const auto& pair : stats.productBreakdown) {
        std::cout << UI::BOLD << pair.first << ":" << UI::RESET << "\n";
        std::cout << "  Asks: " << pair.second.asks << "\n";
        std::cout << "  Bids: " << pair.second.bids << "\n";
    }
    std::cout << "\nTotal Money Spent (Monthly): " << UI::BOLD << UI::CYAN << stats.totalSpent << " USDT" << UI::RESET << "\n";
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void Menu::handleSimulateTrading() {
    UI::printHeader("SIMULATING TRADING ACTIVITIES");
    // Task 4: Refined simulation output
    std::cout << "Simulating trades for all products...\n\n";
    std::vector<TradeSimulator::SimulationResult> results = tradeSimulator.simulateTradingActivities(currentUser, userManager, transactionManager);
    
    for (const auto& res : results) {
        std::cout << UI::BOLD << res.product << ":" << UI::RESET << "\n";
        std::cout << "  " << res.bidsCreated << " Bids created\n";
        std::cout << "  " << res.asksCreated << " Asks created\n\n";
    }
    
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
