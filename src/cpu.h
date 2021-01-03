#include "chess.h"
#include <string>

#ifndef cpu
#define cpu

class Cpu{
    public:
        colors_t color;
    public:
        Cpu(colors_t);
        std::string getMove();
};

#endif