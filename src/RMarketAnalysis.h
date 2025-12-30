#pragma once
#include <vector>
#include <string>
#include <map>
#include "MarketOrder.h"
#include "Candlestick.h"

class RMarketAnalysis {
public:
    static std::vector<Candlestick> computeCandlesticks(
        const std::vector<MarketOrder>& orders,
        std::string product,
        std::string orderType,
        std::string timeframe
    );

private:
    static std::string truncateTimestamp(std::string timestamp, std::string timeframe);
};
