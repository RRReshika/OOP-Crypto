#include "RCSVReader.h"
#include <sstream>

#include <fstream>
#include <iostream>

CSVReader::CSVReader() {}

std::vector<std::vector<std::string>> CSVReader::readCSV(std::string filename) {
    std::vector<std::vector<std::string>> rows;
    std::ifstream file(filename);
    std::string line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            rows.push_back(split(line, ','));
        }
        file.close();
    }
    return rows;
}

std::vector<MarketOrder> CSVReader::readMarketOrders(std::string filename) {
    std::vector<MarketOrder> orders;
    std::ifstream file(filename);
    std::string line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::vector<std::string> tokens = split(line, ',');
            if (tokens.size() == 5) {
                try {
                    orders.push_back(MarketOrder(
                        tokens[0], // timestamp
                        tokens[1], // product
                        tokens[2], // orderType
                        std::stod(tokens[3]), // price
                        std::stod(tokens[4])  // amount
                    ));
                } catch (...) {
                    // Skip invalid rows
                }
            }
        }
        file.close();
    }
    return orders;
}

void CSVReader::appendRow(std::string filename, std::vector<std::string> row) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
        file.close();
    }
}

void CSVReader::writeAll(std::string filename, std::vector<std::vector<std::string>> rows) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& row : rows) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i < row.size() - 1) file << ",";
            }
            file << "\n";
        }
        file.close();
    }
}

#include <chrono>
#include <iomanip>

std::string CSVReader::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::vector<std::string> CSVReader::split(std::string csvLine, char separator) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(csvLine);
    while (std::getline(tokenStream, token, separator)) {
        tokens.push_back(token);
    }
    return tokens;
}
