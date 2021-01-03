#include "chess.h"
#include <string>
#include "board.h"

#ifndef cpu
#define cpu

class Cpu{
    public:
        colors_t color;
    public:
        Cpu(colors_t, Board&);
        std::string getMove();
};

#endif