#include "RCandlestick.h"

// constructor to set all the candle values
rCandle::rCandle(std::string rDt, double rOp, double rHi, double rLo, double rCl)
    : rDt(rDt), rOp(rOp), rHi(rHi), rLo(rLo), rCl(rCl) {}
