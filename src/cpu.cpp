#include "cpu.h"

Cpu::Cpu(colors_t clr, Board* b){
    color = clr;
    brd = b;
}

std::string Cpu::getMove(){
    return "d4";  // Placeholder
}

std::vector<Move> Cpu::getLegalMoves(){
    // Ask if castling is legal
    // Go through my pawns
    // Go through my rooks
    // Go through my bishops
    // Go through my queens
}

std::string Cpu::getMoveName(Move& move){
    return "e4";  // placeholder
}