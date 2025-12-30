#pragma once
#include <string>

class Candlestick {
public:
    Candlestick(std::string date, double open, double high, double low, double close);

    std::string date;
    double open;
    double high;
    double low;
    double close;
};
