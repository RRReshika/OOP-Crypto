#include "RUser.h"

// constructor
rUsr::rUsr(std::string rUsrNm, std::string rCstNm, std::string rFullNm, std::string rEml, std::string rPwdHsh)
    : rUsrNm(rUsrNm), rCstNm(rCstNm), rFullNm(rFullNm), rEml(rEml), rPwdHsh(rPwdHsh), 
      rFailAttempts(0), rLockTime(0) {}
