#include "RCSVReader.h"
#include <sstream>

#include <fstream>
#include <iostream>

rCSV::rCSV() {}

// generic CSV reader
std::vector<std::vector<std::string>> rCSV::rRdCSV(std::string rFn) {
    std::vector<std::vector<std::string>> rRws;
    std::ifstream rF(rFn);
    std::string rLn;
    if (rF.is_open()) {
        while (std::getline(rF, rLn)) {
            if (rLn.empty()) continue;
            rRws.push_back(rSpl(rLn, ','));
        }
        rF.close();
    }
    return rRws;
}

// specifically for market data
std::vector<rMktOrd> rCSV::rRdMkt(std::string rFn) {
    std::vector<rMktOrd> rOrds;
    std::ifstream rF(rFn);
    std::string rLn;
    if (rF.is_open()) {
        while (std::getline(rF, rLn)) {
            if (rLn.empty()) continue;
            std::vector<std::string> rTks = rSpl(rLn, ',');
            if (rTks.size() == 5) {
                try {
                    rOrds.push_back(rMktOrd(
                        rTks[0], rTks[1], rTks[2], std::stod(rTks[3]), std::stod(rTks[4])
                    ));
                } catch (...) {}
            }
        }
        rF.close();
    }
    return rOrds;
}

void rCSV::rAppRow(std::string rFn, std::vector<std::string> rRw) {
    std::ofstream rF(rFn, std::ios::app);
    if (rF.is_open()) {
        for (size_t i = 0; i < rRw.size(); ++i) {
            rF << rRw[i];
            if (i < rRw.size() - 1) rF << ",";
        }
        rF << "\n";
        rF.close();
    }
}

void rCSV::rWrtAll(std::string rFn, std::vector<std::vector<std::string>> rRws) {
    std::ofstream rF(rFn);
    if (rF.is_open()) {
        for (const auto& rRw : rRws) {
            for (size_t i = 0; i < rRw.size(); ++i) {
                rF << rRw[i];
                if (i < rRw.size() - 1) rF << ",";
            }
            rF << "\n";
        }
        rF.close();
    }
}

#include <chrono>
#include <iomanip>

// get current time
std::string rCSV::rTime() {
    auto rNw = std::chrono::system_clock::now();
    auto rInT = std::chrono::system_clock::to_time_t(rNw);
    std::stringstream rSs;
    rSs << std::put_time(std::localtime(&rInT), "%Y-%m-%d %H:%M:%S");
    return rSs.str();
}

std::vector<std::string> rCSV::rSpl(std::string rLn, char rSep) {
    std::vector<std::string> rTks;
    std::string rTk;
    std::istringstream rTksS(rLn);
    while (std::getline(rTksS, rTk, rSep)) {
        rTks.push_back(rTk);
    }
    return rTks;
}
