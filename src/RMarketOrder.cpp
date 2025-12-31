#include "RMarketOrder.h"

// constructor to set all the order details
rMktOrd::rMktOrd(std::string rTime, std::string rProd, std::string rType, double rPrc, double rAmt)
    : rTime(rTime), rProd(rProd), rType(rType), rPrc(rPrc), rAmt(rAmt) {}
