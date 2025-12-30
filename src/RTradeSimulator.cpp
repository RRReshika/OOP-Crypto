#include "RTradeSimulator.h"
#include <iostream>
#include <algorithm>
#include <random>
#include "RUserManager.h"
#include "RTransactionManager.h"
#include "RCSVReader.h"

TradeSimulator::TradeSimulator(MarketData& marketData) : marketData(marketData) {
    generateOrders();
}

void TradeSimulator::generateOrders() {
    // For simplicity, just pick some orders from market data
    std::vector<Order> allOrders = marketData.getOrders();
    if (allOrders.empty()) return;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allOrders.begin(), allOrders.end(), g);

    latestOrders.clear();
    size_t numToPick = std::min(allOrders.size(), (size_t)5);
    for (size_t i = 0; i < numToPick; ++i) {
        latestOrders.push_back(allOrders[i]);
    }
}

std::vector<Order> TradeSimulator::getLatestOrders() {
    return latestOrders;
}

MarketData& TradeSimulator::getMarketData() {
    return marketData;
}

TradeSimulator::Stats TradeSimulator::getStats(std::string username, std::vector<Transaction>& transactions, std::string timeframe) {
    Stats s;
    for (const auto& t : transactions) {
        if (t.username == username) {
            if (timeframe != "" && t.timestamp.find(timeframe) == std::string::npos) continue;
            
            if (t.type == TransactionType::ask) {
                s.totalAsks++;
                s.productBreakdown[t.product].asks++;
            } else if (t.type == TransactionType::bid) {
                s.totalBids++;
                s.productBreakdown[t.product].bids++;
                s.totalSpent += t.amount * t.price;
            }
        }
    }
    return s;
}

std::vector<TradeSimulator::SimulationResult> TradeSimulator::simulateTradingActivities(std::string username, UserManager& userManager, TransactionManager& transactionManager) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> spreadDis(0.001, 0.01);
    std::uniform_real_distribution<double> amountDis(0.1, 2.0);

    std::vector<std::string> products = marketData.getProducts();
    Wallet& wallet = userManager.getWallet(username);
    std::vector<SimulationResult> results;

    for (const auto& product : products) {
        double closePrice = marketData.getMostRecentClosePrice(product);
        if (closePrice <= 0) continue;

        std::string base = product.substr(0, product.find('/'));
        std::string quote = product.substr(product.find('/') + 1);

        int asksCreated = 0;
        int bidsCreated = 0;

        for (int i = 0; i < 5; ++i) {
            // Simulate Ask (Sell)
            double askSpread = spreadDis(gen);
            double askPrice = closePrice * (1.0 + askSpread);
            double askAmount = amountDis(gen);
            
            if (wallet.removeCurrency(base, askAmount)) {
                transactionManager.saveTransaction(Transaction(
                    CSVReader::getCurrentTimestamp(), username, TransactionType::ask, product, askAmount, askPrice, wallet.getBalance(base)
                ));
                asksCreated++;
            }

            // Simulate Bid (Buy)
            double bidSpread = spreadDis(gen);
            double bidPrice = closePrice * (1.0 - bidSpread);
            double bidAmount = amountDis(gen);
            double totalCost = bidAmount * bidPrice;

            if (wallet.removeCurrency(quote, totalCost)) {
                transactionManager.saveTransaction(Transaction(
                    CSVReader::getCurrentTimestamp(), username, TransactionType::bid, product, bidAmount, bidPrice, wallet.getBalance(quote)
                ));
                bidsCreated++;
            }
        }
        results.push_back({product, bidsCreated, asksCreated});
    }
    userManager.saveWallets();
    return results;
}
