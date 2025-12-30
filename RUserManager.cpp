#include "RUserManager.h"
#include "RCSVReader.h"
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>

UserManager::UserManager(std::string usersFile, std::string walletsFile)
    : usersFile(usersFile), walletsFile(walletsFile) {
    loadUsers();
    loadWallets();
}

bool UserManager::registerUser(std::string fullName, std::string email, std::string password, std::string& outUsername, std::string customUsername) {
    // Check if user already exists by name and email
    for (const auto& u : users) {
        if (u.fullName == fullName && u.email == email) return false;
    }
    
    // Check if custom username is already taken (if provided)
    if (!customUsername.empty()) {
        for (const auto& u : users) {
            if (u.customUsername == customUsername || u.username == customUsername) return false;
        }
    }
    
    outUsername = generateUniqueUsername();
    std::string passwordHash = hashPassword(password);
    users.push_back(User(outUsername, customUsername, fullName, email, passwordHash));
    wallets[outUsername] = Wallet();
    // Give initial balance
    wallets[outUsername].insertCurrency("BTC", 1.0);
    wallets[outUsername].insertCurrency("USDT", 10000.0);
    
    saveUsers();
    saveWallets();
    return true;
}

bool UserManager::loginUser(std::string loginName, std::string password) {
    std::string passwordHash = hashPassword(password);
    for (const auto& u : users) {
        if ((u.username == loginName || (!u.customUsername.empty() && u.customUsername == loginName)) && u.passwordHash == passwordHash) {
            return true;
        }
    }
    return false;
}

std::string UserManager::findUsername(std::string fullName, std::string email) {
    for (const auto& u : users) {
        if (u.fullName == fullName && u.email == email) {
            return u.username;
        }
    }
    return "";
}

bool UserManager::resetPassword(std::string fullName, std::string email, std::string newPassword) {
    for (auto& u : users) {
        if (u.fullName == fullName && u.email == email) {
            u.passwordHash = hashPassword(newPassword);
            saveUsers();
            return true;
        }
    }
    return false;
}

Wallet& UserManager::getWallet(std::string username) {
    return wallets[username];
}

void UserManager::loadUsers() {
    std::vector<std::vector<std::string>> rows = CSVReader::readCSV(usersFile);
    for (const auto& tokens : rows) {
        if (tokens.size() == 5) {
            users.push_back(User(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]));
        }
    }
}

void UserManager::loadWallets() {
    std::vector<std::vector<std::string>> rows = CSVReader::readCSV(walletsFile);
    for (const auto& tokens : rows) {
        if (tokens.size() >= 2) {
            std::string username = tokens[0];
            for (size_t i = 1; i < tokens.size(); i += 2) {
                if (i + 1 < tokens.size()) {
                    wallets[username].insertCurrency(tokens[i], std::stod(tokens[i+1]));
                }
            }
        }
    }
}

void UserManager::saveUsers() {
    std::vector<std::vector<std::string>> rows;
    for (const auto& u : users) {
        rows.push_back({u.username, u.customUsername, u.fullName, u.email, u.passwordHash});
    }
    CSVReader::writeAll(usersFile, rows);
}

void UserManager::saveWallets() {
    std::vector<std::vector<std::string>> rows;
    for (auto const& [username, wallet] : wallets) {
        std::vector<std::string> row;
        row.push_back(username);
        for (auto const& [type, amount] : wallet.currencies) {
            row.push_back(type);
            row.push_back(std::to_string(amount));
        }
        rows.push_back(row);
    }
    CSVReader::writeAll(walletsFile, rows);
}

std::string UserManager::hashPassword(std::string password) {
    size_t h = std::hash<std::string>{}(password);
    return std::to_string(h);
}

std::string UserManager::generateUniqueUsername() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<long long> dis(1000000000LL, 9999999999LL);
    
    std::string username;
    bool unique = false;
    while (!unique) {
        username = std::to_string(dis(gen));
        unique = true;
        for (const auto& u : users) {
            if (u.username == username) {
                unique = false;
                break;
            }
        }
    }
    return username;
}
