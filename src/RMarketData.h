#pragma once
#include <vector>
#include <string>
#include "Candlestick.h"

struct Order {
    std::string timestamp;
    std::string product;
    std::string type;
    double price;
    double amount;
};

class MarketData {
public:
    MarketData(std::string filename);
    std::vector<Order> getOrders();
    std::vector<Order> getOrders(std::string product, std::string type);
    std::vector<std::string> getProducts();
    std::vector<Candlestick> generateCandlesticks(std::string product, std::string timeframe, std::string orderType);
    static std::vector<Candlestick> computeCandlesticks(const std::vector<Order>& orders, std::string timeframe, std::string startDate = "", std::string endDate = "");
    static std::string getBucketKey(std::string timestamp, std::string timeframe);
    double getMostRecentClosePrice(std::string product);

private:
    std::vector<Order> orders;
    std::vector<std::string> products;
};
