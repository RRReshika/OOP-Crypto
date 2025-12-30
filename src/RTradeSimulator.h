#pragma once
#include <vector>
#include <string>
#include <map>
#include "RMarketData.h"
#include "RTransaction.h"
#include "RWallet.h"
#include "Candlestick.h"

class UserManager;
class TransactionManager;

class TradeSimulator {
public:
    TradeSimulator(MarketData& marketData);
    void generateOrders();
    std::vector<Order> getLatestOrders();
    MarketData& getMarketData();
    
    struct ProductStats {
        int asks = 0;
        int bids = 0;
    };
    struct Stats {
        int totalAsks = 0;
        int totalBids = 0;
        double totalSpent = 0.0;
        std::map<std::string, ProductStats> productBreakdown;
    };
    Stats getStats(std::string username, std::vector<Transaction>& transactions, std::string timeframe = "");
    
    struct SimulationResult {
        std::string product;
        int bidsCreated;
        int asksCreated;
    };
    std::vector<SimulationResult> simulateTradingActivities(std::string username, class UserManager& userManager, class TransactionManager& transactionManager);

private:
    MarketData& marketData;
    std::vector<Order> latestOrders;
};
