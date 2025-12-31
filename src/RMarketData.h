#pragma once
#include <vector>
#include <string>
#include "RCandlestick.h"

// simple struct to hold order info
struct rOrd {
    std::string rTime;
    std::string rProd;
    std::string rType;
    double rPrc;
    double rAmt;
};

// manages market data from CSV
class rMktDat {
public:
    rMktDat(std::string rFn);
    // get all orders
    std::vector<rOrd> rGetOrds();
    // get orders filtered by product and type
    std::vector<rOrd> rGetOrds(std::string rProd, std::string rType);
    // get list of unique products
    std::vector<std::string> rGetProds();
    // generate candles for a product
    std::vector<rCandle> rGenCndls(std::string rProd, std::string rTf, std::string rType);
    // calculate candles from orders
    static std::vector<rCandle> rCompCndls(const std::vector<rOrd>& rOrds, std::string rTf, std::string rSt = "", std::string rEn = "");
    // get bucket key
    static std::string rBktKy(std::string rTime, std::string rTf);
    // get last close price
    double rLastPrc(std::string rProd);

private:
    std::vector<rOrd> rOrds;
    std::vector<std::string> rProds;
};
