#include "RTradeSimulator.h"
#include <iostream>
#include <algorithm>
#include <random>

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
