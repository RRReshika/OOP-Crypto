// Midterm Object Oriented programming project 
// Student Name : Rajagopal Raja Reshika
// Student Number : 240621557

//  all the variable/ function names start with r for identification and have been used 
//  in SHORT FORMS to keep the code concise.

#include "RMenu.h"
#include "RUserManager.h"
#include "RTransactionManager.h"
#include "RMarketData.h"
#include "RTradeSimulator.h"

// main entry
int main() {
    // load data
    rMktDat rMkt("data/20200601.csv");
    rUsrMgr rUM("data/users.csv", "data/wallets.csv");
    rTransMgr rTM("data/transactions.csv");
    rSim rS(rMkt);

    // start menu
    rMenu rM(rUM, rTM, rS);
    rM.rInit();

    return 0;
}
