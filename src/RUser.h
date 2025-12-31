#pragma once
#include <string>

// user info
class rUsr {
public:
    rUsr(std::string rUsrNm, std::string rCstNm, std::string rFullNm, std::string rEml, std::string rPwdHsh);

    std::string rUsrNm; // 10-digit ID
    std::string rCstNm; // custom name
    std::string rFullNm;
    std::string rEml;
    std::string rPwdHsh; // hash
};
