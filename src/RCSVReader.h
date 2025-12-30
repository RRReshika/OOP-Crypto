#pragma once
#include <vector>
#include <string>
#include "MarketOrder.h"

class CSVReader {
public:
    CSVReader();
    static std::vector<std::vector<std::string>> readCSV(std::string filename);
    static std::vector<MarketOrder> readMarketOrders(std::string filename);
    static void appendRow(std::string filename, std::vector<std::string> row);
    static void writeAll(std::string filename, std::vector<std::vector<std::string>> rows);
    static std::vector<std::string> split(std::string csvLine, char separator);
    static std::string getCurrentTimestamp();
};
