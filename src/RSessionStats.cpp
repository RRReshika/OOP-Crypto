#include "RSessionStats.h"
#include "RUIUtils.h"
#include <iostream>
#include <iomanip>

// start with everything at 0
rSessStats::rSessStats() 
    : rBuys(0), rSells(0), rDeps(0), rWths(0) {}

void rSessStats::rRecBuy() {
    rBuys++;
}

void rSessStats::rRecSell() {
    rSells++;
}

void rSessStats::rRecDep() {
    rDeps++;
}

void rSessStats::rRecWth() {
    rWths++;
}

int rSessStats::rGetTotTrds() const {
    return rBuys + rSells;
}

int rSessStats::rGetBuys() const {
    return rBuys;
}

int rSessStats::rGetSells() const {
    return rSells;
}

int rSessStats::rGetDeps() const {
    return rDeps;
}

int rSessStats::rGetWths() const {
    return rWths;
}

// show what happened this session
void rSessStats::rShowSumm(double rFinalBal) const {
    std::cout << "\n";
    rUI::rHdr("SESSION SUMMARY");
    
    std::cout << " " << rUI::rTL << std::string(50, rUI::rH[0]) << rUI::rTR << "\n";
    
    // Trading activity
    std::cout << " " << rUI::rV << " " << rUI::rB << "Trades:" << rUI::rR 
              << std::string(42, ' ') << rUI::rV << "\n";
    std::cout << " " << rUI::rML << std::string(50, rUI::rH[0]) << rUI::rMR << "\n";
    
    std::cout << " " << rUI::rV << "   Total: " << std::left << std::setw(39) 
              << rGetTotTrds() << rUI::rV << "\n";
    std::cout << " " << rUI::rV << "   " << rUI::rGr << "Buys: " << rUI::rR << std::left 
              << std::setw(39) << rBuys << rUI::rV << "\n";
    std::cout << " " << rUI::rV << "   " << rUI::rRd << "Sells: " << rUI::rR << std::left 
              << std::setw(38) << rSells << rUI::rV << "\n";
    
    // Wallet operations
    if (rDeps > 0 || rWths > 0) {
        std::cout << " " << rUI::rML << std::string(50, rUI::rH[0]) << rUI::rMR << "\n";
        std::cout << " " << rUI::rV << " " << rUI::rB << "Wallet:" << rUI::rR 
                  << std::string(42, ' ') << rUI::rV << "\n";
        std::cout << " " << rUI::rML << std::string(50, rUI::rH[0]) << rUI::rMR << "\n";
        std::cout << " " << rUI::rV << "   Deposits: " << std::left << std::setw(36) 
                  << rDeps << rUI::rV << "\n";
        std::cout << " " << rUI::rV << "   Withdrawals: " << std::left << std::setw(33) 
                  << rWths << rUI::rV << "\n";
    }
    
    // Final balance
    std::cout << " " << rUI::rML << std::string(50, rUI::rH[0]) << rUI::rMR << "\n";
    std::cout << " " << rUI::rV << " " << rUI::rB << "Final Balance:" << rUI::rR 
              << std::string(36, ' ') << rUI::rV << "\n";
    std::cout << " " << rUI::rML << std::string(50, rUI::rH[0]) << rUI::rMR << "\n";
    std::cout << " " << rUI::rV << "   " << rUI::rCy << std::fixed << std::setprecision(2) 
              << rFinalBal << " USDT" << rUI::rR << std::string(30 - std::to_string((int)rFinalBal).length(), ' ') 
              << rUI::rV << "\n";
    
    std::cout << " " << rUI::rBL << std::string(50, rUI::rH[0]) << rUI::rBR << "\n";
    
    std::cout << "\n" << rUI::rCy << " Thanks!" << rUI::rR << "\n\n";
}

void rSessStats::rReset() {
    rBuys = 0;
    rSells = 0;
    rDeps = 0;
    rWths = 0;
}
