#pragma once
#include <vector>
#include <string>
#include "RTransaction.h"

// manages transactions and saves to file
class rTransMgr {
public:
    rTransMgr(std::string rFn);
    // save new transaction
    void rSavTrans(rTrans rT);
    // get all user transactions
    std::vector<rTrans> rGetTrans(std::string rUsrNm);
    // get last few transactions
    std::vector<rTrans> rRecTrans(std::string rUsrNm, int rCnt = 5, std::string rProd = "");

private:
    std::string rFn; // file
    std::vector<rTrans> rTranss; // list
    // helper to load from file
    void rLdTrans();
};
