#include "RMarketData.h"
#include "RCSVReader.h"
#include <fstream>
#include <iostream>
#include <set>

MarketData::MarketData(std::string filename) {
    std::vector<std::vector<std::string>> rows = CSVReader::readCSV(filename);
    std::set<std::string> productSet;

    for (size_t i = 1; i < rows.size(); ++i) { // Skip header
        const auto& tokens = rows[i];
        if (tokens.size() == 5) {
            Order o;
            o.timestamp = tokens[0];
            o.product = tokens[1];
            o.type = tokens[2];
            o.price = std::stod(tokens[3]);
            o.amount = std::stod(tokens[4]);
            orders.push_back(o);
            productSet.insert(o.product);
        }
    }

    for (const auto& p : productSet) {
        products.push_back(p);
    }
}

std::vector<Order> MarketData::getOrders() {
    return orders;
}

std::vector<Order> MarketData::getOrders(std::string product, std::string type) {
    std::vector<Order> filteredOrders;
    for (const auto& o : orders) {
        if (o.product == product && o.type == type) {
            filteredOrders.push_back(o);
        }
    }
    return filteredOrders;
}

std::vector<std::string> MarketData::getProducts() {
    return products;
}

#include <map>
#include <algorithm>

std::string MarketData::getBucketKey(std::string timestamp, std::string timeframe) {
    if (timeframe == "yearly") return timestamp.substr(0, 4);
    if (timeframe == "monthly") return timestamp.substr(0, 7);
    if (timeframe == "daily") return timestamp.substr(0, 10);
    return timestamp;
}

std::vector<Candlestick> MarketData::generateCandlesticks(std::string product, std::string timeframe, std::string orderType) {
    std::vector<Order> filteredOrders = getOrders(product, orderType);
    return computeCandlesticks(filteredOrders, timeframe);
}

std::vector<Candlestick> MarketData::computeCandlesticks(const std::vector<Order>& orders, std::string timeframe, std::string startDate, std::string endDate) {
    std::vector<Candlestick> candlesticks;
    if (orders.empty()) return candlesticks;

    std::vector<Order> filtered = orders;

    // Optional date range filtering
    if (!startDate.empty() || !endDate.empty()) {
        filtered.erase(std::remove_if(filtered.begin(), filtered.end(), [&](const Order& o) {
            if (!startDate.empty() && o.timestamp < startDate) return true;
            if (!endDate.empty() && o.timestamp > endDate) return true;
            return false;
        }), filtered.end());
    }

    if (filtered.empty()) return candlesticks;

    // Sort orders by timestamp
    std::sort(filtered.begin(), filtered.end(), [](const Order& a, const Order& b) {
        return a.timestamp < b.timestamp;
    });

    std::map<std::string, std::vector<double>> groupedPrices;
    std::vector<std::string> bucketOrder;

    for (const auto& o : filtered) {
        std::string key = getBucketKey(o.timestamp, timeframe);
        if (groupedPrices.count(key) == 0) {
            bucketOrder.push_back(key);
        }
        groupedPrices[key].push_back(o.price);
    }

    for (const auto& time : bucketOrder) {
        const auto& prices = groupedPrices[time];
        double open = prices.front();
        double close = prices.back();
        double high = *std::max_element(prices.begin(), prices.end());
        double low = *std::min_element(prices.begin(), prices.end());
        candlesticks.push_back(Candlestick(time, open, high, low, close));
    }

    return candlesticks;
}
