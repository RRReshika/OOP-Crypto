#pragma once
#include <vector>
#include <string>
#include "RTransaction.h"

class TransactionManager {
public:
    TransactionManager(std::string filename);
    void saveTransaction(Transaction transaction);
    std::vector<Transaction> getTransactions(std::string username);

private:
    std::string filename;
    std::vector<Transaction> transactions;
    void loadTransactions();
};
