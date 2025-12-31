#include "RTransaction.h"

// constructor
rTrans::rTrans(std::string rTime, std::string rUsrNm, rTransTyp rTyp, std::string rProd, double rAmt, double rPrc, double rBalAft)
    : rTime(rTime), rUsrNm(rUsrNm), rTyp(rTyp), rProd(rProd), rAmt(rAmt), rPrc(rPrc), rBalAft(rBalAft) {}

// enum to string
std::string rTrans::rTyp2Str(rTransTyp rTyp) {
    switch (rTyp) {
        case rTransTyp::deposit: return "deposit";
        case rTransTyp::withdrawal: return "withdrawal";
        case rTransTyp::ask: return "ask";
        case rTransTyp::bid: return "bid";
        default: return "unknown";
    }
}

// string to enum
rTransTyp rTrans::rStr2Typ(std::string rStr) {
    if (rStr == "deposit") return rTransTyp::deposit;
    if (rStr == "withdrawal") return rTransTyp::withdrawal;
    if (rStr == "ask") return rTransTyp::ask;
    if (rStr == "bid") return rTransTyp::bid;
    return rTransTyp::deposit; // default
}
