#include "RWallet.h"
#include <iostream>
#include <sstream>
#include <iomanip>

rWlt::rWlt() {}

void rWlt::rInsCur(std::string rTyp, double rAmt) {
    if (rAmt < 0) throw std::invalid_argument("Neg amt");
    if (rCurs.count(rTyp) == 0) rCurs[rTyp] = 0;
    rCurs[rTyp] += rAmt;
}

bool rWlt::rRemCur(std::string rTyp, double rAmt) {
    if (rAmt < 0) throw std::invalid_argument("Neg amt");
    if (rCurs.count(rTyp) == 0 || rCurs[rTyp] < rAmt) return false;
    rCurs[rTyp] -= rAmt;
    return true;
}

void rWlt::rDep(std::string rTyp, double rAmt) {
    if (rAmt <= 0) return;
    rInsCur(rTyp, rAmt);
}

bool rWlt::rWth(std::string rTyp, double rAmt) {
    if (rAmt <= 0) return false;
    return rRemCur(rTyp, rAmt);
}

bool rWlt::rHasCur(std::string rTyp, double rAmt) {
    if (rCurs.count(rTyp) == 0) return false;
    return rCurs[rTyp] >= rAmt;
}

double rWlt::rGetBal(std::string rTyp) {
    if (rCurs.count(rTyp) == 0) return 0;
    return rCurs[rTyp];
}

std::string rWlt::r2Str() {
    std::stringstream rSs;
    rSs << std::left << std::setw(15) << "Currency" << std::setw(15) << "Balance" << "\n";
    rSs << std::string(30, '-') << "\n";
    for (auto const& [rTyp, rAmt] : rCurs) {
        rSs << std::left << std::setw(15) << rTyp 
           << std::fixed << std::setprecision(4) << std::setw(15) << rAmt << "\n";
    }
    return rSs.str();
}
