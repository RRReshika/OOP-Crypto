#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>

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
    const std::string BG_BLUE = "\033[44m";

    // Box Drawing Characters (ASCII for compatibility)
    const std::string TL = "+"; // Top Left
    const std::string TR = "+"; // Top Right
    const std::string BL = "+"; // Bottom Left
    const std::string BR = "+"; // Bottom Right
    const std::string H  = "-"; // Horizontal
    const std::string V  = "|"; // Vertical
    const std::string ML = "+"; // Mid Left
    const std::string MR = "+"; // Mid Right
    const std::string MT = "+"; // Mid Top
    const std::string MB = "+"; // Mid Bottom
    const std::string C  = "+"; // Center

    inline void printLogo() {
        std::cout << CYAN << BOLD;
        std::cout << "   ____                      _          _____           _ _    _ _   \n";
        std::cout << "  / ___|_ __ _   _ _ __ | |_ ___   |_   _|__   ___ | | | _(_) |_ \n";
        std::cout << " | |   | '__| | | | '_ \\| __/ _ \\    | |/ _ \\ / _ \\| | |/ / | __|\n";
        std::cout << " | |___| |  | |_| | |_) | || (_) |   | | (_) | (_) | |   <| | |_ \n";
        std::cout << "  \\____|_|   \\__, | .__/ \\__\\___/    |_|\___/ \\___/|_|_|\\_\\_|_|\\__|\n";
        std::cout << "             |___/|_|                                            \n";
        std::cout << RESET;
    }

    // Get raw character input without Enter (macOS/Linux)
    inline int getChar() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        if (ch == 27) { // Escape sequence
            getchar(); // skip [
            ch = getchar();
            if (ch == 'A') ch = 1000; // Up
            else if (ch == 'B') ch = 1001; // Down
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }

    inline int interactiveMenu(const std::string& title, const std::vector<std::string>& options) {
        int selected = 0;
        while (true) {
            std::cout << "\033[2J\033[1;1H"; // Clear screen
            printLogo();
            std::cout << BOLD << CYAN << "\n " << TL << std::string(38, H[0]) << TR << "\n";
            std::cout << " " << V << " " << title << std::string(37 - title.length(), ' ') << V << "\n";
            std::cout << " " << ML << std::string(38, H[0]) << MR << "\n";
            
            for (int i = 0; i < options.size(); ++i) {
                std::cout << " " << V << " ";
                if (i == selected) {
                    std::cout << BG_BLUE << WHITE << BOLD << " > " << options[i] << " " << RESET;
                    std::cout << std::string(34 - options[i].length(), ' ') << V << "\n";
                } else {
                    std::cout << "   " << options[i] << std::string(35 - options[i].length(), ' ') << V << "\n";
                }
            }
            std::cout << " " << BL << std::string(38, H[0]) << BR << "\n" << RESET;
            std::cout << "\n [Arrows] Move  [Enter] Select\n";

            int ch = getChar();
            if (ch == 1000) { // Up
                selected = (selected - 1 + options.size()) % options.size();
            } else if (ch == 1001) { // Down
                selected = (selected + 1) % options.size();
            } else if (ch == 10 || ch == 13) { // Enter
                return selected;
            }
        }
    }

    inline void printHeader(const std::string& title) {
        std::cout << BOLD << CYAN << TL << std::string(38, H[0]) << TR << "\n";
        std::cout << V << " " << title << std::string(37 - title.length(), ' ') << V << "\n";
        std::cout << BL << std::string(38, H[0]) << BR << "\n" << RESET;
    }

    inline void printSuccess(const std::string& msg) {
        std::cout << BOLD << GREEN << " [✓] " << RESET << msg << "\n";
    }

    inline void printError(const std::string& msg) {
        std::cout << BOLD << RED << " [✗] " << RESET << msg << "\n";
    }

    inline void printInfo(const std::string& msg) {
        std::cout << BOLD << BLUE << " [i] " << RESET << msg << "\n";
    }

    inline void printWarning(const std::string& msg) {
        std::cout << BOLD << YELLOW << " [!] " << RESET << msg << "\n";
    }
}
