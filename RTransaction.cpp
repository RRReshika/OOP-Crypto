#include "RTransaction.h"

Transaction::Transaction(std::string timestamp, std::string username, TransactionType type, std::string product, double amount, double price, double balanceAfter)
    : timestamp(timestamp), username(username), type(type), product(product), amount(amount), price(price), balanceAfter(balanceAfter) {}

std::string Transaction::typeToString(TransactionType type) {
    switch (type) {
        case TransactionType::deposit: return "deposit";
        case TransactionType::withdrawal: return "withdrawal";
        case TransactionType::ask: return "ask";
        case TransactionType::bid: return "bid";
        default: return "unknown";
    }
}

TransactionType Transaction::stringToType(std::string typeStr) {
    if (typeStr == "deposit") return TransactionType::deposit;
    if (typeStr == "withdrawal") return TransactionType::withdrawal;
    if (typeStr == "ask") return TransactionType::ask;
    if (typeStr == "bid") return TransactionType::bid;
    return TransactionType::deposit; // Default
}
