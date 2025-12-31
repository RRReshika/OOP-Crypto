#pragma once
#include <string>

// stores OHLC data for a single candle
class rCandle {
public:
    rCandle(std::string rDt, double rOp, double rHi, double rLo, double rCl);

    std::string rDt; // when it happened
    double rOp;      // starting price
    double rHi;      // highest price
    double rLo;      // lowest price
    double rCl;      // final price
};
