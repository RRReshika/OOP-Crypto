#pragma once
#include <vector>
#include <string>
#include "RMarketData.h"

class TradeSimulator {
public:
    TradeSimulator(MarketData& marketData);
    void generateOrders();
    std::vector<Order> getLatestOrders();
    MarketData& getMarketData();

private:
    MarketData& marketData;
    std::vector<Order> latestOrders;
};
