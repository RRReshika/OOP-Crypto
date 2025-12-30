#pragma once
#include <string>

class Candlestick {
public:
    Candlestick(std::string timestamp, double open, double high, double low, double close);

    std::string timestamp;
    double open;
    double high;
    double low;
    double close;
};
