#include "RTransactionManager.h"
#include "RCSVReader.h"
#include <fstream>
#include <iostream>

TransactionManager::TransactionManager(std::string filename) : filename(filename) {
    loadTransactions();
}

void TransactionManager::saveTransaction(Transaction transaction) {
    transactions.push_back(transaction);
    CSVReader::appendRow(filename, {
        transaction.timestamp,
        transaction.username,
        Transaction::typeToString(transaction.type),
        transaction.product,
        std::to_string(transaction.amount),
        std::to_string(transaction.price),
        std::to_string(transaction.balanceAfter)
    });
}

std::vector<Transaction> TransactionManager::getTransactions(std::string username) {
    std::vector<Transaction> userTransactions;
    for (const auto& t : transactions) {
        if (t.username == username) {
            userTransactions.push_back(t);
        }
    }
    return userTransactions;
}

void TransactionManager::loadTransactions() {
    std::vector<std::vector<std::string>> rows = CSVReader::readCSV(filename);
    for (const auto& tokens : rows) {
        if (tokens.size() == 7) {
            transactions.push_back(Transaction(
                tokens[0], tokens[1], Transaction::stringToType(tokens[2]),
                tokens[3], std::stod(tokens[4]), std::stod(tokens[5]), std::stod(tokens[6])
            ));
        }
    }
}
