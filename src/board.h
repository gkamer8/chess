#include "chess.h"
#include <string>

#ifndef board
#define board

class Board{
    private:
        int move_num;
        colors_t move_color;
    public:
        Board();
        void makeMove();
        bool isLegal(std::string);
};

#endif