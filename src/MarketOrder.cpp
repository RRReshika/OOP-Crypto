#include "MarketOrder.h"

MarketOrder::MarketOrder(std::string timestamp, std::string product, std::string orderType, double price, double amount)
    : timestamp(timestamp), product(product), orderType(orderType), price(price), amount(amount) {}
