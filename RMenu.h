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

    void printMenu();
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
    void handleContinue();
    void handleMarketAnalysis();
    void handleForgotUsername();
    void handlePasswordReset();
    std::string promptProductSelection();
};
