#pragma once
#include <string>

// wallet transaction types
enum class rTransTyp {
    deposit,
    withdrawal,
    ask, // sell
    bid  // buy
};

// stores single transaction info
class rTrans {
public:
    rTrans(std::string rTime, std::string rUsrNm, rTransTyp rTyp, std::string rProd, double rAmt, double rPrc, double rBalAft);

    std::string rTime;   // when
    std::string rUsrNm;  // who
    rTransTyp rTyp;      // what
    std::string rProd;   // which crypto
    double rAmt;         // how much
    double rPrc;         // price
    double rBalAft;      // balance after

    // enum/string conversion
    static std::string rTyp2Str(rTransTyp rTyp);
    static rTransTyp rStr2Typ(std::string rStr);
};
