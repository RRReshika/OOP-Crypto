#pragma once
#include "RUserManager.h"
#include "RTransactionManager.h"
#include "RTradeSimulator.h"
#include "RSessionStats.h"

// main menu
class rMenu {
public:
    rMenu(rUsrMgr& rUsrMgr, rTransMgr& rTransMgr, rSim& rSim);
    // start loop
    void rInit();

private:
    rUsrMgr& rUM;
    rTransMgr& rTM;
    rSim& rS;
    std::string rUsr; // current user
    rSessStats rSessStat; // session statistics tracker

    // UI helpers
    void rBar();
    int rGetOpt();
    void rProc(int rOpt);

    // screen handlers
    void rReg();
    void rLgn();
    void rWltS();
    void rTransS();
    void rDep();
    void rWth();
    void rStats();
    void rSimS();
    void rAnls();
    void rFgtUsr();
    void rRstPwd();
    std::string rSelPrd();

    // helper
    bool rValPrd(const std::string& rProd);
};
