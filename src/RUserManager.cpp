#include "RUserManager.h"
#include "RCSVReader.h"
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>

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
    
    for (auto& rU : rUsrs) {
        if (rU.rUsrNm == rLgnNm || (!rU.rCstNm.empty() && rU.rCstNm == rLgnNm)) {
            // Check if account is locked
            if (rU.rLockTime > 0) {
                std::time_t rNow = std::time(nullptr);
                double rElapsed = std::difftime(rNow, rU.rLockTime);
                
                // Unlock after 5 minutes (300 seconds)
                if (rElapsed >= 300) {
                    rU.rFailAttempts = 0;
                    rU.rLockTime = 0;
                    rSavUsrs();
                } else {
                    // Still locked
                    return false;
                }
            }
            
            // Check password
            if (rU.rPwdHsh == rPwdH) {
                // Successful login - reset attempts
                rU.rFailAttempts = 0;
                rU.rLockTime = 0;
                rSavUsrs();
                return true;
            } else {
                // Failed login - increment attempts
                rU.rFailAttempts++;
                
                // Lock after 3 failed attempts
                if (rU.rFailAttempts >= 3) {
                    rU.rLockTime = std::time(nullptr);
                }
                
                rSavUsrs();
                return false;
            }
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
        if (rTks.size() >= 5) {
            rUsr rU(rTks[0], rTks[1], rTks[2], rTks[3], rTks[4]);
            // Load lockout info if available
            if (rTks.size() >= 7) {
                rU.rFailAttempts = std::stoi(rTks[5]);
                rU.rLockTime = std::stoll(rTks[6]);
            }
            rUsrs.push_back(rU);
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
        rRws.push_back({
            rU.rUsrNm, 
            rU.rCstNm, 
            rU.rFullNm, 
            rU.rEml, 
            rU.rPwdHsh,
            std::to_string(rU.rFailAttempts),
            std::to_string(rU.rLockTime)
        });
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

bool rUsrMgr::rIsLocked(std::string rLgnNm) {
    for (const auto& rU : rUsrs) {
        if (rU.rUsrNm == rLgnNm || (!rU.rCstNm.empty() && rU.rCstNm == rLgnNm)) {
            if (rU.rLockTime > 0) {
                std::time_t rNow = std::time(nullptr);
                double rElapsed = std::difftime(rNow, rU.rLockTime);
                return rElapsed < 300; // Locked for 5 minutes
            }
            return false;
        }
    }
    return false;
}

int rUsrMgr::rGetLockTime(std::string rLgnNm) {
    for (const auto& rU : rUsrs) {
        if (rU.rUsrNm == rLgnNm || (!rU.rCstNm.empty() && rU.rCstNm == rLgnNm)) {
            if (rU.rLockTime > 0) {
                std::time_t rNow = std::time(nullptr);
                double rElapsed = std::difftime(rNow, rU.rLockTime);
                int rRemaining = 300 - (int)rElapsed;
                return rRemaining > 0 ? rRemaining : 0;
            }
            return 0;
        }
    }
    return 0;
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
