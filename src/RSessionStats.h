#pragma once
#include <string>

// keeps track of what happened during the session
class rSessStats {
public:
    rSessStats();
    
    void rRecBuy();    // count a buy
    void rRecSell();   // count a sell
    void rRecDep();    // count a deposit
    void rRecWth();    // count a withdrawal
    
    int rGetTotTrds() const;  // total trades
    int rGetBuys() const;
    int rGetSells() const;
    int rGetDeps() const;
    int rGetWths() const;
    
    void rShowSumm(double rFinalBal) const;  // print summary
    void rReset();  // clear everything

private:
    int rBuys;
    int rSells;
    int rDeps;
    int rWths;
};
