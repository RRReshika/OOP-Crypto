#pragma once
#include <string>

class User {
public:
    User(std::string username, std::string customUsername, std::string fullName, std::string email, std::string passwordHash);

    std::string username; // 10-digit ID
    std::string customUsername;
    std::string fullName;
    std::string email;
    std::string passwordHash;
};
