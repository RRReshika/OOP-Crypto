#pragma once
#include <vector>
#include <string>
#include <map>
#include "RMarketData.h"
#include "RTransaction.h"
#include "RWallet.h"
#include "RCandlestick.h"

class rUsrMgr;
class rTransMgr;

// trading logic and simulation
class rSim {
public:
    rSim(rMktDat& rMkt);
    // random orders for market movement
    void rGenOrds();
    // get generated orders
    std::vector<rOrd> rGetOrds();
    // access market data
    rMktDat& rGetMkt();
    
    // user stats
    struct rPrdStats {
        int rAsks = 0;
        int rBids = 0;
    };
    struct rStats {
        int rTAsks = 0;
        int rTBids = 0;
        double rTSpnt = 0.0;
        std::map<std::string, rPrdStats> rPrdBrk;
    };
    // calculate user stats
    rStats rGetStats(std::string rUsrNm, std::vector<rTrans>& rTranss, std::string rTf = "");
    
    // simulation results
    struct rSimRes {
        std::string rProd;
        int rBids;
        int rAsks;
    };
    // Task 4: automated trading
    std::vector<rSimRes> rSimTrds(std::string rUsrNm, class rUsrMgr& rUsrMgr, class rTransMgr& rTransMgr);

private:
    rMktDat& rMkt;
    std::vector<rOrd> rOrds;
};
