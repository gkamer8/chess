#include "board.h"
#include "chess.h"
#include <string>
#include <iostream>

Board::Board(){
    move_color = white;
    move_num = 1;
}

void Board::makeMove(){

}

bool Board::isLegal(std::string move){
    return true;
}