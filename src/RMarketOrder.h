#pragma once
#include <string>

// represents a single order in the market
class rMktOrd {
public:
    rMktOrd(std::string rTime, std::string rProd, std::string rType, double rPrc, double rAmt);

    std::string rTime; // when it happened
    std::string rProd; // which crypto
    std::string rType; // bid or ask
    double rPrc;       // price
    double rAmt;       // amount
};
