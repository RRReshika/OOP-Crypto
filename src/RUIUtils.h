#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>

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

    // raw char input
    inline int rGetCh() {
        struct termios rOld, rNew;
        char rBuf = 0;
        
        if (tcgetattr(STDIN_FILENO, &rOld) < 0) return -1;
        
        rNew = rOld;
        rNew.c_lflag &= ~(ICANON | ECHO);
        rNew.c_cc[VMIN] = 1;
        rNew.c_cc[VTIME] = 0;
        
        if (tcsetattr(STDIN_FILENO, TCSANOW, &rNew) < 0) return -1;
        
        if (read(STDIN_FILENO, &rBuf, 1) < 0) {
            tcsetattr(STDIN_FILENO, TCSANOW, &rOld);
            return -1;
        }
        
        int rCh = (unsigned char)rBuf;
        
        if (rCh == 27) { // Escape sequence
            char rSeq[2];
            if (read(STDIN_FILENO, &rSeq[0], 1) > 0 && read(STDIN_FILENO, &rSeq[1], 1) > 0) {
                if (rSeq[0] == '[') {
                    if (rSeq[1] == 'A') rCh = 1000; // up
                    else if (rSeq[1] == 'B') rCh = 1001; // down
                }
            }
        }
        
        tcsetattr(STDIN_FILENO, TCSANOW, &rOld);
        return rCh;
    }



    inline int rMenu(const std::string& rTit, const std::vector<std::string>& rOpts) {
        int rSel = 0;
        while (true) {
            std::cout << "\033[2J\033[1;1H";
            std::cout.flush();
            rLogo();
            std::cout << rB << rCy << "\n " << rTL << std::string(38, rH[0]) << rTR << "\n";
            std::cout << " " << rV << " " << rTit << std::string(37 - rTit.length(), ' ') << rV << "\n";
            std::cout << " " << rML << std::string(38, rH[0]) << rMR << "\n";
            
            for (int i = 0; i < rOpts.size(); ++i) {
                std::cout << " " << rV << " ";
                if (i == rSel) {
                    std::cout << rBgBl << rWh << rB << " > " << rOpts[i] << " " << rR;
                    std::cout << std::string(34 - rOpts[i].length(), ' ') << rV << "\n";
                } else {
                    std::cout << "   " << rOpts[i] << std::string(35 - rOpts[i].length(), ' ') << rV << "\n";
                }
            }
            std::cout << " " << rBL << std::string(38, rH[0]) << rBR << "\n" << rR;
            std::cout << "\n [Arrows] Move  [Enter] Select\n";

            int rCh = rGetCh();
            if (rCh == 1000) {
                rSel = (rSel - 1 + rOpts.size()) % rOpts.size();
            } else if (rCh == 1001) {
                rSel = (rSel + 1) % rOpts.size();
            } else if (rCh == 10 || rCh == 13) {
                std::cout << "\033[2J\033[1;1H";
                std::cout.flush();
                return rSel;
            }
        }
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
