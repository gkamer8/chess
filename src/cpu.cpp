#include "cpu.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wc++11-extensions"

Cpu::Cpu(colors_t clr, Board* b){
    color = clr;
    brd = b;
}

std::string Cpu::getMove(){
    // Grab a legal move at random and then get the name
    std::vector<Move> legalMoves = getLegalMoves(*brd);
    srand(time(NULL));
    int selection = rand() % legalMoves.size();
    return brd->getName(legalMoves[selection]);
}

// Gets legal moves (represented by move structs) for whatever side has the turn
std::vector<Move> Cpu::getLegalMoves(Board& play_brd){
    std::vector<Move> legalMoves;
    colors_t oppo_col = play_brd.getMoveColor() == white ? black : white;
    std::unordered_map<piece_t, std::list<Piece*>, std::hash<int> > piece_map = oppo_col == white ? *(play_brd.getBlackPieceMap()) : *(play_brd.getWhitePieceMap());
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
        // If they're on the 2nd/7th rank, see if I can move them two squares up
        // See if my pawns can capture, including en passant
            // If I can promote a pawn, add promotion possibilities
    for(auto const& i : piece_map[p]){
        Square* sq = i->square;
        if(oppo_col == black){
            // Can't push the pawn if it's blocked
            if(sq->n->piece == nullptr){
                // Can I promote the pawn?
                if(sq->rank == 6){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->n;
                    to_add.promotedTo = Q;
                    legalMoves.push_back(to_add);

                    Move to_addN;
                    to_addN.enPassant = "";
                    to_addN.ks_castle = false;
                    to_addN.qs_castle = false;
                    to_addN.piece = i;
                    to_addN.to = sq->n;
                    to_addN.promotedTo = N;
                    legalMoves.push_back(to_addN);

                    Move to_addB;
                    to_addB.enPassant = "";
                    to_addB.ks_castle = false;
                    to_addB.qs_castle = false;
                    to_addB.piece = i;
                    to_addB.to = sq->n;
                    to_addB.promotedTo = B;
                    legalMoves.push_back(to_addB);

                    Move to_addR;
                    to_addR.enPassant = "";
                    to_addR.ks_castle = false;
                    to_addR.qs_castle = false;
                    to_addR.piece = i;
                    to_addR.to = sq->n;
                    to_addR.promotedTo = R;
                    legalMoves.push_back(to_addR);
                }
                else{
                    // Add a one square pawn push
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->n;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                }
                // TODO
                // See if I can move the pawn two squares
                if(sq->rank == 1 && sq->n->n->piece == nullptr){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->n->n;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                }
            }
            // Deal with captures
            // Capture east
                // Promotion
            if(sq->ne != nullptr && sq->ne->piece != nullptr && sq->ne->piece->owner == oppo_col){
                if(sq->rank == 6){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->ne;
                    to_add.promotedTo = Q;
                    legalMoves.push_back(to_add);

                    Move to_addN;
                    to_addN.enPassant = "";
                    to_addN.ks_castle = false;
                    to_addN.qs_castle = false;
                    to_addN.piece = i;
                    to_addN.to = sq->ne;
                    to_addN.promotedTo = N;
                    legalMoves.push_back(to_addN);

                    Move to_addB;
                    to_addB.enPassant = "";
                    to_addB.ks_castle = false;
                    to_addB.qs_castle = false;
                    to_addB.piece = i;
                    to_addB.to = sq->ne;
                    to_addB.promotedTo = B;
                    legalMoves.push_back(to_addB);

                    Move to_addR;
                    to_addR.enPassant = "";
                    to_addR.ks_castle = false;
                    to_addR.qs_castle = false;
                    to_addR.piece = i;
                    to_addR.to = sq->ne;
                    to_addR.promotedTo = R;
                    legalMoves.push_back(to_addR);
                }
                else{
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->ne;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                }
            }
            // Capture west
                // Promotion
            if(sq->nw != nullptr && sq->nw->piece != nullptr && sq->nw->piece->owner == oppo_col){
                if(sq->rank == 6){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->nw;
                    to_add.promotedTo = Q;
                    legalMoves.push_back(to_add);

                    Move to_addN;
                    to_addN.enPassant = "";
                    to_addN.ks_castle = false;
                    to_addN.qs_castle = false;
                    to_addN.piece = i;
                    to_addN.to = sq->nw;
                    to_addN.promotedTo = N;
                    legalMoves.push_back(to_addN);

                    Move to_addB;
                    to_addB.enPassant = "";
                    to_addB.ks_castle = false;
                    to_addB.qs_castle = false;
                    to_addB.piece = i;
                    to_addB.to = sq->nw;
                    to_addB.promotedTo = B;
                    legalMoves.push_back(to_addB);

                    Move to_addR;
                    to_addR.enPassant = "";
                    to_addR.ks_castle = false;
                    to_addR.qs_castle = false;
                    to_addR.piece = i;
                    to_addR.to = sq->nw;
                    to_addR.promotedTo = R;
                    legalMoves.push_back(to_addR);
                }
                else{
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->nw;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                }
            }
            // Capture enpassant east
            if(sq->ne != nullptr && play_brd.getEnPassantLoc() != "" && sq->e->name == play_brd.getEnPassantLoc()){
                Move to_add;
                to_add.enPassant = sq->ne->name;
                to_add.ks_castle = false;
                to_add.qs_castle = false;
                to_add.piece = i;
                to_add.to = sq->ne;
                to_add.promotedTo = p;
                legalMoves.push_back(to_add);
            }
            // Capture enpassant west
            if(play_brd.getEnPassantLoc() != "" && sq->nw != nullptr && sq->w->name == play_brd.getEnPassantLoc()){
                Move to_add;
                to_add.enPassant = sq->nw->name;
                to_add.ks_castle = false;
                to_add.qs_castle = false;
                to_add.piece = i;
                to_add.to = sq->nw;
                to_add.promotedTo = p;
                legalMoves.push_back(to_add);
            }
        }
        else{
            // Can't push the pawn if it's blocked
            if(sq->s->piece == nullptr){
                // Can I promote the pawn?
                if(sq->rank == 1){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->s;
                    to_add.promotedTo = Q;
                    legalMoves.push_back(to_add);

                    Move to_addN;
                    to_addN.enPassant = "";
                    to_addN.ks_castle = false;
                    to_addN.qs_castle = false;
                    to_addN.piece = i;
                    to_addN.to = sq->s;
                    to_addN.promotedTo = N;
                    legalMoves.push_back(to_addN);

                    Move to_addB;
                    to_addB.enPassant = "";
                    to_addB.ks_castle = false;
                    to_addB.qs_castle = false;
                    to_addB.piece = i;
                    to_addB.to = sq->s;
                    to_addB.promotedTo = B;
                    legalMoves.push_back(to_addB);

                    Move to_addR;
                    to_addR.enPassant = "";
                    to_addR.ks_castle = false;
                    to_addR.qs_castle = false;
                    to_addR.piece = i;
                    to_addR.to = sq->s;
                    to_addR.promotedTo = R;
                    legalMoves.push_back(to_addR);
                }
                else{
                    // Add a one square pawn push
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->s;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                }
                // TODO
                // See if I can move the pawn two squares
                if(sq->rank == 6 && sq->s->s->piece == nullptr){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->s->s;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                }
            }
            // Deal with captures
            // Capture east
                // Promotion
            if(sq->se != nullptr && sq->se->piece != nullptr && sq->se->piece->owner == oppo_col){
                if(sq->rank == 1){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->se;
                    to_add.promotedTo = Q;
                    legalMoves.push_back(to_add);

                    Move to_addN;
                    to_addN.enPassant = "";
                    to_addN.ks_castle = false;
                    to_addN.qs_castle = false;
                    to_addN.piece = i;
                    to_addN.to = sq->se;
                    to_addN.promotedTo = N;
                    legalMoves.push_back(to_addN);

                    Move to_addB;
                    to_addB.enPassant = "";
                    to_addB.ks_castle = false;
                    to_addB.qs_castle = false;
                    to_addB.piece = i;
                    to_addB.to = sq->se;
                    to_addB.promotedTo = B;
                    legalMoves.push_back(to_addB);

                    Move to_addR;
                    to_addR.enPassant = "";
                    to_addR.ks_castle = false;
                    to_addR.qs_castle = false;
                    to_addR.piece = i;
                    to_addR.to = sq->se;
                    to_addR.promotedTo = R;
                    legalMoves.push_back(to_addR);
                }
                else{
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->se;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                }
            }
            // Capture west
                // Promotion
            if(sq->sw != nullptr && sq->sw->piece != nullptr && sq->sw->piece->owner == oppo_col){
                if(sq->rank == 1){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->sw;
                    to_add.promotedTo = Q;
                    legalMoves.push_back(to_add);

                    Move to_addN;
                    to_addN.enPassant = "";
                    to_addN.ks_castle = false;
                    to_addN.qs_castle = false;
                    to_addN.piece = i;
                    to_addN.to = sq->sw;
                    to_addN.promotedTo = N;
                    legalMoves.push_back(to_addN);

                    Move to_addB;
                    to_addB.enPassant = "";
                    to_addB.ks_castle = false;
                    to_addB.qs_castle = false;
                    to_addB.piece = i;
                    to_addB.to = sq->sw;
                    to_addB.promotedTo = B;
                    legalMoves.push_back(to_addB);

                    Move to_addR;
                    to_addR.enPassant = "";
                    to_addR.ks_castle = false;
                    to_addR.qs_castle = false;
                    to_addR.piece = i;
                    to_addR.to = sq->sw;
                    to_addR.promotedTo = R;
                    legalMoves.push_back(to_addR);
                }
                else{
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = sq->sw;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                }
            }
            // Capture enpassant east
            if(sq->se != nullptr && play_brd.getEnPassantLoc() != "" && sq->e->name == play_brd.getEnPassantLoc()){
                Move to_add;
                to_add.enPassant = sq->se->name;
                to_add.ks_castle = false;
                to_add.qs_castle = false;
                to_add.piece = i;
                to_add.to = sq->se;
                to_add.promotedTo = p;
                legalMoves.push_back(to_add);
            }
            // Capture enpassant west
            if(play_brd.getEnPassantLoc() != "" && sq->sw != nullptr && sq->w->name == play_brd.getEnPassantLoc()){
                Move to_add;
                to_add.enPassant = sq->sw->name;
                to_add.ks_castle = false;
                to_add.qs_castle = false;
                to_add.piece = i;
                to_add.to = sq->sw;
                to_add.promotedTo = p;
                legalMoves.push_back(to_add);
            }
        }
    }

    // Go through my non-castling king moves – make sure destination square isn't attacked
    Piece* king = piece_map[K].front();
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
    for(const auto i : piece_map[R]){
        for(int block_i = 0; block_i < 4; block_i++){
            Square* current = i->square;
            while(true){
                if(block_i == 0){
                    if(current->n == nullptr){
                        break;
                    }
                    else{
                        current = current->n;
                    }
                }
                if(block_i == 1){
                    if(current->s == nullptr){
                        break;
                    }
                    else{
                        current = current->s;
                    }
                }
                if(block_i == 2){
                    if(current->e == nullptr){
                        break;
                    }
                    else{
                        current = current->e;
                    }
                }
                if(block_i == 3){
                    if(current->w == nullptr){
                        break;
                    }
                    else{
                        current = current->w;
                    }
                }
                if(current->piece == nullptr){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = current;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                }
                else if(current->piece->owner == oppo_col){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = current;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                    break;
                }
                else{
                    break;
                }
            }
        }
    }
    // Go through my bishops
    for(const auto i : piece_map[B]){
        for(int block_i = 0; block_i < 4; block_i++){
            Square* current = i->square;
            while(true){
                if(block_i == 0){
                    if(current->ne == nullptr){
                        break;
                    }
                    else{
                        current = current->ne;
                    }
                }
                if(block_i == 1){
                    if(current->se == nullptr){
                        break;
                    }
                    else{
                        current = current->se;
                    }
                }
                if(block_i == 2){
                    if(current->nw == nullptr){
                        break;
                    }
                    else{
                        current = current->nw;
                    }
                }
                if(block_i == 3){
                    if(current->sw == nullptr){
                        break;
                    }
                    else{
                        current = current->sw;
                    }
                }
                if(current->piece == nullptr){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = current;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                }
                else if(current->piece->owner == oppo_col){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = current;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                    break;
                }
                else{
                    break;
                }
            }
        }
    }
    // Go through my queens
    for(const auto i : piece_map[Q]){
        for(int block_i = 0; block_i < 8; block_i++){
            Square* current = i->square;
            while(true){
                if(block_i == 0){
                    if(current->ne == nullptr){
                        break;
                    }
                    else{
                        current = current->ne;
                    }
                }
                if(block_i == 1){
                    if(current->se == nullptr){
                        break;
                    }
                    else{
                        current = current->se;
                    }
                }
                if(block_i == 2){
                    if(current->nw == nullptr){
                        break;
                    }
                    else{
                        current = current->nw;
                    }
                }
                if(block_i == 3){
                    if(current->sw == nullptr){
                        break;
                    }
                    else{
                        current = current->sw;
                    }
                }
                if(block_i == 4){
                    if(current->n == nullptr){
                        break;
                    }
                    else{
                        current = current->n;
                    }
                }
                if(block_i == 5){
                    if(current->s == nullptr){
                        break;
                    }
                    else{
                        current = current->s;
                    }
                }
                if(block_i == 6){
                    if(current->e == nullptr){
                        break;
                    }
                    else{
                        current = current->e;
                    }
                }
                if(block_i == 7){
                    if(current->w == nullptr){
                        break;
                    }
                    else{
                        current = current->w;
                    }
                }
                if(current->piece == nullptr){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = current;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                }
                else if(current->piece->owner == oppo_col){
                    Move to_add;
                    to_add.enPassant = "";
                    to_add.ks_castle = false;
                    to_add.qs_castle = false;
                    to_add.piece = i;
                    to_add.to = current;
                    to_add.promotedTo = p;
                    legalMoves.push_back(to_add);
                    break;
                }
                else{
                    break;
                }
            }
        }
    }
    // Go through my knights
    for(const auto i : piece_map[N]){
        Square* sq = i->square;
        if(sq->ne != nullptr && sq->ne->n != nullptr && (sq->ne->n->piece == nullptr || sq->ne->n->piece->owner == oppo_col)){
            Move to_add;
            to_add.enPassant = "";
            to_add.ks_castle = false;
            to_add.qs_castle = false;
            to_add.piece = i;
            to_add.to = sq->ne->n;
            to_add.promotedTo = p;
            legalMoves.push_back(to_add);
        }
        if(sq->nw != nullptr && sq->nw->n != nullptr && (sq->nw->n->piece == nullptr || sq->nw->n->piece->owner == oppo_col)){
            Move to_add;
            to_add.enPassant = "";
            to_add.ks_castle = false;
            to_add.qs_castle = false;
            to_add.piece = i;
            to_add.to = sq->nw->n;
            to_add.promotedTo = p;
            legalMoves.push_back(to_add);
        }
        if(sq->ne != nullptr && sq->ne->e != nullptr && (sq->ne->e->piece == nullptr || sq->ne->e->piece->owner == oppo_col)){
            Move to_add;
            to_add.enPassant = "";
            to_add.ks_castle = false;
            to_add.qs_castle = false;
            to_add.piece = i;
            to_add.to = sq->ne->e;
            to_add.promotedTo = p;
            legalMoves.push_back(to_add);
        }
        if(sq->nw != nullptr && sq->nw->w != nullptr && (sq->nw->w->piece == nullptr || sq->nw->w->piece->owner == oppo_col)){
            Move to_add;
            to_add.enPassant = "";
            to_add.ks_castle = false;
            to_add.qs_castle = false;
            to_add.piece = i;
            to_add.to = sq->nw->w;
            to_add.promotedTo = p;
            legalMoves.push_back(to_add);
        }
        if(sq->se != nullptr && sq->se->s != nullptr && (sq->se->s->piece == nullptr || sq->se->s->piece->owner == oppo_col)){
            Move to_add;
            to_add.enPassant = "";
            to_add.ks_castle = false;
            to_add.qs_castle = false;
            to_add.piece = i;
            to_add.to = sq->se->s;
            to_add.promotedTo = p;
            legalMoves.push_back(to_add);
        }
        if(sq->sw != nullptr && sq->sw->s != nullptr && (sq->sw->s->piece == nullptr || sq->sw->s->piece->owner == oppo_col)){
            Move to_add;
            to_add.enPassant = "";
            to_add.ks_castle = false;
            to_add.qs_castle = false;
            to_add.piece = i;
            to_add.to = sq->sw->s;
            to_add.promotedTo = p;
            legalMoves.push_back(to_add);
        }
        if(sq->sw != nullptr && sq->sw->w != nullptr && (sq->sw->w->piece == nullptr || sq->sw->w->piece->owner == oppo_col)){
            Move to_add;
            to_add.enPassant = "";
            to_add.ks_castle = false;
            to_add.qs_castle = false;
            to_add.piece = i;
            to_add.to = sq->sw->w;
            to_add.promotedTo = p;
            legalMoves.push_back(to_add);
        }
        if(sq->se != nullptr && sq->se->e != nullptr && (sq->se->e->piece == nullptr || sq->se->e->piece->owner == oppo_col)){
            Move to_add;
            to_add.enPassant = "";
            to_add.ks_castle = false;
            to_add.qs_castle = false;
            to_add.piece = i;
            to_add.to = sq->se->e;
            to_add.promotedTo = p;
            legalMoves.push_back(to_add);
        }
    }

    // Make sure that none of the moves result in check
    std::vector<Move> filteredLegalMoves;
    for(const auto i : legalMoves){
        Board checkCheck(play_brd);
        Move checkMove;
        checkMove.ks_castle = i.ks_castle;
        checkMove.qs_castle = i.qs_castle;
        checkMove.piece = (*checkCheck.getSquareMap())[i.piece->square->name].piece;
        checkMove.promotedTo = i.promotedTo;
        checkMove.to = &(*checkCheck.getSquareMap())[i.to->name];
        checkMove.enPassant = "";
        checkCheck.executeMove(checkMove);
        if(!checkCheck.inCheck(i.piece->owner)){
            filteredLegalMoves.push_back(i);
        }
    }

    return filteredLegalMoves;
}

std::string Cpu::getMoveName(Move& move){
    return "e4";  // placeholder
}