#include "chess.h"
#include <string>
#include "board.h"
#include <vector>

#ifndef cpu
#define cpu

class Cpu{
    public:
        colors_t color;
        Board* brd;
    public:
        Cpu(colors_t, Board*);
        std::string getMove();
        std::vector<Move> getLegalMoves(Board&);
        std::string getMoveName(Move&);
};

#endif