#pragma once
#include <string>
#include <map>
#include <vector>

class Wallet {
public:
    Wallet();
    void insertCurrency(std::string type, double amount);
    bool removeCurrency(std::string type, double amount);
    bool containsCurrency(std::string type, double amount);
    double getBalance(std::string type);
    std::string toString();

    std::map<std::string, double> currencies;
};
