#pragma once
#include <vector>
#include <string>
#include <map>
#include "RUser.h"
#include "RWallet.h"

// manages users, logins, and wallets
class rUsrMgr {
public:
    rUsrMgr(std::string rUsrF, std::string rWltF);
    // create new account
    bool rRegUsr(std::string rFullNm, std::string rEml, std::string rPwd, std::string& rUsrNm, std::string rCstNm = "");
    // check login
    bool rLgnUsr(std::string rUsrNm, std::string rPwd);
    // find username
    std::string rFndUsr(std::string rFullNm, std::string rEml);
    // reset password
    bool rRstPwd(std::string rFullNm, std::string rEml, std::string rNPwd);
    // get user wallet
    rWlt& rGetWlt(std::string rUsrNm);
    // save wallets to CSV
    void rSavWlts();

private:
    std::string rUsrF;
    std::string rWltF;
    std::vector<rUsr> rUsrs; // all users
    std::map<std::string, rWlt> rWlts; // user wallets

    // internal helpers
    void rLdUsrs();
    void rLdWlts();
    void rSavUsrs();
    // simple hash
    std::string rHshPwd(std::string rPwd);
    // random 10-digit ID
    std::string rGenUsr();
};
