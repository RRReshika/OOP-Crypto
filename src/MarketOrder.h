#pragma once
#include <string>

class MarketOrder {
public:
    MarketOrder(std::string timestamp, std::string product, std::string orderType, double price, double amount);

    std::string timestamp;
    std::string product;
    std::string orderType; // "bid" or "ask"
    double price;
    double amount;
};
