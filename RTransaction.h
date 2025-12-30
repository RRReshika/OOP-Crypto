#pragma once
#include <string>

enum class TransactionType {
    deposit,
    withdrawal,
    ask,
    bid
};

class Transaction {
public:
    Transaction(std::string timestamp, std::string username, TransactionType type, std::string product, double amount, double price, double balanceAfter);

    std::string timestamp;
    std::string username;
    TransactionType type;
    std::string product;
    double amount;
    double price;
    double balanceAfter;

    static std::string typeToString(TransactionType type);
    static TransactionType stringToType(std::string typeStr);
};
