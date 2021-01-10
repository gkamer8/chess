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
    colors_t oppo_col = play_brd.getMoveColor() == white ? black : white;
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

    // Go through my non-castling king moves – make sure destination square isn't attacked
    Piece* king = play_brd.getMoveColor() == white ? (*play_brd.getWhitePieceMap())[K].front() : (*play_brd.getBlackPieceMap())[K].front();
    if(king->square->n != nullptr){
        Square* to_sq = king->square->n;
        if(to_sq->piece == nullptr){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "K";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
        else if(to_sq->piece->owner == oppo_col){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "Kx";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
    }
    if(king->square->s != nullptr){
        Square* to_sq = king->square->s;
        if(to_sq->piece == nullptr){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "K";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
        else if(to_sq->piece->owner == oppo_col){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "Kx";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
    }
    if(king->square->e != nullptr){
        Square* to_sq = king->square->e;
        if(to_sq->piece == nullptr){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "K";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
        else if(to_sq->piece->owner == oppo_col){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "Kx";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
    }
    if(king->square->w != nullptr){
        Square* to_sq = king->square->w;
        if(to_sq->piece == nullptr){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "K";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
        else if(to_sq->piece->owner == oppo_col){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "Kx";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
    }
    if(king->square->ne != nullptr){
        Square* to_sq = king->square->ne;
        if(to_sq->piece == nullptr){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "K";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
        else if(to_sq->piece->owner == oppo_col){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "Kx";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
    }
    if(king->square->nw != nullptr){
        Square* to_sq = king->square->nw;
        if(to_sq->piece == nullptr){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "K";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
        else if(to_sq->piece->owner == oppo_col){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "Kx";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
    }
    if(king->square->se != nullptr){
        Square* to_sq = king->square->se;
        if(to_sq->piece == nullptr){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "K";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
        else if(to_sq->piece->owner == oppo_col){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "Kx";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
    }
    if(king->square->sw != nullptr){
        Square* to_sq = king->square->sw;
        if(to_sq->piece == nullptr){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "K";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
        else if(to_sq->piece->owner == oppo_col){
            if(!play_brd.isAttacked(to_sq, oppo_col)){
                std::string toParse = "Kx";
                toParse += to_sq->name;
                legalMoves.push_back(play_brd.parseMove(toParse));
            }
        }
    }

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