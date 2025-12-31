#include "RMarketData.h"
#include "RCSVReader.h"
#include <fstream>
#include <iostream>
#include <set>

rMktDat::rMktDat(std::string rFn) {
    // load CSV data
    std::vector<std::vector<std::string>> rRws = rCSV::rRdCSV(rFn);
    std::set<std::string> rProdS;

    for (size_t i = 1; i < rRws.size(); ++i) {
        const auto& rTks = rRws[i];
        if (rTks.size() == 5) {
            rOrd rO;
            rO.rTime = rTks[0];
            rO.rProd = rTks[1];
            rO.rType = rTks[2];
            rO.rPrc = std::stod(rTks[3]);
            rO.rAmt = std::stod(rTks[4]);
            rOrds.push_back(rO);
            rProdS.insert(rO.rProd);
        }
    }

    for (const auto& rP : rProdS) {
        rProds.push_back(rP);
    }
}

std::vector<rOrd> rMktDat::rGetOrds() {
    return rOrds;
}

std::vector<rOrd> rMktDat::rGetOrds(std::string rProd, std::string rType) {
    std::vector<rOrd> rFltrd;
    for (const auto& rO : rOrds) {
        if (rO.rProd == rProd && rO.rType == rType) {
            rFltrd.push_back(rO);
        }
    }
    return rFltrd;
}

std::vector<std::string> rMktDat::rGetProds() {
    return rProds;
}

#include <map>
#include <algorithm>

std::string rMktDat::rBktKy(std::string rTime, std::string rTf) {
    if (rTf == "yearly") return rTime.substr(0, 4);
    if (rTf == "monthly") return rTime.substr(0, 7);
    if (rTf == "daily") return rTime.substr(0, 10);
    return rTime;
}

std::vector<rCandle> rMktDat::rGenCndls(std::string rProd, std::string rTf, std::string rType) {
    std::vector<rOrd> rFltrd = rGetOrds(rProd, rType);
    return rCompCndls(rFltrd, rTf);
}

std::vector<rCandle> rMktDat::rCompCndls(const std::vector<rOrd>& rOrds, std::string rTf, std::string rSt, std::string rEn) {
    std::vector<rCandle> rCndls;
    if (rOrds.empty()) return rCndls;

    std::vector<rOrd> rFltrd = rOrds;

    // date range filtering
    if (!rSt.empty() || !rEn.empty()) {
        rFltrd.erase(std::remove_if(rFltrd.begin(), rFltrd.end(), [&](const rOrd& rO) {
            if (!rSt.empty() && rO.rTime < rSt) return true;
            if (!rEn.empty() && rO.rTime > rEn) return true;
            return false;
        }), rFltrd.end());
    }

    if (rFltrd.empty()) return rCndls;

    // sort by timestamp
    std::sort(rFltrd.begin(), rFltrd.end(), [](const rOrd& a, const rOrd& b) {
        return a.rTime < b.rTime;
    });

    std::map<std::string, std::vector<double>> rGrpd;
    std::vector<std::string> rBktOrd;

    for (const auto& rO : rFltrd) {
        std::string rKy = rBktKy(rO.rTime, rTf);
        if (rGrpd.count(rKy) == 0) {
            rBktOrd.push_back(rKy);
        }
        rGrpd[rKy].push_back(rO.rPrc);
    }

    for (const auto& rTm : rBktOrd) {
        const auto& rPrcs = rGrpd[rTm];
        double rOp = rPrcs.front();
        double rCl = rPrcs.back();
        double rHi = *std::max_element(rPrcs.begin(), rPrcs.end());
        double rLo = *std::min_element(rPrcs.begin(), rPrcs.end());
        rCndls.push_back(rCandle(rTm, rOp, rHi, rLo, rCl));
    }

    return rCndls;
}

double rMktDat::rLastPrc(std::string rProd) {
    std::vector<rOrd> rPrdOrds;
    for (const auto& rO : rOrds) {
        if (rO.rProd == rProd) rPrdOrds.push_back(rO);
    }
    if (rPrdOrds.empty()) return 0.0;
    
    // sort by timestamp
    std::sort(rPrdOrds.begin(), rPrdOrds.end(), [](const rOrd& a, const rOrd& b) {
        return a.rTime < b.rTime;
    });
    
    return rPrdOrds.back().rPrc;
}
