#pragma once
#include <string>
#include <map>
#include <vector>

// tracks crypto balances
class rWlt {
public:
    rWlt();
    // add crypto
    void rInsCur(std::string rTyp, double rAmt);
    // remove crypto
    bool rRemCur(std::string rTyp, double rAmt);
    // check if enough
    bool rHasCur(std::string rTyp, double rAmt);
    // deposit
    void rDep(std::string rTyp, double rAmt);
    // withdraw
    bool rWth(std::string rTyp, double rAmt);
    // get balance
    double rGetBal(std::string rTyp);
    // show balances
    std::string r2Str();

    // currency map
    std::map<std::string, double> rCurs;
};
