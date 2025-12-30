#include "RMarketAnalysis.h"
#include <algorithm>
#include <iostream>

std::vector<Candlestick> RMarketAnalysis::computeCandlesticks(
    const std::vector<MarketOrder>& orders,
    std::string product,
    std::string orderType,
    std::string timeframe
) {
    std::map<std::string, std::vector<MarketOrder>> buckets;

    for (const auto& order : orders) {
        if (order.product == product && order.orderType == orderType) {
            std::string key = truncateTimestamp(order.timestamp, timeframe);
            buckets[key].push_back(order);
        }
    }

    std::vector<Candlestick> results;
    for (auto& pair : buckets) {
        std::string date = pair.first;
        std::vector<MarketOrder>& bucketOrders = pair.second;

        // Sort by timestamp to ensure open/close are correct
        std::sort(bucketOrders.begin(), bucketOrders.end(), [](const MarketOrder& a, const MarketOrder& b) {
            return a.timestamp < b.timestamp;
        });

        double open = bucketOrders.front().price;
        double close = bucketOrders.back().price;
        double high = bucketOrders[0].price;
        double low = bucketOrders[0].price;

        for (const auto& o : bucketOrders) {
            if (o.price > high) high = o.price;
            if (o.price < low) low = o.price;
        }

        results.push_back(Candlestick(date, open, high, low, close));
    }

    // Sort results by date
    std::sort(results.begin(), results.end(), [](const Candlestick& a, const Candlestick& b) {
        return a.date < b.date;
    });

    return results;
}

std::string RMarketAnalysis::truncateTimestamp(std::string timestamp, std::string timeframe) {
    if (timeframe == "daily") {
        return timestamp.substr(0, 10); // YYYY-MM-DD
    } else if (timeframe == "monthly") {
        return timestamp.substr(0, 7);  // YYYY-MM
    } else if (timeframe == "yearly") {
        return timestamp.substr(0, 4);  // YYYY
    }
    return timestamp;
}
