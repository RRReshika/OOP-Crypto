#include "RCandlestick.h"

Candlestick::Candlestick(std::string timestamp, double open, double high, double low, double close)
    : timestamp(timestamp), open(open), high(high), low(low), close(close) {}
