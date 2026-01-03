#include "RTransactionManager.h"
#include "RCSVReader.h"
#include <fstream>
#include <iostream>

rTransMgr::rTransMgr(std::string rFn) : rFn(rFn) {
    rLdTrans();
}

void rTransMgr::rSavTrans(rTrans rT) {
    rTranss.push_back(rT);
    // write to CSV
    rCSV::rAppRow(rFn, {
        rT.rTime,
        rT.rUsrNm,
        rTrans::rTyp2Str(rT.rTyp),
        rT.rProd,
        std::to_string(rT.rAmt),
        std::to_string(rT.rPrc),
        std::to_string(rT.rBalAft)
    });
}

std::vector<rTrans> rTransMgr::rGetTrans(std::string rUsrNm) {
    std::vector<rTrans> rUT;
    for (const auto& rT : rTranss) {
        if (rT.rUsrNm == rUsrNm) {
            rUT.push_back(rT);
        }
    }
    return rUT;
}

std::vector<rTrans> rTransMgr::rRecTrans(std::string rUsrNm, int rCnt, std::string rProd) {
    std::vector<rTrans> rFltrd;
    // newest first
    for (auto it = rTranss.rbegin(); it != rTranss.rend(); ++it) {
        if (it->rUsrNm == rUsrNm) {
            if (rProd == "" || it->rProd == rProd) {
                rFltrd.push_back(*it);
                if (rFltrd.size() >= static_cast<size_t>(rCnt)) break;
            }
        }
    }
    return rFltrd;
}

void rTransMgr::rLdTrans() {
    // read CSV
    std::vector<std::vector<std::string>> rRws = rCSV::rRdCSV(rFn);
    for (const auto& rTks : rRws) {
        if (rTks.size() == 7) {
            rTranss.push_back(rTrans(
                rTks[0], rTks[1], rTrans::rStr2Typ(rTks[2]),
                rTks[3], std::stod(rTks[4]), std::stod(rTks[5]), std::stod(rTks[6])
            ));
        }
    }
}
