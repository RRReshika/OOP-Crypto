#include "RWallet.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Wallet::Wallet() {}

void Wallet::insertCurrency(std::string type, double amount) {
    if (amount < 0) throw std::invalid_argument("Amount cannot be negative");
    if (currencies.count(type) == 0) currencies[type] = 0;
    currencies[type] += amount;
}

bool Wallet::removeCurrency(std::string type, double amount) {
    if (amount < 0) throw std::invalid_argument("Amount cannot be negative");
    if (currencies.count(type) == 0 || currencies[type] < amount) return false;
    currencies[type] -= amount;
    return true;
}

bool Wallet::containsCurrency(std::string type, double amount) {
    if (currencies.count(type) == 0) return false;
    return currencies[type] >= amount;
}

double Wallet::getBalance(std::string type) {
    if (currencies.count(type) == 0) return 0;
    return currencies[type];
}

std::string Wallet::toString() {
    std::stringstream ss;
    ss << std::left << std::setw(15) << "Currency" << std::setw(15) << "Balance" << "\n";
    ss << std::string(30, '-') << "\n";
    for (auto const& [type, amount] : currencies) {
        ss << std::left << std::setw(15) << type 
           << std::fixed << std::setprecision(4) << std::setw(15) << amount << "\n";
    }
    return ss.str();
}
