#pragma once
#include <vector>
#include <string>
#include "RMarketOrder.h"

// handles all file reading and writing
class rCSV {
public:
    rCSV();
    // read any CSV into a list of rows
    static std::vector<std::vector<std::string>> rRdCSV(std::string rFn);
    // read market orders from the big data file
    static std::vector<rMktOrd> rRdMkt(std::string rFn);
    // add a single new row to the end of a file
    static void rAppRow(std::string rFn, std::vector<std::string> rRw);
    // overwrite a whole file with new data
    static void rWrtAll(std::string rFn, std::vector<std::vector<std::string>> rRws);
    // helper to split a line by commas
    static std::vector<std::string> rSpl(std::string rLn, char rSep);
    // get the current time in a nice format
    static std::string rTime();
};
