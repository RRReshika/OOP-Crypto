#include "RMenu.h"
#include "RCSVReader.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include "RUIUtils.h"
#include "RMarketAnalysis.h"

rMenu::rMenu(rUsrMgr& rUM, rTransMgr& rTM, rSim& rS)
    : rUM(rUM), rTM(rTM), rS(rS), rUsr("") {}

void rMenu::rInit() {
    // Main application loop - re-invokes menu after each action completes
    while (true) {
        if (rUsr == "") {
            // User not logged in - show welcome menu
            std::vector<std::string> rOpts = {"Register", "Login", "Exit"};
            
            // MENU INVOCATION: Enters modal menu loop
            // - Menu loop runs with dedicated navigation handling
            // - Returns only when user presses Enter
            // - Menu loop terminates immediately upon return
            int rCh = rUI::rMenu("CRYPTO TOOLKIT - WELCOME", rOpts);
            // MENU TERMINATED: Control returned, menu loop no longer running
            
            // Execute selected action (menu will NOT re-render during execution)
            if (rCh == 0) rReg();
            else if (rCh == 1) rLgn();
            else if (rCh == 2) break;
            // After action completes, outer loop continues and menu is RE-INVOKED
        } else {
            // User logged in - show main menu
            std::vector<std::string> rOpts = {
                "Market Analysis (Candlesticks)",
                "Wallet (Deposit/Withdraw)",
                "View Transactions",
                "View Stats",
                "Simulate Trades (Task 4)",
                "Logout"
            };
            
            // Build info bar with user and balance
            rWlt& rW = rUM.rGetWlt(rUsr);
            std::string rInfo = "User: " + rUsr + " | Balance: " + 
                               std::to_string(rW.rGetBal("USDT")) + " USDT";
            
            // MENU INVOCATION: Enters modal menu loop
            int rCh = rUI::rMenu("CRYPTO TOOLKIT - MAIN MENU", rOpts, rInfo);
            // MENU TERMINATED: Control returned, menu loop no longer running
            
            // Execute selected action (menu will NOT re-render during execution)
            if (rCh == 5) {
                rUsr = ""; // Logout
                rUI::rOk("Logged out successfully.");
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                rProc(rCh + 1);
            }
            // After action completes, outer loop continues and menu is RE-INVOKED
        }
    }
}

void rMenu::rBar() {
    rWlt& rW = rUM.rGetWlt(rUsr);
    std::string rBalS = "Balance: " + std::to_string(rW.rGetBal("USDT")) + " USDT";
    
    std::cout << rUI::rBgBl << rUI::rWh << rUI::rB;
    std::cout << " User: " << rUsr << " | " << rBalS;
    std::cout << std::string(std::max(0, 40 - (int)rUsr.length() - (int)rBalS.length() - 8), ' ');
    std::cout << rUI::rR << "\n";
}

int rMenu::rGetOpt() {
    std::string rLn;
    while (true) {
        if (std::getline(std::cin, rLn)) {
            // Sanitize input to remove escape sequences
            rLn = rUI::rSanitize(rLn);
            
            // Trim whitespace
            size_t rStart = rLn.find_first_not_of(" \t\n\r");
            size_t rEnd = rLn.find_last_not_of(" \t\n\r");
            
            if (rStart == std::string::npos || rLn.empty()) {
                return -1;  // Empty input means cancel/back
            }
            
            rLn = rLn.substr(rStart, rEnd - rStart + 1);
            
            // Check if input contains only digits
            bool rValid = true;
            for (char c : rLn) {
                if (c < '0' || c > '9') {
                    rValid = false;
                    break;
                }
            }
            
            if (!rValid) {
                std::cout << rUI::rRd << " [✗] Invalid input. Enter a number: " << rUI::rR;
                continue;
            }
            
            try {
                return std::stoi(rLn);
            } catch (...) {
                std::cout << rUI::rRd << " [✗] Invalid number. Try again: " << rUI::rR;
                continue;
            }
        }
        return -1;
    }
}

std::string rMenu::rSelPrd() {
    std::vector<std::string> rPrds = rS.rGetMkt().rGetProds();
    int rCh = rUI::rMenu("SELECT PRODUCT", rPrds);
    return rPrds[rCh];
}

void rMenu::rProc(int rOpt) {
    switch (rOpt) {
        case 1: rAnls(); break;
        case 2: {
            rUI::rHdr("WALLET MANAGEMENT");
            rWlt& rW = rUM.rGetWlt(rUsr);
            std::cout << "Current Balance:\n" << rW.r2Str() << "\n";
            std::cout << "------------------------------------\n";
            std::cout << " 1: Deposit\n";
            std::cout << " 2: Withdraw\n";
            std::cout << " 0: Back\n";
            std::cout << "Choice: ";
            int rSub = rGetOpt();
            if (rSub == 1) rDep();
            else if (rSub == 2) rWth();
            break;
        }
        case 3: rTransS(); break;
        case 4: rStats(); break;
        case 5: rSimS(); break;
        default: std::cout << "[!] Invalid option\n"; break;
    }
}

void rMenu::rReg() {
    std::string rFullNm, rEml, rPwd, rUsrNm, rCstNm;
    rUI::rHdr("REGISTER NEW USER");
    
    std::cout << "Full Name: ";
    std::getline(std::cin, rFullNm);
    
    std::cout << "Email: ";
    std::getline(std::cin, rEml);
    
    std::cout << "Password: ";
    std::getline(std::cin, rPwd);
    
    std::cout << "Do you want to create a custom username? (y/n): ";
    std::string rCh;
    std::getline(std::cin, rCh);
    if (rCh == "y" || rCh == "Y") {
        std::cout << "Enter custom username: ";
        std::getline(std::cin, rCstNm);
    }
    
    if (rFullNm.empty() || rEml.empty() || rPwd.empty()) {
        rUI::rErr("All fields are required.");
    } else if (rUM.rRegUsr(rFullNm, rEml, rPwd, rUsrNm, rCstNm)) {
        rUI::rOk("Registered successfully!");
        std::cout << "Your 10-digit ID is: " << rUI::rB << rUI::rCy << rUsrNm << rUI::rR << "\n";
        if (!rCstNm.empty()) {
            std::cout << "Your custom username is: " << rUI::rB << rUI::rCy << rCstNm << rUI::rR << "\n";
        }
    } else {
        rUI::rErr("Registration failed.");
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void rMenu::rLgn() {
    std::string rU, rP;
    rUI::rHdr("LOGIN");
    std::cout << "Username: ";
    std::getline(std::cin, rU);
    std::cout << "Password: ";
    std::getline(std::cin, rP);
    
    if (rUM.rLgnUsr(rU, rP)) {
        rUsr = rU;
        rUI::rOk("Logged in!");
    } else {
        rUI::rErr("Invalid credentials.");
        std::cout << "\nOptions:\n";
        std::cout << " 1: Re-enter\n";
        std::cout << " 2: Forgot username?\n";
        std::cout << " 3: Reset password\n";
        std::cout << " 4: Back\n";
        std::cout << "Choice: ";
        int rCh = rGetOpt();
        if (rCh == 1) rLgn();
        else if (rCh == 2) rFgtUsr();
        else if (rCh == 3) rRstPwd();
    }
    if (rUsr != "") {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void rMenu::rFgtUsr() {
    std::string rFullNm, rEml;
    rUI::rHdr("FORGOT USERNAME");
    std::cout << "Full Name: ";
    std::getline(std::cin, rFullNm);
    std::cout << "Email: ";
    std::getline(std::cin, rEml);
    
    std::string rUsrNm = rUM.rFndUsr(rFullNm, rEml);
    if (rUsrNm != "") {
        rUI::rOk("User found!");
        std::cout << "Your username is: " << rUI::rB << rUI::rCy << rUsrNm << rUI::rR << "\n";
    } else {
        rUI::rErr("No user found.");
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void rMenu::rRstPwd() {
    std::string rFullNm, rEml, rNPwd;
    rUI::rHdr("PASSWORD RESET");
    std::cout << "Full Name: ";
    std::getline(std::cin, rFullNm);
    std::cout << "Email: ";
    std::getline(std::cin, rEml);
    std::cout << "New Password: ";
    std::getline(std::cin, rNPwd);
    
    if (rUM.rRstPwd(rFullNm, rEml, rNPwd)) {
        rUI::rOk("Password updated!");
    } else {
        rUI::rErr("Verification failed.");
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#include "RMarketOrder.h"

void rMenu::rAnls() {
    std::string rP = rSelPrd();
    if (rP == "" || !rValPrd(rP)) {
        rUI::rWrn("Invalid product.");
        return;
    }

    std::vector<std::string> rTfs = {"Daily", "Monthly", "Yearly"};
    int rTfCh = rUI::rMenu("SELECT TIMEFRAME", rTfs);
    std::string rTf = (rTfCh == 0) ? "daily" : (rTfCh == 1) ? "monthly" : "yearly";

    std::vector<rMktOrd> rOrds = rCSV::rRdMkt("data/20200601.csv");
    std::vector<rCandle> rAsks = rMktAnls::rCompCndl(rOrds, rP, "ask", rTf);
    std::vector<rCandle> rBids = rMktAnls::rCompCndl(rOrds, rP, "bid", rTf);

    auto rPrnTbl = [](std::string rTit, const std::vector<rCandle>& rCndls, std::string rCol) {
        std::cout << "\n" << rUI::rB << rCol << " " << rUI::rTL << std::string(68, rUI::rH[0]) << rUI::rTR << rUI::rR << "\n";
        std::cout << rUI::rB << rCol << " " << rUI::rV << " " << rTit << std::string(67 - rTit.length(), ' ') << rUI::rV << rUI::rR << "\n";
        std::cout << rUI::rB << rCol << " " << rUI::rML << std::string(68, rUI::rH[0]) << rUI::rMR << rUI::rR << "\n";
        
        if (rCndls.empty()) {
            std::cout << " " << rUI::rV << " No data." << std::string(58, ' ') << rUI::rV << "\n";
        } else {
            std::cout << " " << rUI::rV << " " << std::left << std::setw(20) << "Date" 
                      << std::setw(12) << "Open" 
                      << std::setw(12) << "High" 
                      << std::setw(12) << "Low" 
                      << std::setw(11) << "Close" << rUI::rV << "\n";
            std::cout << " " << rUI::rML << std::string(68, rUI::rH[0]) << rUI::rMR << "\n";
            
            for (const auto& rC : rCndls) {
                std::string rTrndCol = (rC.rCl >= rC.rOp) ? rUI::rGr : rUI::rRd;
                std::cout << " " << rUI::rV << " " << rTrndCol << std::left << std::setw(20) << rC.rDt 
                          << std::fixed << std::setprecision(4)
                          << std::setw(12) << rC.rOp 
                          << std::setw(12) << rC.rHi 
                          << std::setw(12) << rC.rLo 
                          << std::setw(11) << rC.rCl << rUI::rR << rUI::rV << "\n";
            }
        }
        std::cout << " " << rUI::rBL << std::string(68, rUI::rH[0]) << rUI::rBR << "\n";
    };

    rPrnTbl("Asks Candlestick Summary", rAsks, rUI::rRd);
    rPrnTbl("Bids Candlestick Summary", rBids, rUI::rGr);

    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void rMenu::rWltS() {
    rUI::rHdr("WALLET BALANCE");
    rWlt& rW = rUM.rGetWlt(rUsr);
    std::cout << "Current Balance:\n" << rW.r2Str() << "\n";
    
    rUI::rHdr("LAST 5 TRANSACTIONS");
    std::vector<rTrans> rRec = rTM.rRecTrans(rUsr, 5);
    
    auto rPrnTransTbl = [](const std::vector<rTrans>& rTs) {
        std::cout << " " << rUI::rTL << std::string(72, rUI::rH[0]) << rUI::rTR << "\n";
        std::cout << " " << rUI::rV << " " << std::left << std::setw(20) << "Date & Time" 
                  << std::setw(12) << "Type" 
                  << std::setw(15) << "Product" 
                  << std::setw(10) << "Amount" 
                  << std::setw(11) << "Balance After" << rUI::rV << "\n";
        std::cout << " " << rUI::rML << std::string(72, rUI::rH[0]) << rUI::rMR << "\n";
        
        if (rTs.empty()) {
            std::cout << " " << rUI::rV << " No transactions found." << std::string(48, ' ') << rUI::rV << "\n";
        } else {
            for (const auto& rT : rTs) {
                std::string rTypCol = (rT.rTyp == rTransTyp::deposit || rT.rTyp == rTransTyp::bid) ? rUI::rGr : rUI::rRd;
                std::cout << " " << rUI::rV << " " << std::left << std::setw(20) << rT.rTime 
                          << rTypCol << std::setw(12) << rTrans::rTyp2Str(rT.rTyp) << rUI::rR
                          << std::setw(15) << rT.rProd 
                          << std::setw(10) << rT.rAmt 
                          << std::setw(11) << rT.rBalAft << rUI::rV << "\n";
            }
        }
        std::cout << " " << rUI::rBL << std::string(72, rUI::rH[0]) << rUI::rBR << "\n";
    };

    rPrnTransTbl(rRec);
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void rMenu::rTransS() {
    rUI::rHdr("YOUR TRANSACTIONS");
    std::cout << "Filter by product: ";
    std::string rP;
    std::getline(std::cin, rP);
    
    std::vector<rTrans> rTs = rTM.rGetTrans(rUsr);
    std::vector<rTrans> rFltrd;
    for (const auto& rT : rTs) {
        if (rP == "" || rT.rProd == rP) rFltrd.push_back(rT);
    }

    auto rPrnTransTbl = [](const std::vector<rTrans>& rTs) {
        std::cout << " " << rUI::rTL << std::string(72, rUI::rH[0]) << rUI::rTR << "\n";
        std::cout << " " << rUI::rV << " " << std::left << std::setw(20) << "Date & Time" 
                  << std::setw(12) << "Type" 
                  << std::setw(15) << "Product" 
                  << std::setw(10) << "Amount" 
                  << std::setw(11) << "Balance After" << rUI::rV << "\n";
        std::cout << " " << rUI::rML << std::string(72, rUI::rH[0]) << rUI::rMR << "\n";
        
        if (rTs.empty()) {
            std::cout << " " << rUI::rV << " No transactions found." << std::string(48, ' ') << rUI::rV << "\n";
        } else {
            for (const auto& rT : rTs) {
                std::string rTypCol = (rT.rTyp == rTransTyp::deposit || rT.rTyp == rTransTyp::bid) ? rUI::rGr : rUI::rRd;
                std::cout << " " << rUI::rV << " " << std::left << std::setw(20) << rT.rTime 
                          << rTypCol << std::setw(12) << rTrans::rTyp2Str(rT.rTyp) << rUI::rR
                          << std::setw(15) << rT.rProd 
                          << std::setw(10) << rT.rAmt 
                          << std::setw(11) << rT.rBalAft << rUI::rV << "\n";
            }
        }
        std::cout << " " << rUI::rBL << std::string(72, rUI::rH[0]) << rUI::rBR << "\n";
    };

    rPrnTransTbl(rFltrd);
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void rMenu::rDep() {
    rUI::rHdr("DEPOSIT FUNDS");
    std::cout << "Currency: ";
    std::string rTyp;
    std::getline(std::cin, rTyp);
    if (rTyp.empty()) return;

    std::cout << "Amount: ";
    std::string rAmtS;
    std::getline(std::cin, rAmtS);
    
    try {
        double rAmt = std::stod(rAmtS);
        if (rAmt <= 0) throw 0;
        
        rWlt& rW = rUM.rGetWlt(rUsr);
        rW.rDep(rTyp, rAmt);
        rUM.rSavWlts();
        
        rTM.rSavTrans(rTrans(
            rCSV::rTime(), rUsr, rTransTyp::deposit, rTyp, rAmt, 1.0, rW.rGetBal(rTyp)
        ));
        rUI::rOk("Deposit successful!");
    } catch (...) {
        rUI::rErr("Invalid amount.");
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void rMenu::rWth() {
    rUI::rHdr("WITHDRAW FUNDS");
    std::cout << "Currency: ";
    std::string rTyp;
    std::getline(std::cin, rTyp);
    if (rTyp.empty()) return;

    std::cout << "Amount: ";
    std::string rAmtS;
    std::getline(std::cin, rAmtS);
    
    try {
        double rAmt = std::stod(rAmtS);
        if (rAmt <= 0) throw 0;
        
        rWlt& rW = rUM.rGetWlt(rUsr);
        if (rW.rWth(rTyp, rAmt)) {
            rUM.rSavWlts();
            rTM.rSavTrans(rTrans(
                rCSV::rTime(), rUsr, rTransTyp::withdrawal, rTyp, rAmt, 1.0, rW.rGetBal(rTyp)
            ));
            rUI::rOk("Withdrawal successful!");
        } else {
            rUI::rErr("Insufficient balance.");
        }
    } catch (...) {
        rUI::rErr("Invalid amount.");
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void rMenu::rStats() {
    rUI::rHdr("USER STATISTICS");
    std::cout << "Date (YYYY-MM-DD): ";
    std::string rDt;
    std::getline(std::cin, rDt);
    
    std::vector<rTrans> rAll = rTM.rGetTrans(rUsr);
    rSim::rStats rSt = rS.rGetStats(rUsr, rAll, rDt);
    
    std::cout << "\n " << rUI::rTL << std::string(38, rUI::rH[0]) << rUI::rTR << "\n";
    std::cout << " " << rUI::rV << " Summary (" << (rDt == "" ? "All" : rDt) << ")" << std::string(28 - (rDt == "" ? 3 : (int)rDt.length()), ' ') << rUI::rV << "\n";
    std::cout << " " << rUI::rML << std::string(38, rUI::rH[0]) << rUI::rMR << "\n";
    std::cout << " " << rUI::rV << " Asks: " << rUI::rB << rUI::rYe << std::setw(24) << rSt.rTAsks << rUI::rR << " " << rUI::rV << "\n";
    std::cout << " " << rUI::rV << " Bids: " << rUI::rB << rUI::rGr << std::setw(24) << rSt.rTBids << rUI::rR << " " << rUI::rV << "\n";
    std::cout << " " << rUI::rML << std::string(38, rUI::rH[0]) << rUI::rMR << "\n";
    
    for (const auto& rP : rSt.rPrdBrk) {
        std::string rL = rP.first + ": A:" + std::to_string(rP.second.rAsks) + " B:" + std::to_string(rP.second.rBids);
        std::cout << " " << rUI::rV << " " << rL << std::string(37 - rL.length(), ' ') << rUI::rV << "\n";
    }
    
    std::cout << " " << rUI::rML << std::string(38, rUI::rH[0]) << rUI::rMR << "\n";
    std::cout << " " << rUI::rV << " Spent: " << rUI::rB << rUI::rCy << std::setw(18) << std::fixed << std::setprecision(2) << rSt.rTSpnt << " USDT" << rUI::rR << " " << rUI::rV << "\n";
    std::cout << " " << rUI::rBL << std::string(38, rUI::rH[0]) << rUI::rBR << "\n";
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void rMenu::rSimS() {
    rUI::rHdr("SIMULATING TRADES");
    std::vector<rSim::rSimRes> rRes = rS.rSimTrds(rUsr, rUM, rTM);
    
    std::cout << " " << rUI::rTL << std::string(38, rUI::rH[0]) << rUI::rTR << "\n";
    for (const auto& rR : rRes) {
        std::string rL = rR.rProd + ": B:" + std::to_string(rR.rBids) + " A:" + std::to_string(rR.rAsks);
        std::cout << " " << rUI::rV << " " << rL << std::string(37 - rL.length(), ' ') << rUI::rV << "\n";
    }
    std::cout << " " << rUI::rBL << std::string(38, rUI::rH[0]) << rUI::rBR << "\n";
    
    rUI::rOk("Trades saved.");
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool rMenu::rValPrd(const std::string& rProd) {
    std::vector<std::string> rPrds = rS.rGetMkt().rGetProds();
    for (const std::string& rP : rPrds) {
        if (rP == rProd) return true;
    }
    return false;
}

