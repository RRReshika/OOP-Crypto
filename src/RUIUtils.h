#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <cstring>

namespace rUI {
    // ANSI colors
    const std::string rR   = "\033[0m";
    const std::string rB    = "\033[1m";
    const std::string rRd     = "\033[31m";
    const std::string rGr   = "\033[32m";
    const std::string rYe  = "\033[33m";
    const std::string rBl    = "\033[34m";
    const std::string rMa = "\033[35m";
    const std::string rCy    = "\033[36m";
    const std::string rWh   = "\033[37m";
    const std::string rBgBl = "\033[44m";

    // box chars
    const std::string rTL = "+";
    const std::string rTR = "+";
    const std::string rBL = "+";
    const std::string rBR = "+";
    const std::string rH  = "-";
    const std::string rV  = "|";
    const std::string rML = "+";
    const std::string rMR = "+";

    inline void rLogo() {
        std::cout << rCy << rB;
        std::cout << "   ____                      _          _____           _ _    _ _   \n";
        std::cout << "  / ___|_ __ _   _ _ __ | |_ ___   |_   _|__   ___ | | | _(_) |_ \n";
        std::cout << " | |   | '__| | | | '_ \\| __/ _ \\    | |/ _ \\ / _ \\| | |/ / | __|\n";
        std::cout << " | |___| |  | |_| | |_) | || (_) |   | | (_) | (_) | |   <| | |_ \n";
        std::cout << "  \\____|_|   \\__, | .__/ \\__\\___/    |_|\\___/ \\___/|_|_|\\_\\_|_|\\__|\n";
        std::cout << "             |___/|_|                                            \n";
        std::cout << rR;
    }

    // Sanitize input by removing escape sequences and non-printable chars
    inline std::string rSanitize(const std::string& rIn) {
        std::string rOut;
        bool rEsc = false;
        
        for (size_t i = 0; i < rIn.length(); ++i) {
            char c = rIn[i];
            
            // Detect start of escape sequence
            if (c == 27 || c == '\033') {
                rEsc = true;
                continue;
            }
            
            // Skip characters that are part of escape sequence
            if (rEsc) {
                // Escape sequences typically end with a letter
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                    rEsc = false;
                }
                continue;
            }
            
            // Only keep printable characters and spaces
            if ((c >= 32 && c <= 126) || c == '\n' || c == '\t') {
                rOut += c;
            }
        }
        
        return rOut;
    }

    // Raw character input for menu navigation only
    // Returns: -1=error, 10/13=enter, 1000=up, 1001=down
    inline int rGetKey() {
        struct termios rOld, rNew;
        
        // Save current terminal settings
        if (tcgetattr(STDIN_FILENO, &rOld) < 0) return -1;
        
        // Configure raw mode: no echo, no canonical processing
        rNew = rOld;
        rNew.c_lflag &= ~(ICANON | ECHO);
        rNew.c_cc[VMIN] = 1;
        rNew.c_cc[VTIME] = 0;
        
        // Apply raw mode temporarily
        if (tcsetattr(STDIN_FILENO, TCSANOW, &rNew) < 0) return -1;
        
        char rBuf[3];
        std::memset(rBuf, 0, sizeof(rBuf));
        
        // Read first character
        if (read(STDIN_FILENO, &rBuf[0], 1) < 0) {
            tcsetattr(STDIN_FILENO, TCSANOW, &rOld);
            return -1;
        }
        
        int rKey = (unsigned char)rBuf[0];
        
        // Handle escape sequences (arrow keys)
        if (rKey == 27) {
            // Try to read next two bytes for arrow key sequence
            if (read(STDIN_FILENO, &rBuf[1], 1) > 0) {
                if (rBuf[1] == '[') {
                    if (read(STDIN_FILENO, &rBuf[2], 1) > 0) {
                        if (rBuf[2] == 'A') rKey = 1000; // Up arrow
                        else if (rBuf[2] == 'B') rKey = 1001; // Down arrow
                    }
                }
            }
        }
        
        // Restore terminal settings immediately
        tcsetattr(STDIN_FILENO, TCSANOW, &rOld);
        
        return rKey;
    }

    // Modal menu with explicit lifecycle control
    // LOOP LIFECYCLE:
    //   - Starts: Menu enters dedicated navigation loop
    //   - During: Handles arrow keys, redraws on each navigation
    //   - Terminates: IMMEDIATELY on Enter - loop exits via return
    //   - After: Control transfers to caller, menu never re-renders unless re-invoked
    inline int rMenu(const std::string& rTit, const std::vector<std::string>& rOpts, 
                     const std::string& rInfoBar = "") {
        int rSel = 0;
        
        // MENU LOOP - Runs ONLY while user is selecting
        while (true) {
            // Clear screen and position cursor at top
            std::cout << "\033[2J\033[1;1H";
            std::cout.flush();
            
            // Display info bar if provided (e.g., user balance)
            if (!rInfoBar.empty()) {
                std::cout << rBgBl << rWh << rB << " " << rInfoBar;
                // Pad to 80 characters
                int rPad = 79 - rInfoBar.length();
                if (rPad > 0) std::cout << std::string(rPad, ' ');
                std::cout << rR << "\n";
            }
            
            // Render menu completely
            rLogo();
            std::cout << rB << rCy << "\n " << rTL << std::string(38, rH[0]) << rTR << "\n";
            std::cout << " " << rV << " " << rTit << std::string(37 - rTit.length(), ' ') << rV << "\n";
            std::cout << " " << rML << std::string(38, rH[0]) << rMR << "\n";
            
            // Render all options with current selection highlighted
            for (size_t i = 0; i < rOpts.size(); ++i) {
                std::cout << " " << rV << " ";
                if (i == (size_t)rSel) {
                    // Highlighted option
                    std::cout << rBgBl << rWh << rB << " > " << rOpts[i] << " " << rR;
                    int pad = 34 - rOpts[i].length();
                    if (pad > 0) std::cout << std::string(pad, ' ');
                } else {
                    // Normal option
                    std::cout << "   " << rOpts[i];
                    int pad = 35 - rOpts[i].length();
                    if (pad > 0) std::cout << std::string(pad, ' ');
                }
                std::cout << rV << "\n";
            }
            
            std::cout << " " << rBL << std::string(38, rH[0]) << rBR << "\n" << rR;
            std::cout << "\n " << rB << "[↑/↓]" << rR << " Navigate  " 
                      << rB << "[Enter]" << rR << " Select";
            std::cout.flush();
            
            // Get key input in raw mode (no echo)
            int rKey = rGetKey();
            
            // Handle navigation keys - update selection and continue loop
            if (rKey == 1000) { // Up arrow
                rSel = (rSel - 1 + rOpts.size()) % rOpts.size();
                // Loop continues - menu will redraw with new selection
            } else if (rKey == 1001) { // Down arrow
                rSel = (rSel + 1) % rOpts.size();
                // Loop continues - menu will redraw with new selection
            } else if (rKey == 10 || rKey == 13) { // Enter key
                // EXPLICIT TERMINATION: Clear screen and exit loop immediately
                std::cout << "\033[2J\033[1;1H";
                std::cout.flush();
                // RETURN terminates loop - control transfers to caller
                // Menu will NOT re-render unless explicitly re-invoked
                return rSel;
            }
            // All other keys ignored - loop continues for next valid input
        }
        // This point is never reached due to return statement above
    }

    inline void rHdr(const std::string& rTit) {
        std::cout << rB << rCy << rTL << std::string(38, rH[0]) << rTR << "\n";
        std::cout << rV << " " << rTit << std::string(37 - rTit.length(), ' ') << rV << "\n";
        std::cout << rBL << std::string(38, rH[0]) << rBR << "\n" << rR;
    }

    inline void rOk(const std::string& rMsg) {
        std::cout << rB << rGr << " [✓] " << rR << rMsg << "\n";
    }

    inline void rErr(const std::string& rMsg) {
        std::cout << rB << rRd << " [✗] " << rR << rMsg << "\n";
    }

    inline void rInf(const std::string& rMsg) {
        std::cout << rB << rBl << " [i] " << rR << rMsg << "\n";
    }

    inline void rWrn(const std::string& rMsg) {
        std::cout << rB << rYe << " [!] " << rR << rMsg << "\n";
    }
}
