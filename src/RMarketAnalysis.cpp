#include "RMarketAnalysis.h"
#include <algorithm>
#include <iostream>

std::vector<rCandle> rMktAnls::rCompCndl(
    const std::vector<rMktOrd>& rOrds,
    std::string rProd,
    std::string rType,
    std::string rTf
) {
    // group orders into buckets
    std::map<std::string, std::vector<rMktOrd>> rBkts;

    for (const auto& rOrd : rOrds) {
        if (rOrd.rProd == rProd && rOrd.rType == rType) {
            std::string rKy = rTrunTime(rOrd.rTime, rTf);
            rBkts[rKy].push_back(rOrd);
        }
    }

    std::vector<rCandle> rRes;
    for (auto& rPr : rBkts) {
        std::string rDt = rPr.first;
        std::vector<rMktOrd>& rBktOrds = rPr.second;

        // sort by timestamp
        std::sort(rBktOrds.begin(), rBktOrds.end(), [](const rMktOrd& a, const rMktOrd& b) {
            return a.rTime < b.rTime;
        });

        double rOp = rBktOrds.front().rPrc;
        double rCl = rBktOrds.back().rPrc;
        double rHi = rBktOrds[0].rPrc;
        double rLo = rBktOrds[0].rPrc;

        for (const auto& rO : rBktOrds) {
            if (rO.rPrc > rHi) rHi = rO.rPrc;
            if (rO.rPrc < rLo) rLo = rO.rPrc;
        }

        rRes.push_back(rCandle(rDt, rOp, rHi, rLo, rCl));
    }

    // sort by date
    std::sort(rRes.begin(), rRes.end(), [](const rCandle& a, const rCandle& b) {
        return a.rDt < b.rDt;
    });

    return rRes;
}

std::string rMktAnls::rTrunTime(std::string rTime, std::string rTf) {
    if (rTf == "daily") {
        return rTime.substr(0, 10);
    } else if (rTf == "monthly") {
        return rTime.substr(0, 7);
    } else if (rTf == "yearly") {
        return rTime.substr(0, 4);
    }
    return rTime;
}
