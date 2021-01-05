#include "board.h"
#include "chess.h"
#include <string>
#include <iostream>
#include <unordered_map>

Board::Board(){
    move_color = white;
    move_num = 1;
    check = false;
    white_ks_castle_eligible = true;
    black_ks_castle_eligible = true;
    white_qs_castle_eligible = true;
    black_qs_castle_eligible = true;

    // Initialize board
    sq_colors_t clr_track = dark;
    for(int rank = 0; rank<8; rank++){
        for(char file = 'a'; file<'i'; file++){
            std::string key = "";
            key += file;
            key += std::to_string(rank + 1);

            Square new_square;
            new_square.file = file;
            new_square.rank = rank;
            new_square.name = key;
            new_square.clr = clr_track;

            if(file != 'i'){
                clr_track = clr_track == dark ? light : dark;
            }

            if(rank == 6 || rank == 1){
                Piece* pie_ptr = new Piece();
                pie_ptr->square = &new_square;
                pie_ptr->type = p;
                pie_ptr->owner = rank == 6 ? black : white;
                new_square.piece = pie_ptr;
            }
            else if(rank == 0 || rank == 7){
                Piece* pie_ptr = new Piece();
                switch(file){
                    case 'a':
                    case 'h':
                        pie_ptr->type = R;
                        break;
                    case 'b':
                    case 'g':
                        pie_ptr->type = N;
                        break;
                    case 'c':
                    case 'f':
                        pie_ptr->type = B;
                        break;
                    case 'd':
                        pie_ptr->type = Q;
                        break;
                    case 'e':
                        pie_ptr->type = K;
                }
                pie_ptr->owner = rank == 7 ? black : white;
                new_square.piece = pie_ptr;
            }
            else{
                new_square.piece = nullptr;
            }
            square_map[key] = new_square;
            if(square_map[key].piece != nullptr){
                square_map[key].piece->square = &square_map[key];
            }
        }
    }

    // Now add attributes that point to other squares
    for(int rank = 0; rank<8; rank++){
        for(char file = 'a'; file<'i'; file++){
            std::string key = "";
            key += file;
            key += std::to_string(rank + 1);

            std::string nkey = "";
            nkey += file;
            nkey += std::to_string(rank + 2);
            square_map[key].n = rank == 7 ? nullptr : &(square_map[nkey]);

            std::string skey = "";
            skey += file;
            skey += std::to_string(rank);
            square_map[key].s = rank == 0 ? nullptr : &(square_map[skey]);

            std::string ekey = "";
            ekey += (file + 1);
            ekey += std::to_string(rank + 1);
            square_map[key].e = file == 'h' ? nullptr : &(square_map[ekey]);

            std::string wkey = "";
            wkey += (file - 1);
            wkey += std::to_string(rank + 1);
            square_map[key].w = file == 'a' ? nullptr : &(square_map[wkey]);

            std::string nekey = "";
            nekey += (file + 1);
            nekey += std::to_string(rank + 2);
            square_map[key].ne = (rank == 7 || file == 'h') ? nullptr : &(square_map[nekey]);

            std::string nwkey = "";
            nwkey += (file - 1);
            nwkey += std::to_string(rank + 2);
            square_map[key].nw = (rank == 7 || file == 'a') ? nullptr : &(square_map[nwkey]);

            std::string sekey = "";
            sekey += (file + 1);
            sekey += std::to_string(rank);
            square_map[key].se = (rank == 0 || file == 'h') ? nullptr : &(square_map[sekey]);

            std::string swkey = "";
            swkey += (file - 1);
            swkey += std::to_string(rank);
            square_map[key].sw = (rank == 0 || file == 'a') ? nullptr : &(square_map[swkey]);
        }
    }

    // Now put together the piece list
    Piece* current_piece;
    for(int rank = 0; rank<8; rank++){
        for(char file = 'a'; file<'i'; file++){
            std::string key = "";
            key += file;
            key += std::to_string(rank + 1);

            if(rank == 0 && file == 'a'){
                // Should be a rook
                head_piece.next = square_map[key].piece;
                head_piece.prev = nullptr;
                current_piece = head_piece.next;
                current_piece->prev = &head_piece;
            }
            else if(square_map[key].piece != nullptr){
                current_piece->next = square_map[key].piece;
                current_piece->next->prev = current_piece;
                current_piece = current_piece->next;
            }
        }
    }

    // Initialize piece_map
    current_piece = &head_piece;
    while(current_piece->next != nullptr){
        current_piece = current_piece->next;
        piece_map[current_piece->type].push_front(current_piece);
    }
}

void Board::makeMove(){

}

bool Board::isLegal(std::string move){
    return true;  // placeholder
}

struct Move Board::parseMove(std::string move){
    Move parsedMove;

    // TODO: Custom exceptions

    // If the move is < 2 chars, throw it out.
    if(move.length() < 2){
        throw "Move illformed";
    }

    // Is the move ks castles?
    if(move == "O-O" || move == "0-0"){
        parsedMove.ks_castle = true;
        parsedMove.qs_castle = false;
        for(std::list<Piece*>::iterator it = piece_map[K].begin(); it != piece_map[K].end(); it++){
            if(move_color == (*it)->owner){
                parsedMove.piece = *it;
            }
        }
        if(move_color == white){
            parsedMove.to = &square_map["g1"];
        }
        else{
            parsedMove.to = &square_map["g8"];
        }
        return parsedMove;
    }

    // Is the move qs castles?
    if(move == "O-O-O" || move == "0-0-0"){
        parsedMove.ks_castle = false;
        parsedMove.qs_castle = true;
        for(std::list<Piece*>::iterator it = piece_map[K].begin(); it != piece_map[K].end(); it++){
            if(move_color == (*it)->owner){
                parsedMove.piece = *it;
            }
        }
        if(move_color == white){
            parsedMove.to = &square_map["c1"];
        }
        else{
            parsedMove.to = &square_map["c8"];
        }
        return parsedMove;
    }

    // Is it a pawn move?
    if(std::islower(move[0])){
        // Is it a pawn push/promotion?
        if(std::isdigit(move[1])){
            // Is it a pawn promotion? (of the form e8Q)
            if(move.length() == 3){
                // Has to be an actual file
                if(move[0] > 'h' || move[0] < 'a'){
                    throw "Move illformed";
                }
                // Has to be to the last rank
                if(move_color == white && move[1] != '8'){
                    throw "Move illformed";
                }
                if(move_color == black && move[1] != '1'){
                    throw "Move illformed";
                }
                // last char has to be N, B, Q, or R
                if(!(move[2] == 'N' || move[2] == 'Q' || move[2] == 'R' || move[2] == 'B')){
                    throw "Move illformed";
                }
                // Find the pawn being moved
                Square* current_square = move_color == white ? square_map[move.substr(0, 2)].s : square_map[move.substr(0, 2)].n;
                while(true){
                    if(current_square->piece != nullptr && current_square->piece->type == p && current_square->piece->owner == move_color){
                        parsedMove.piece = current_square->piece;
                        break;
                    }
                    if(move_color == white){
                        if(current_square->s == nullptr){
                            throw "Move illformed";
                        }
                        current_square = current_square->s;
                    }
                    else{
                        if(current_square->n == nullptr){
                            throw "Move illformed";
                        }
                        current_square = current_square->n;
                    }
                }
                // Enter promotedTo
                switch(move[2]){
                    case 'Q':
                        parsedMove.promotedTo = Q;
                        break;
                    case 'B':
                        parsedMove.promotedTo = B;
                        break;
                    case 'N':
                        parsedMove.promotedTo = N;
                        break;
                    case 'R':
                        parsedMove.promotedTo = R;
                        break;
                }
                parsedMove.ks_castle = false;
                parsedMove.qs_castle = false;
                parsedMove.to = &square_map[move.substr(0, 2)];
                return parsedMove; 
            }
            // Pawn push
            else{
                // Pawn push can't be more than 2 characters
                if(move.length() > 2){
                    throw "Move illformed";
                }
                // Has to actually represent a square
                if(square_map.count(move) == 0){
                    throw "Move illformed";
                }
                // Can't be _1, _2 or _8 as white; can't be _7, _8, or _1 as black
                if(move_color == white){
                    if(move[1] == '1' || move[1] == '2' || move[1] == '8'){
                        throw "Move illformed";
                    }
                }
                else{
                    if(move[1] == '7' || move[1] == '8' || move[1] == '1'){
                        throw "Move illformed";
                    }
                }
                // Find the pawn being moved
                Square* current_square = move_color == white ? square_map[move].s : square_map[move].n;
                while(true){
                    if(current_square->piece != nullptr && current_square->piece->type == p && current_square->piece->owner == move_color){
                        parsedMove.piece = current_square->piece;
                        break;
                    }
                    if(move_color == white){
                        if(current_square->s == nullptr){
                            throw "Move illformed";
                        }
                        current_square = current_square->s;
                    }
                    else{
                        if(current_square->n == nullptr){
                            throw "Move illformed";
                        }
                        current_square = current_square->n;
                    }
                }
                parsedMove.ks_castle = false;
                parsedMove.qs_castle = false;
                parsedMove.to = &square_map[move];
                return parsedMove;
            }
        }
        // Is it a pawn capture?
        if(move[1] == 'x'){
            // At this point, it must be 4 characters, i.e. exd4
            if(move.length() != 4){
                throw "Move illformed";
            }
            // First char has to be an actual file
            if(move[0] < 'a' || move[0] > 'h'){
                throw "Move illformed";
            }
            // Third and fourth chars must be an actual location
            if(move[2] < 'a' || move[2] > 'h' || move[3] < '1' || move[3] > '8'){
                throw "Move illformed";
            }
            // Find the implied position of the pawn being moved
            char file = move[0];
            char rank = move_color == white ? move[3] - 1 : move[3] + 1;
            std::string loc_key = "";
            loc_key += file;
            loc_key += rank;
            if(square_map[loc_key].piece == nullptr || square_map[loc_key].piece->type != p){
                throw "Move illformed";
            }
            else{
                parsedMove.piece = square_map[loc_key].piece;
            }

            parsedMove.ks_castle = false;
            parsedMove.qs_castle = false;
            parsedMove.to = &square_map[move.substr(2, 2)];
            return parsedMove;
        }
        else{
            throw "Move illformed";
        }

    }

    // Is it a king move?
    if(move[0] == 'K'){
        // Has to be three chars
        if(move.length() != 3){
            throw "Move illformed";
        }
        // Has to be an actual location
        if(move[1] < 'a' || move[1] > 'h' || move[2] < '1' || move[2] > '8'){
            throw "Move illformed";
        }
        if(piece_map[K].front()->owner == move_color){
            parsedMove.piece = piece_map[K].front();
        }
        else{
            parsedMove.piece = piece_map[K].back();
        }
        parsedMove.ks_castle = false;
        parsedMove.qs_castle = false;
        parsedMove.to = &square_map[move.substr(1, 2)];
        return parsedMove;
    }

    throw "Move illformed";
}

void Board::setMoveColor(colors_t col){
    move_color = col;
}

Piece* Board::getHeadPiece(){
    return &head_piece;
}

std::unordered_map<std::string, Square, CustomHash> Board::getSquareMap(){
    return square_map;
}

std::unordered_map< piece_t, std::list<Piece*>, std::hash<int> > Board::getPieceMap(){
    return piece_map;
}

