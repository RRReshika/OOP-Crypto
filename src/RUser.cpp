#include "RUser.h"

User::User(std::string username, std::string customUsername, std::string fullName, std::string email, std::string passwordHash)
    : username(username), customUsername(customUsername), fullName(fullName), email(email), passwordHash(passwordHash) {}
