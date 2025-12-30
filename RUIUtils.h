#pragma once
#include <string>
#include <iostream>

namespace UI {
    // ANSI Color Codes
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN    = "\033[36m";
    const std::string WHITE   = "\033[37m";

    // Backgrounds
    const std::string BG_BLUE = "\033[44m";

    inline void clearScreen() {
        // Removed to preserve terminal history as per user request
        // std::cout << "\033[2J\033[1;1H";
    }

    inline void printHeader(const std::string& title) {
        std::cout << BOLD << CYAN << "========================================\n";
        std::cout << "   " << title << "\n";
        std::cout << "========================================\n" << RESET;
    }

    inline void printSuccess(const std::string& msg) {
        std::cout << BOLD << GREEN << "[SUCCESS] " << RESET << msg << "\n";
    }

    inline void printError(const std::string& msg) {
        std::cout << BOLD << RED << "[ERROR] " << RESET << msg << "\n";
    }

    inline void printInfo(const std::string& msg) {
        std::cout << BOLD << BLUE << "[INFO] " << RESET << msg << "\n";
    }

    inline void printWarning(const std::string& msg) {
        std::cout << BOLD << YELLOW << "[!] " << RESET << msg << "\n";
    }
}
