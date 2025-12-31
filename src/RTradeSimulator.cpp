#include "RTradeSimulator.h"
#include <iostream>
#include <algorithm>
#include <random>
#include "RUserManager.h"
#include "RTransactionManager.h"
#include "RCSVReader.h"

rSim::rSim(rMktDat& rMkt) : rMkt(rMkt) {
    rGenOrds();
}

void rSim::rGenOrds() {
    // pick 5 random orders
    std::vector<rOrd> rAll = rMkt.rGetOrds();
    if (rAll.empty()) return;

    static std::random_device rRd;
    static std::mt19937 rG(rRd());
    std::shuffle(rAll.begin(), rAll.end(), rG);

    rOrds.clear();
    size_t rPick = std::min(rAll.size(), (size_t)5);
    for (size_t i = 0; i < rPick; ++i) {
        rOrds.push_back(rAll[i]);
    }
}

std::vector<rOrd> rSim::rGetOrds() {
    return rOrds;
}

rMktDat& rSim::rGetMkt() {
    return rMkt;
}

rSim::rStats rSim::rGetStats(std::string rUsrNm, std::vector<rTrans>& rTranss, std::string rTf) {
    rStats rS;
    for (const auto& rT : rTranss) {
        if (rT.rUsrNm == rUsrNm) {
            if (rTf != "" && rT.rTime.find(rTf) == std::string::npos) continue;
            
            if (rT.rTyp == rTransTyp::ask) {
                rS.rTAsks++;
                rS.rPrdBrk[rT.rProd].rAsks++;
            } else if (rT.rTyp == rTransTyp::bid) {
                rS.rTBids++;
                rS.rPrdBrk[rT.rProd].rBids++;
                rS.rTSpnt += rT.rAmt * rT.rPrc;
            }
        }
    }
    return rS;
}

std::vector<rSim::rSimRes> rSim::rSimTrds(std::string rUsrNm, rUsrMgr& rUsrMgr, rTransMgr& rTransMgr) {
    static std::random_device rRd;
    static std::mt19937 rGen(rRd());
    std::uniform_real_distribution<double> rSprdD(0.001, 0.01);
    std::uniform_real_distribution<double> rAmtD(0.1, 2.0);

    std::vector<std::string> rPrds = rMkt.rGetProds();
    rWlt& rW = rUsrMgr.rGetWlt(rUsrNm);
    std::vector<rSimRes> rRes;

    for (const auto& rP : rPrds) {
        double rClP = rMkt.rLastPrc(rP);
        if (rClP <= 0) continue;

        std::string rBase = rP.substr(0, rP.find('/'));
        std::string rQuote = rP.substr(rP.find('/') + 1);

        int rAsksC = 0;
        int rBidsC = 0;

        for (int i = 0; i < 5; ++i) {
            // sell
            double rASprd = rSprdD(rGen);
            double rAPrc = rClP * (1.0 + rASprd);
            double rAAmt = rAmtD(rGen);
            
            if (rW.rRemCur(rBase, rAAmt)) {
                rTransMgr.rSavTrans(rTrans(
                    rCSV::rTime(), rUsrNm, rTransTyp::ask, rP, rAAmt, rAPrc, rW.rGetBal(rBase)
                ));
                rAsksC++;
            }

            // buy
            double rBSprd = rSprdD(rGen);
            double rBPrc = rClP * (1.0 - rBSprd);
            double rBAmt = rAmtD(rGen);
            double rCost = rBAmt * rBPrc;

            if (rW.rRemCur(rQuote, rCost)) {
                rTransMgr.rSavTrans(rTrans(
                    rCSV::rTime(), rUsrNm, rTransTyp::bid, rP, rBAmt, rBPrc, rW.rGetBal(rQuote)
                ));
                rBidsC++;
            }
        }
        rRes.push_back({rP, rBidsC, rAsksC});
    }
    rUsrMgr.rSavWlts();
    return rRes;
}
