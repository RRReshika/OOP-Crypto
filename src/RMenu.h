#pragma once
#include "RUserManager.h"
#include "RTransactionManager.h"
#include "RTradeSimulator.h"

class Menu {
public:
    Menu(UserManager& userManager, TransactionManager& transactionManager, TradeSimulator& tradeSimulator);
    void init();

private:
    UserManager& userManager;
    TransactionManager& transactionManager;
    TradeSimulator& tradeSimulator;
    std::string currentUser;

    void printStatusBar();
    int getUserOption();
    void processOption(int option);

    void handleRegister();
    void handleLogin();
    void handleExchangeStats();
    void handleMakeOffer();
    void handleMakeBid();
    void handleWallet();
    void handleTransactions();
    void handleDeposit();
    void handleWithdraw();
    void handleStats();
    void handleSimulateTrading();
    void handleContinue();
    void handleMarketAnalysis();
    void handleForgotUsername();
    void handlePasswordReset();
    std::string promptProductSelection();

    // Validation helpers
    bool isValidProduct(const std::string& product);
    bool isValidTimeframe(const std::string& timeframe);
    bool isValidDate(const std::string& date);
};
