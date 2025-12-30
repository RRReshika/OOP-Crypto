#pragma once
#include <vector>
#include <string>
#include "RTransaction.h"

class TransactionManager {
public:
    TransactionManager(std::string filename);
    void saveTransaction(Transaction transaction);
    std::vector<Transaction> getTransactions(std::string username);
    std::vector<Transaction> getRecentTransactions(std::string username, int count = 5, std::string product = "");

private:
    std::string filename;
    std::vector<Transaction> transactions;
    void loadTransactions();
};
