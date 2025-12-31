#pragma once
#include <vector>
#include <string>
#include <map>
#include "RMarketOrder.h"
#include "RCandlestick.h"

// turns orders into candlestick data
class rMktAnls {
public:
    // main function for candle math
    static std::vector<rCandle> rCompCndl(
        const std::vector<rMktOrd>& rOrds,
        std::string rProd,
        std::string rType,
        std::string rTf
    );

private:
    // helper to chop off time part of timestamp
    static std::string rTrunTime(std::string rTime, std::string rTf);
};
