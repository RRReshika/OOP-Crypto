#include "RUserManager.h"
#include "RCSVReader.h"
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>

rUsrMgr::rUsrMgr(std::string rUsrF, std::string rWltF)
    : rUsrF(rUsrF), rWltF(rWltF) {
    rLdUsrs();
    rLdWlts();
}

bool rUsrMgr::rRegUsr(std::string rFullNm, std::string rEml, std::string rPwd, std::string& rUsrNm, std::string rCstNm) {
    // check if account exists
    for (const auto& rU : rUsrs) {
        if (rU.rFullNm == rFullNm && rU.rEml == rEml) return false;
    }
    
    // check if custom username taken
    if (!rCstNm.empty()) {
        for (const auto& rU : rUsrs) {
            if (rU.rCstNm == rCstNm || rU.rUsrNm == rCstNm) return false;
        }
    }
    
    rUsrNm = rGenUsr();
    std::string rPwdH = rHshPwd(rPwd);
    rUsrs.push_back(rUsr(rUsrNm, rCstNm, rFullNm, rEml, rPwdH));
    rWlts[rUsrNm] = rWlt();
    // starter money
    rWlts[rUsrNm].rInsCur("BTC", 1.0);
    rWlts[rUsrNm].rInsCur("USDT", 10000.0);
    
    rSavUsrs();
    rSavWlts();
    return true;
}

bool rUsrMgr::rLgnUsr(std::string rLgnNm, std::string rPwd) {
    std::string rPwdH = rHshPwd(rPwd);
    for (const auto& rU : rUsrs) {
        if ((rU.rUsrNm == rLgnNm || (!rU.rCstNm.empty() && rU.rCstNm == rLgnNm)) && rU.rPwdHsh == rPwdH) {
            return true;
        }
    }
    return false;
}

std::string rUsrMgr::rFndUsr(std::string rFullNm, std::string rEml) {
    for (const auto& rU : rUsrs) {
        if (rU.rFullNm == rFullNm && rU.rEml == rEml) {
            return rU.rUsrNm;
        }
    }
    return "";
}

bool rUsrMgr::rRstPwd(std::string rFullNm, std::string rEml, std::string rNPwd) {
    for (auto& rU : rUsrs) {
        if (rU.rFullNm == rFullNm && rU.rEml == rEml) {
            rU.rPwdHsh = rHshPwd(rNPwd);
            rSavUsrs();
            return true;
        }
    }
    return false;
}

rWlt& rUsrMgr::rGetWlt(std::string rUsrNm) {
    return rWlts[rUsrNm];
}

void rUsrMgr::rLdUsrs() {
    std::vector<std::vector<std::string>> rRws = rCSV::rRdCSV(rUsrF);
    for (const auto& rTks : rRws) {
        if (rTks.size() == 5) {
            rUsrs.push_back(rUsr(rTks[0], rTks[1], rTks[2], rTks[3], rTks[4]));
        }
    }
}

void rUsrMgr::rLdWlts() {
    std::vector<std::vector<std::string>> rRws = rCSV::rRdCSV(rWltF);
    for (const auto& rTks : rRws) {
        if (rTks.size() >= 2) {
            std::string rUsrNm = rTks[0];
            for (size_t i = 1; i < rTks.size(); i += 2) {
                if (i + 1 < rTks.size()) {
                    rWlts[rUsrNm].rInsCur(rTks[i], std::stod(rTks[i+1]));
                }
            }
        }
    }
}

void rUsrMgr::rSavUsrs() {
    std::vector<std::vector<std::string>> rRws;
    for (const auto& rU : rUsrs) {
        rRws.push_back({rU.rUsrNm, rU.rCstNm, rU.rFullNm, rU.rEml, rU.rPwdHsh});
    }
    rCSV::rWrtAll(rUsrF, rRws);
}

void rUsrMgr::rSavWlts() {
    std::vector<std::vector<std::string>> rRws;
    for (auto const& [rUsrNm, rWlt] : rWlts) {
        std::vector<std::string> rRw;
        rRw.push_back(rUsrNm);
        for (auto const& [rTyp, rAmt] : rWlt.rCurs) {
            rRw.push_back(rTyp);
            rRw.push_back(std::to_string(rAmt));
        }
        rRws.push_back(rRw);
    }
    rCSV::rWrtAll(rWltF, rRws);
}

std::string rUsrMgr::rHshPwd(std::string rPwd) {
    size_t rH = std::hash<std::string>{}(rPwd);
    return std::to_string(rH);
}

std::string rUsrMgr::rGenUsr() {
    static std::random_device rRd;
    static std::mt19937 rGen(rRd());
    static std::uniform_int_distribution<long long> rDis(1000000000LL, 9999999999LL);
    
    std::string rUsrNm;
    bool rUnq = false;
    while (!rUnq) {
        rUsrNm = std::to_string(rDis(rGen));
        rUnq = true;
        for (const auto& rU : rUsrs) {
            if (rU.rUsrNm == rUsrNm) {
                rUnq = false;
                break;
            }
        }
    }
    return rUsrNm;
}
