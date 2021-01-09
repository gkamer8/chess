#include "board.h"
#include "chess.h"
#include <string>
#include <iostream>
#include <unordered_map>

Board::Board(){
    move_color = white;
    move_num = 1;
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

    // Initialize piece_maps
    for(int rank = 0; rank<8; rank++){
        for(char file = 'a'; file<'i'; file++){
            std::string key = "";
            key += file;
            key += std::to_string(rank + 1);
            if(square_map[key].piece != nullptr){
                if(square_map[key].piece->owner == white){
                    white_piece_map[square_map[key].piece->type].push_front(square_map[key].piece);
                }
                else{
                    black_piece_map[square_map[key].piece->type].push_front(square_map[key].piece);
                }
            }
        }
    }
}

// Copy constructor
Board::Board(Board& brd){
    move_color = brd.move_color;
    move_num = brd.move_num;
    white_ks_castle_eligible = brd.white_ks_castle_eligible;
    black_ks_castle_eligible = brd.black_ks_castle_eligible;
    white_qs_castle_eligible = brd.white_qs_castle_eligible;
    black_qs_castle_eligible = brd.black_qs_castle_eligible;
    square_map = *(brd.getSquareMap());
    black_piece_map = *(brd.getBlackPieceMap());
    white_piece_map = *(brd.getWhitePieceMap());
    moves = brd.moves;
}

// Note: might throw an error if asked to castle and it can't
void Board::executeMove(Move& move){

    move.piece->square->piece = nullptr;
    move.piece->square = move.to;
    move.to->piece = move.piece;

    // Move the rook if it's a castle
    if(move.ks_castle){
        if(move_color == white){
            square_map["f1"].piece = square_map["h1"].piece;
            square_map["h1"].piece = nullptr;
            square_map["f1"].piece->square = &square_map["f1"];
        }
        else{
            square_map["f8"].piece = square_map["h8"].piece;
            square_map["h8"].piece = nullptr;
            square_map["f8"].piece->square = &square_map["f8"];
        }
        return;
    }
    else if(move.qs_castle){
        if(move_color == white){
            square_map["c1"].piece = square_map["a1"].piece;
            square_map["a1"].piece = nullptr;
            square_map["c1"].piece->square = &square_map["c1"];
        }
        else{
            square_map["c8"].piece = square_map["a8"].piece;
            square_map["a8"].piece = nullptr;
            square_map["c8"].piece->square = &square_map["c8"];
        }
        return;
    }

    if(move.promotedTo != p){
        move.piece->type = move.promotedTo;
    }
}

void Board::makeMove(std::string move_str){
    Move move = parseMove(move_str);  // This could throw a "Move illformed"

    // If illegal, end
    if(!isLegal(move)){
        throw "Illegal move";
    }

    // After:
    // See if it ends the game
    // Set castle eligibility appropriately
    // Increment move number
    // Set move color
    // Add to moves vector
}

bool Board::isLegal(Move& move){
    // First take care of the castling situation
    if(move.ks_castle){
        if(move_color == white){
            if(!white_ks_castle_eligible){
                return false;
            }
            // There must be open space
            if(square_map["g1"].piece != nullptr){
                return false;
            }
            if(square_map["f1"].piece != nullptr){
                return false;
            }
            // Can't castle through check
            // Create copy board, move the king to f1, and see if it's in check
            Board checkCheck(*this);
            Move kingMove;
            kingMove.qs_castle = false;
            kingMove.ks_castle;
            kingMove.piece = white_piece_map[K].front();
            kingMove.to = &square_map["f1"];
            kingMove.promotedTo = p;
            checkCheck.executeMove(kingMove);
            if(checkCheck.inCheck(move_color)){
                return false;
            }
        }
        else{
            if(!black_ks_castle_eligible){
                return false;
            }
            // There must be open space
            if(square_map["g8"].piece != nullptr){
                return false;
            }
            if(square_map["f8"].piece != nullptr){
                return false;
            }
            // Can't castle through check
            // Create copy board, move the king to f1, and see if it's in check
            Board checkCheck(*this);
            Move kingMove;
            kingMove.qs_castle = false;
            kingMove.ks_castle;
            kingMove.piece = white_piece_map[K].front();
            kingMove.to = &square_map["f8"];
            kingMove.promotedTo = p;
            checkCheck.executeMove(kingMove);
            if(checkCheck.inCheck(move_color)){
                return false;
            }
        }
    }
    else if(move.qs_castle){
        if(move_color == white){
            if(!white_qs_castle_eligible){
                return false;
            }
            // There must be open space
            if(square_map["c1"].piece != nullptr){
                return false;
            }
            if(square_map["d1"].piece != nullptr){
                return false;
            }
            // Can't castle through check
            // Create copy board, move the king to f1, and see if it's in check
            Board checkCheck(*this);
            Move kingMove;
            kingMove.qs_castle = false;
            kingMove.ks_castle;
            kingMove.piece = white_piece_map[K].front();
            kingMove.to = &square_map["d1"];
            kingMove.promotedTo = p;
            checkCheck.executeMove(kingMove);
            if(checkCheck.inCheck(move_color)){
                return false;
            }
        }
        else{
            if(!black_qs_castle_eligible){
                return false;
            }
            // There must be open space
            if(square_map["c8"].piece != nullptr){
                return false;
            }
            if(square_map["d8"].piece != nullptr){
                return false;
            }
            // Can't castle through check
            // Create copy board, move the king to f1, and see if it's in check
            Board checkCheck(*this);
            Move kingMove;
            kingMove.qs_castle = false;
            kingMove.ks_castle;
            kingMove.piece = white_piece_map[K].front();
            kingMove.to = &square_map["d8"];
            kingMove.promotedTo = p;
            checkCheck.executeMove(kingMove);
            if(checkCheck.inCheck(move_color)){
                return false;
            }
        }
    }
    // Execute the move on a fake board – can't put the mover in check
    Board checkCheck(*this);
    checkCheck.executeMove(move);
    if(checkCheck.inCheck(move_color)){
        return false;
    }

    return true;
}

struct Move Board::parseMove(std::string move){
    Move parsedMove;
    parsedMove.promotedTo = p;  // Like setting it to null – can never be promoted to pawn

    // TODO: Custom exceptions

    // If the move is < 2 chars, throw it out.
    if(move.length() < 2){
        throw "Move illformed";
    }

    // Is the move ks castles?
    if(move == "O-O" || move == "0-0"){
        parsedMove.ks_castle = true;
        parsedMove.qs_castle = false;
        if(move_color == white){
            parsedMove.piece = white_piece_map[K].front();
            parsedMove.to = &square_map["g1"];
        }
        else{
            parsedMove.piece = black_piece_map[K].front();
            parsedMove.to = &square_map["g8"];
        }
        return parsedMove;
    }

    // Is the move qs castles?
    if(move == "O-O-O" || move == "0-0-0"){
        parsedMove.ks_castle = false;
        parsedMove.qs_castle = true;
        if(move_color == white){
            parsedMove.piece = white_piece_map[K].front();
            parsedMove.to = &square_map["c1"];
        }
        else{
            parsedMove.piece = black_piece_map[K].front();
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
                if(current_square->piece == nullptr){
                    // No piece
                    throw "Move illformed";
                }
                else if(current_square->piece->type != p){
                    // There's a piece but not a pawn
                    throw "Move illformed";
                }
                else if(current_square->piece->owner != move_color){
                    // The pawn is not the right kind
                    throw "Move illformed";
                }
                else{
                    parsedMove.piece = current_square->piece;
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
                if(current_square->piece != nullptr && current_square->piece->type == p && current_square->piece->owner == move_color){
                    parsedMove.piece = current_square->piece;
                }
                else if(current_square->piece == nullptr){
                    if(move_color == white){
                        if(current_square->s->piece != nullptr && current_square->s->piece->type == p && current_square->s->piece->owner == white){
                            parsedMove.piece = current_square->s->piece;
                        }
                        else{
                            throw "Move illformed";
                        }
                    }
                    if(move_color == black){
                        if(current_square->n->piece != nullptr && current_square->n->piece->type == p && current_square->n->piece->owner == black){
                            parsedMove.piece = current_square->n->piece;
                        }
                        else{
                            throw "Move illformed";
                        }
                    }
                }
                else{
                    // Something blocking the pawn
                    throw "Move illformed";
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
            // Third and fourth chars must be an actual location above 2
            if(move[2] < 'a' || move[2] > 'h' || move[3] <= '2' || move[3] > '8'){
                throw "Move illformed";
            }
            // Find the implied position of the pawn being moved
            char file = move[0];
            char rank = move_color == white ? move[3] - 1 : move[3] + 1;
            std::string loc_key = "";
            loc_key += file;
            loc_key += rank;
            if(square_map[loc_key].piece == nullptr || square_map[loc_key].piece->type != p || square_map[loc_key].piece->owner != move_color){
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
        if(move[1] == 'x'){
            move.erase(1, 1);
        }
        // Has to be three chars
        if(move.length() != 3){
            throw "Move illformed";
        }
        // Has to be an actual location
        if(move[1] < 'a' || move[1] > 'h' || move[2] < '1' || move[2] > '8'){
            throw "Move illformed";
        }
        // King has to be next to the target square
        std::string key = "";
        key += move[1];
        key += move[2];
        if(square_map[key].s != nullptr && square_map[key].s->piece != nullptr && square_map[key].s->piece->type == K && square_map[key].s->piece->owner == move_color){
            parsedMove.piece = square_map[key].s->piece;
        }
        else if(square_map[key].w != nullptr && square_map[key].w->piece != nullptr && square_map[key].w->piece->type == K && square_map[key].w->piece->owner == move_color){
            parsedMove.piece = square_map[key].w->piece;
        }
        else if(square_map[key].n != nullptr && square_map[key].n->piece != nullptr && square_map[key].n->piece->type == K && square_map[key].n->piece->owner == move_color){
            parsedMove.piece = square_map[key].n->piece;
        }
        else if(square_map[key].e != nullptr && square_map[key].e->piece != nullptr && square_map[key].e->piece->type == K && square_map[key].e->piece->owner == move_color){
            parsedMove.piece = square_map[key].e->piece;
        }
        else{
            throw "Move illformed";
        }
        parsedMove.ks_castle = false;
        parsedMove.qs_castle = false;
        parsedMove.to = &square_map[move.substr(1, 2)];
        return parsedMove;
    }

    // Is it a Queen move?
    if(move[0] == 'Q'){
        // Get rid of captures sign
        move.erase(std::remove(move.begin(), move.end(), 'x'), move.end());
        // Get where the move is to
        std::string loc_key = "";
        if(move[move.size() - 2] < 'a' || move[move.size() - 2] > 'h'){
            throw "Move illformed";
        }
        else{
            loc_key += move[move.size() - 2];
        }
        if(move[move.size() - 1] < '1' || move[move.size() - 1] > '8'){
            throw "Move illformed";
        }
        else{
            loc_key += move[move.size() - 1];
        }
        // Deal with disambiguations
        char disambiguated_file = '\0';
        char disambiguated_rank = '\0';
        if(move.size() == 4){
            if(move[1] >= 'a' && move[1] <= 'h'){
                disambiguated_file = move[1];
            }
            else if(move[1] >= '1' && move[1] <= '8'){
                disambiguated_rank = move[1];
            }
            else{
                throw "Move illformed";
            }
        }
        else if(move.size() == 5){
            if(move[1] >= 'a' && move[1] <= 'h'){
                disambiguated_file = move[1];
            }
            else{
                throw "Move illformed";
            }
            if(move[2] >= '1' && move[2] <= '8'){
                disambiguated_rank = move[1];
            }
            else{
                throw "Move illformed";
            }
        }
        // The square can't be occupied by your own piece
        if(square_map[loc_key].piece != nullptr && square_map[loc_key].piece->owner == move_color){
            throw "Move illformed";
        }
        // Find the correct queen
        Square* current;
        bool found = false;
        // Check north
        current = &square_map[loc_key];
        while(!found && current->n != nullptr){
            current = current->n;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == Q){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check south
        current = &square_map[loc_key];
        while(!found && current->s != nullptr){
            current = current->s;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == Q){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check west
        current = &square_map[loc_key];
        while(!found && current->w != nullptr){
            current = current->w;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == Q){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check east
        current = &square_map[loc_key];
        while(!found && current->e != nullptr){
            current = current->e;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == Q){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check ne
        current = &square_map[loc_key];
        while(!found && current->n != nullptr && current->n->e != nullptr){
            current = current->n->e;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == Q){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check nw
        current = &square_map[loc_key];
        while(!found && current->n != nullptr && current->n->w != nullptr){
            current = current->n->w;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == Q){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check se
        current = &square_map[loc_key];
        while(!found && current->s != nullptr && current->s->e != nullptr){
            current = current->s->e;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == Q){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check sw
        current = &square_map[loc_key];
        while(!found && current->s != nullptr && current->s->w != nullptr){
            current = current->s->w;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == Q){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't have a piece blocking the square.
            }
        }
        if(!found){
            throw "Move illformed";
        }
        parsedMove.ks_castle = false;
        parsedMove.qs_castle = false;
        parsedMove.to = &square_map[loc_key];
        return parsedMove;
    }

    // Bishop move
    if(move[0] == 'B'){
        // Get rid of captures sign
        move.erase(std::remove(move.begin(), move.end(), 'x'), move.end());
        // Get where the move is to
        std::string loc_key = "";
        if(move[move.size() - 2] < 'a' || move[move.size() - 2] > 'h'){
            throw "Move illformed";
        }
        else{
            loc_key += move[move.size() - 2];
        }
        if(move[move.size() - 1] < '1' || move[move.size() - 1] > '8'){
            throw "Move illformed";
        }
        else{
            loc_key += move[move.size() - 1];
        }
        // Deal with disambiguations
        char disambiguated_file = '\0';
        char disambiguated_rank = '\0';
        if(move.size() == 4){
            if(move[1] >= 'a' && move[1] <= 'h'){
                disambiguated_file = move[1];
            }
            else if(move[1] >= '1' && move[1] <= '8'){
                disambiguated_rank = move[1];
            }
            else{
                throw "Move illformed";
            }
        }
        else if(move.size() == 5){
            if(move[1] >= 'a' && move[1] <= 'h'){
                disambiguated_file = move[1];
            }
            else{
                throw "Move illformed";
            }
            if(move[2] >= '1' && move[2] <= '8'){
                disambiguated_rank = move[1];
            }
            else{
                throw "Move illformed";
            }
        }
        // The square can't be occupied by your own piece
        if(square_map[loc_key].piece != nullptr && square_map[loc_key].piece->owner == move_color){
            throw "Move illformed";
        }
        // Find the correct Bishop
        Square* current;
        bool found = false;
        // Check ne
        current = &square_map[loc_key];
        while(!found && current->n != nullptr && current->n->e != nullptr){
            current = current->n->e;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == B){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check nw
        current = &square_map[loc_key];
        while(!found && current->n != nullptr && current->n->w != nullptr){
            current = current->n->w;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == B){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check se
        current = &square_map[loc_key];
        while(!found && current->s != nullptr && current->s->e != nullptr){
            current = current->s->e;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == B){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check sw
        current = &square_map[loc_key];
        while(!found && current->s != nullptr && current->s->w != nullptr){
            current = current->s->w;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == B){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't have a piece blocking the square.
            }
        }
        if(!found){
            throw "Move illformed";
        }
        parsedMove.ks_castle = false;
        parsedMove.qs_castle = false;
        parsedMove.to = &square_map[loc_key];
        return parsedMove;
    }

    // Rook move
    if(move[0] == 'R'){
        // Get rid of captures sign
        move.erase(std::remove(move.begin(), move.end(), 'x'), move.end());
        // Get where the move is to
        std::string loc_key = "";
        if(move[move.size() - 2] < 'a' || move[move.size() - 2] > 'h'){
            throw "Move illformed";
        }
        else{
            loc_key += move[move.size() - 2];
        }
        if(move[move.size() - 1] < '1' || move[move.size() - 1] > '8'){
            throw "Move illformed";
        }
        else{
            loc_key += move[move.size() - 1];
        }
        // Deal with disambiguations
        char disambiguated_file = '\0';
        char disambiguated_rank = '\0';
        if(move.size() == 4){
            if(move[1] >= 'a' && move[1] <= 'h'){
                disambiguated_file = move[1];
            }
            else if(move[1] >= '1' && move[1] <= '8'){
                disambiguated_rank = move[1];
            }
            else{
                throw "Move illformed";
            }
        }
        else if(move.size() == 5){
            if(move[1] >= 'a' && move[1] <= 'h'){
                disambiguated_file = move[1];
            }
            else{
                throw "Move illformed";
            }
            if(move[2] >= '1' && move[2] <= '8'){
                disambiguated_rank = move[1];
            }
            else{
                throw "Move illformed";
            }
        }
        // The square can't be occupied by your own piece
        if(square_map[loc_key].piece != nullptr && square_map[loc_key].piece->owner == move_color){
            throw "Move illformed";
        }
        // Find the correct Rook
        Square* current;
        bool found = false;
        // Check north
        current = &square_map[loc_key];
        while(!found && current->n != nullptr){
            current = current->n;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == R){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check south
        current = &square_map[loc_key];
        while(!found && current->s != nullptr){
            current = current->s;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == R){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check west
        current = &square_map[loc_key];
        while(!found && current->w != nullptr){
            current = current->w;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == R){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check east
        current = &square_map[loc_key];
        while(!found && current->e != nullptr){
            current = current->e;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == R){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        if(!found){
            throw "Move illformed";
        }
        parsedMove.ks_castle = false;
        parsedMove.qs_castle = false;
        parsedMove.to = &square_map[loc_key];
        return parsedMove;
    }

    // Knight move
    if(move[0] == 'N'){
        // Get rid of captures sign
        move.erase(std::remove(move.begin(), move.end(), 'x'), move.end());
        // Get where the move is to
        std::string loc_key = "";
        if(move[move.size() - 2] < 'a' || move[move.size() - 2] > 'h'){
            throw "Move illformed";
        }
        else{
            loc_key += move[move.size() - 2];
        }
        if(move[move.size() - 1] < '1' || move[move.size() - 1] > '8'){
            throw "Move illformed";
        }
        else{
            loc_key += move[move.size() - 1];
        }
        // Deal with disambiguations
        char disambiguated_file = '\0';
        char disambiguated_rank = '\0';
        if(move.size() == 4){
            if(move[1] >= 'a' && move[1] <= 'h'){
                disambiguated_file = move[1];
            }
            else if(move[1] >= '1' && move[1] <= '8'){
                disambiguated_rank = move[1];
            }
            else{
                throw "Move illformed";
            }
        }
        else if(move.size() == 5){
            if(move[1] >= 'a' && move[1] <= 'h'){
                disambiguated_file = move[1];
            }
            else{
                throw "Move illformed";
            }
            if(move[2] >= '1' && move[2] <= '8'){
                disambiguated_rank = move[1];
            }
            else{
                throw "Move illformed";
            }
        }
        // The square can't be occupied by your own piece
        if(square_map[loc_key].piece != nullptr && square_map[loc_key].piece->owner == move_color){
            throw "Move illformed";
        }
        // Find the correct Knight
        Square* current;
        bool found = false;
        // Check nne
        current = &square_map[loc_key];
        while(!found && current->n != nullptr && current->n->n != nullptr && current->n->n->e != nullptr){
            current = current->n->n->e;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == N){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check nnw
        current = &square_map[loc_key];
        while(!found && current->n != nullptr && current->n->n != nullptr && current->n->n->w != nullptr){
            current = current->n->n->w;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == N){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check nee
        current = &square_map[loc_key];
        while(!found && current->n != nullptr && current->n->e != nullptr && current->n->e->e != nullptr){
            current = current->n->e->e;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == N){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check nww
        current = &square_map[loc_key];
        while(!found && current->n != nullptr && current->n->w != nullptr && current->n->w->w != nullptr){
            current = current->n->w->w;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == N){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check see
        current = &square_map[loc_key];
        while(!found && current->s != nullptr && current->s->e != nullptr && current->s->e->e != nullptr){
            current = current->s->e->e;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == N){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check sww
        current = &square_map[loc_key];
        while(!found && current->s != nullptr && current->s->w != nullptr && current->s->w->w != nullptr){
            current = current->s->w->w;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == N){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check sse
        current = &square_map[loc_key];
        while(!found && current->s != nullptr && current->s->s != nullptr && current->s->s->e != nullptr){
            current = current->s->s->e;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == N){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't hav ea piece blocking the square.
            }
        }
        // Check ssw
        current = &square_map[loc_key];
        while(!found && current->s != nullptr && current->s->s != nullptr && current->s->s->w != nullptr){
            current = current->s->s->w;
            if(current->piece != nullptr){
                if(current->piece->owner == move_color && current->piece->type == N){
                    if((disambiguated_file == current->name[0] || disambiguated_file == '\0') && (disambiguated_rank == current->name[1] || disambiguated_rank == '\0')){
                        parsedMove.piece = current->piece;
                        found = true;
                    }
                }
                break;  // Either way, can't have a piece blocking the square.
            }
        }
        if(!found){
            throw "Move illformed";
        }
        parsedMove.ks_castle = false;
        parsedMove.qs_castle = false;
        parsedMove.to = &square_map[loc_key];
        return parsedMove;
    }

    throw "Move illformed";
}

void Board::setMoveColor(colors_t col){
    move_color = col;
}

void Board::removePiece(Piece* pie){
    // Remove from piece map list
    std::unordered_map<piece_t, std::list<Piece*>, std::hash<int> >* piece_map = pie->owner == white ? &white_piece_map : &black_piece_map;
    (*piece_map)[pie->type].remove(pie);
    // Remove from the square
    pie->square->piece = nullptr;
    // Free the memory
    delete pie;
}

void Board::addPiece(Piece* pie){
    // Add to piece map
    if(pie->owner == white){
        white_piece_map[pie->type].push_front(pie);
    }
    else{
        black_piece_map[pie->type].push_front(pie);
    }
    // Add to the square
    pie->square->piece = pie;
}

// Determines if the king of a certain color is being attacked
bool Board::inCheck(colors_t king_col){

    Piece* king;
    if(king_col == black){
        king = black_piece_map[K].front();
    }
    else{
        king = white_piece_map[K].front();
    }

    // Check for attacking pawns
    Square* current = king->square;
    if(king_col == white){
        if(current->n != nullptr){
            if(current->n->w != nullptr && current->n->w->piece != nullptr && current->n->w->piece->type==p && current->n->w->piece->owner == black){
                return true;
            }
        }
    }
    else{
        if(current->s != nullptr){
            if(current->s->w != nullptr && current->s->w->piece != nullptr && current->s->w->piece->type==p && current->s->w->piece->owner == white){
                return true;
            }
        }
    }

    // Check nsew for rooks or queens
    // Check n
    while(current->n != nullptr){
        current = current->n;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && (current->piece->type == R || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }
    current = king->square;
    while(current->s != nullptr){
        current = current->s;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && (current->piece->type == R || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }
    current = king->square;
    while(current->e != nullptr){
        current = current->e;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && (current->piece->type == R || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }
    current = king->square;
    while(current->w != nullptr){
        current = current->w;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && (current->piece->type == R || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }

    // Check the diagonals for bishops or queens
    current = king->square;
    while(current->n != nullptr && current->n->e != nullptr){
        current = current->n->e;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && (current->piece->type == B || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }
    current = king->square;
    while(current->n != nullptr && current->n->w != nullptr){
        current = current->n->w;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && (current->piece->type == B || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }
    current = king->square;
    while(current->s != nullptr && current->s->e != nullptr){
        current = current->s->e;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && (current->piece->type == B || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }
    current = king->square;
    while(current->s != nullptr && current->s->w != nullptr){
        current = current->s->w;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && (current->piece->type == B || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }

    // Check the knight squares
    current = king->square;
    if(current->n != nullptr && current->n->n != nullptr && current->n->n->e != nullptr){
        current = current->n->n->e;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = king->square;
    if(current->n != nullptr && current->n->n != nullptr && current->n->n->w != nullptr){
        current = current->n->n->w;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = king->square;
    if(current->n != nullptr && current->n->w != nullptr && current->n->w->w != nullptr){
        current = current->n->w->w;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = king->square;
    if(current->n != nullptr && current->n->e != nullptr && current->n->e->e != nullptr){
        current = current->n->e->e;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = king->square;
    if(current->s != nullptr && current->s->e != nullptr && current->s->e->e != nullptr){
        current = current->s->e->e;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = king->square;
    if(current->s != nullptr && current->s->w != nullptr && current->s->w->w != nullptr){
        current = current->s->w->w;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = king->square;
    if(current->s != nullptr && current->s->s != nullptr && current->s->s->e != nullptr){
        current = current->s->s->e;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = king->square;
    if(current->s != nullptr && current->s->s != nullptr && current->s->s->w != nullptr){
        current = current->s->s->w;
        if(current->piece != nullptr){
            if(current->piece->owner != king_col && current->piece->type == N){
                return true;
            }
        }
    }
    return false;
}

std::unordered_map<std::string, Square, CustomHash>* Board::getSquareMap(){
    return &square_map;
}

std::unordered_map< piece_t, std::list<Piece*>, std::hash<int> >* Board::getBlackPieceMap(){
    return &black_piece_map;
}

std::unordered_map< piece_t, std::list<Piece*>, std::hash<int> >* Board::getWhitePieceMap(){
    return &white_piece_map;
}

