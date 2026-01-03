#pragma once
#include <string>
#include "RCandlestick.h"

enum class rTrend {
    Bullish,
    Bearish,
    Neutral
};

enum class rVolatility {
    Low,
    Medium,
    High
};

// analyze trends and volatility
class rTrendAnlz {
public:
    // trend stuff
    static rTrend rAnalyze(const rCandle& rCndl);
    static std::string rGetDesc(rTrend rT);
    static std::string rGetDetail(const rCandle& rCndl);
    static double rGetStrength(const rCandle& rCndl);
    static bool rIsStrong(const rCandle& rCndl, double rThresh = 2.0);
    
    // volatility stuff
    static double rCalcVolScore(const rCandle& rCndl);  // formula: (high - low) / avg
    static rVolatility rGetVolLevel(const rCandle& rCndl);
    static std::string rGetVolDesc(rVolatility rV);
    static std::string rGetVolDetail(const rCandle& rCndl);
};
