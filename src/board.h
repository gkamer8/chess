#include "chess.h"
#include <string>

#ifndef board
#define board

class Board{
    private:
        int move_num;
        colors_t move_color;
        bool white_ks_castle_eligible;
        bool black_ks_castle_eligible;
        bool white_qs_castle_eligible;
        bool black_qs_castle_eligible;
        bool check;
    public:
        Board();
        void makeMove();
        bool isLegal(std::string);
};

#endif