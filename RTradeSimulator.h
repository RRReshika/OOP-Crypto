#pragma once
#include <vector>
#include <string>
#include "RMarketData.h"
#include "RTransaction.h"

class TradeSimulator {
public:
    TradeSimulator(MarketData& marketData);
    void generateOrders();
    std::vector<Order> getLatestOrders();
    MarketData& getMarketData();
    
    struct Stats {
        int asks;
        int bids;
        double totalSpent;
    };
    Stats getStats(std::string username, std::vector<Transaction>& transactions, std::string timeframe = "");

private:
    MarketData& marketData;
    std::vector<Order> latestOrders;
};
