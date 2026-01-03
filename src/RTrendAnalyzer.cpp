#include "RTrendAnalyzer.h"
#include <cmath>
#include <sstream>
#include <iomanip>

// check if price went up or down
rTrend rTrendAnlz::rAnalyze(const rCandle& rCndl) {
    if (rCndl.rCl > rCndl.rOp) {
        return rTrend::Bullish;  // went up
    } else if (rCndl.rCl < rCndl.rOp) {
        return rTrend::Bearish;  // went down
    } else {
        return rTrend::Neutral;  // stayed same
    }
}

std::string rTrendAnlz::rGetDesc(rTrend rT) {
    switch (rT) {
        case rTrend::Bullish:
            return "Bullish";
        case rTrend::Bearish:
            return "Bearish";
        case rTrend::Neutral:
            return "Neutral";
        default:
            return "Unknown";
    }
}

std::string rTrendAnlz::rGetDetail(const rCandle& rCndl) {
    rTrend rT = rAnalyze(rCndl);
    double rStr = rGetStrength(rCndl);
    
    std::stringstream rSs;
    rSs << std::fixed << std::setprecision(2);
    
    rSs << "Market Trend: " << rGetDesc(rT);
    
    if (rT == rTrend::Bullish) {
        rSs << " (price went up)\\n";
        rSs << "   Open: " << rCndl.rOp << " | Close: " << rCndl.rCl << "\\n";
        rSs << "   Gain: +" << rStr << "%";
        if (rIsStrong(rCndl)) {
            rSs << " [Strong]";
        }
    } else if (rT == rTrend::Bearish) {
        rSs << " (price went down)\\n";
        rSs << "   Open: " << rCndl.rOp << " | Close: " << rCndl.rCl << "\\n";
        rSs << "   Loss: " << rStr << "%";
        if (rIsStrong(rCndl)) {
            rSs << " [Strong]";
        }
    } else {
        rSs << " (no change)\\n";
        rSs << "   Open: " << rCndl.rOp << " | Close: " << rCndl.rCl << "\\n";
        rSs << "   Change: 0.00%";
    }
    
    double rRange = ((rCndl.rHi - rCndl.rLo) / rCndl.rOp) * 100.0;
    rSs << "\\n   Range: " << rRange << "% ";
    rSs << "(H: " << rCndl.rHi << ", L: " << rCndl.rLo << ")";
    
    return rSs.str();
}

double rTrendAnlz::rGetStrength(const rCandle& rCndl) {
    if (rCndl.rOp == 0) return 0.0;
    return ((rCndl.rCl - rCndl.rOp) / rCndl.rOp) * 100.0;
}

bool rTrendAnlz::rIsStrong(const rCandle& rCndl, double rThresh) {
    double rStr = std::abs(rGetStrength(rCndl));
    return rStr >= rThresh;
}

// calculate volatility using (high-low)/average formula
double rTrendAnlz::rCalcVolScore(const rCandle& rCndl) {
    double rAvgPrice = (rCndl.rOp + rCndl.rCl) / 2.0;
    
    if (rAvgPrice == 0.0) return 0.0;
    
    double rRange = rCndl.rHi - rCndl.rLo;
    return rRange / rAvgPrice;
}

// put volatility into categories
rVolatility rTrendAnlz::rGetVolLevel(const rCandle& rCndl) {
    double rScore = rCalcVolScore(rCndl);
    
    // thresholds: <2% = low, 2-5% = medium, >5% = high
    if (rScore < 0.02) {
        return rVolatility::Low;
    } else if (rScore < 0.05) {
        return rVolatility::Medium;
    } else {
        return rVolatility::High;
    }
}

std::string rTrendAnlz::rGetVolDesc(rVolatility rV) {
    switch (rV) {
        case rVolatility::Low:
            return "Low";
        case rVolatility::Medium:
            return "Medium";
        case rVolatility::High:
            return "High";
        default:
            return "Unknown";
    }
}

std::string rTrendAnlz::rGetVolDetail(const rCandle& rCndl) {
    double rScore = rCalcVolScore(rCndl);
    rVolatility rVol = rGetVolLevel(rCndl);
    
    std::stringstream rSs;
    rSs << std::fixed << std::setprecision(2);
    
    rSs << "Volatility: " << rGetVolDesc(rVol);
    
    if (rVol == rVolatility::High) {
        rSs << " (big price swings)\\n";
    } else if (rVol == rVolatility::Medium) {
        rSs << " (normal trading)\\n";
    } else {
        rSs << " (stable)\\n";
    }
    
    rSs << "   Range: " << (rScore * 100.0) << "% ";
    rSs << "(High: " << rCndl.rHi << ", Low: " << rCndl.rLo << ")";
    
    return rSs.str();
}
