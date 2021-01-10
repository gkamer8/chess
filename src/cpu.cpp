#include "cpu.h"

Cpu::Cpu(colors_t clr, Board* b){
    color = clr;
    brd = b;
}

std::string Cpu::getMove(){
    return "d4";  // Placeholder
}

// Gets legal moves (represented by move structs) for whatever side has the turn
std::vector<Move> Cpu::getLegalMoves(Board& play_brd){
    std::vector<Move> legalMoves;
    // Ask if castling is legal
    Move ks_castle = play_brd.parseMove("0-0");
    if(play_brd.isLegal(ks_castle)){
        legalMoves.push_back(ks_castle);
    }
    Move qs_castle = play_brd.parseMove("0-0-0");
    if(play_brd.isLegal(qs_castle)){
        legalMoves.push_back(qs_castle);
    }

    // Go through my pawns
        // See if I can move my pawns one square up
            // If I can promote a pawn, add promotion possibilities
        // See if my pawns can capture, including en passant
            // If I can promote a pawn, add promotion possibilities
        // If they're on the 2nd/7th rank, see if I can move them two squares up

    // Go through my non-castling king moves – make sure I'm not in check afterward
    Piece* king = play_brd.getMoveColor() == white ? (*play_brd.getWhitePieceMap())[K].front() : (*play_brd.getBlackPieceMap())[K].front();

    // Go through my rooks
    // Go through my bishops
    // Go through my queens
    // Go through my knights

    // Make sure that all the pieces I can move are not pinned
    return legalMoves;
}

std::string Cpu::getMoveName(Move& move){
    return "e4";  // placeholder
}