#pragma once
#include <string>
#include <ctime>

// user info
class rUsr {
public:
    rUsr(std::string rUsrNm, std::string rCstNm, std::string rFullNm, std::string rEml, std::string rPwdHsh);

    std::string rUsrNm; // 10-digit ID
    std::string rCstNm; // custom name
    std::string rFullNm;
    std::string rEml;
    std::string rPwdHsh; // hash
    int rFailAttempts; // failed login attempts
    std::time_t rLockTime; // when account was locked (0 = not locked)
};
