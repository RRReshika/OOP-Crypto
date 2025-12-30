#pragma once
#include <vector>
#include <string>
#include <map>
#include "RUser.h"
#include "RWallet.h"

class UserManager {
public:
    UserManager(std::string usersFile, std::string walletsFile);
    bool registerUser(std::string fullName, std::string email, std::string password, std::string& outUsername, std::string customUsername = "");
    bool loginUser(std::string username, std::string password);
    std::string findUsername(std::string fullName, std::string email);
    bool resetPassword(std::string fullName, std::string email, std::string newPassword);
    Wallet& getWallet(std::string username);
    void saveWallets();

private:
    std::string usersFile;
    std::string walletsFile;
    std::vector<User> users;
    std::map<std::string, Wallet> wallets;

    void loadUsers();
    void loadWallets();
    void saveUsers();
    std::string hashPassword(std::string password);
    std::string generateUniqueUsername();
};
