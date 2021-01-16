#include "board.h"
#include "chess.h"
#include <string>
#include <iostream>
#include <unordered_map>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wc++11-extensions"

// For display()
std::string pieceToShortString(piece_t t){
    switch(t){
        case K:
            return "K";
        case N:
            return "N";
        case B:
            return "B";
        case R:
            return "R";
        case Q:
            return "Q";
        default:
            return "P";
    }
}

std::string pieceToPrefix(piece_t t){
    switch(t){
        case K:
            return "K";
        case N:
            return "N";
        case B:
            return "B";
        case R:
            return "R";
        case Q:
            return "Q";
        default:
            return "";
    }
}

Board::Board(){
    move_color = white;
    move_num = 1;
    white_ks_castle_eligible = true;
    black_ks_castle_eligible = true;
    white_qs_castle_eligible = true;
    black_qs_castle_eligible = true;
    enPassantLoc = "";

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

Board::~Board(){
    // Before: 159.3MB after move 200
    // After: 564KB after move 200
    // Remove pieces (squares not created with new)
    for(char f = 'a'; f <= 'h'; f++){
        for(char r = '1'; r <= '8'; r++){
            std::string loc = "";
            loc += f;
            loc += r;
            if(square_map[loc].piece != nullptr){
                removePiece(square_map[loc].piece);
            }
        }
    }
}

// Copy constructor - deep copy
Board::Board(Board& brd){
    move_color = brd.move_color;
    move_num = brd.move_num;
    white_ks_castle_eligible = brd.white_ks_castle_eligible;
    black_ks_castle_eligible = brd.black_ks_castle_eligible;
    white_qs_castle_eligible = brd.white_qs_castle_eligible;
    black_qs_castle_eligible = brd.black_qs_castle_eligible;
    enPassantLoc = brd.enPassantLoc;
    for (std::pair<std::string, Square> element : brd.square_map){
        Square new_square;
        square_map[element.first] = new_square;
    }
    for (std::pair<std::string, Square> element : brd.square_map){
        square_map[element.first].clr = element.second.clr;
        square_map[element.first].file = element.second.file;
        square_map[element.first].rank = element.second.rank;
        square_map[element.first].name = element.second.name;


        square_map[element.first].n = element.second.n == nullptr ? nullptr : &square_map[element.second.n->name];
        square_map[element.first].s = element.second.s == nullptr ? nullptr : &square_map[element.second.s->name];
        square_map[element.first].e = element.second.e == nullptr ? nullptr : &square_map[element.second.e->name];
        square_map[element.first].w = element.second.w == nullptr ? nullptr : &square_map[element.second.w->name];
        square_map[element.first].ne = element.second.ne == nullptr ? nullptr : &square_map[element.second.ne->name];
        square_map[element.first].nw = element.second.nw == nullptr ? nullptr : &square_map[element.second.nw->name];
        square_map[element.first].se = element.second.se == nullptr ? nullptr : &square_map[element.second.se->name];
        square_map[element.first].sw = element.second.sw == nullptr ? nullptr : &square_map[element.second.sw->name];

        square_map[element.first].piece = nullptr;
        if(element.second.piece != nullptr){
            Piece* new_piece = new Piece();
            new_piece->owner = element.second.piece->owner;
            new_piece->square = &square_map[element.first];
            new_piece->type = element.second.piece->type;
            square_map[element.first].piece = new_piece;
            if(new_piece->owner == white){
                white_piece_map[new_piece->type].push_front(new_piece);
            }
            else{
                black_piece_map[new_piece->type].push_front(new_piece);
            }
        }
    }
    moves = brd.moves;  // Should automatically clone
}

// Note: might throw an error if asked to castle and it can't
void Board::executeMove(Move& move){

    // Any pawn moving up two squares triggers the en passant flag
    // Otherwise, see if it's an en passant capture itself
            // In which case, remove the captured pawn and turn off the en passant flag
    // In other cases, turn off the en passant flag
    if(move.piece->type == p && ((move_color == white && (move.to->rank == 2 + move.piece->square->rank)) || (move_color == black && (move.to->rank == move.piece->square->rank - 2)))){
        enPassantLoc = "";
        enPassantLoc += move.to->file;
        enPassantLoc += std::to_string(move.to->rank + 1);
    }
    else if(move.enPassant != ""){
        removePiece(square_map[move.enPassant].piece);
        enPassantLoc = "";
    }
    else{
        enPassantLoc = "";
    }
    // Address castling
    if(move.ks_castle){
        if(move_color == white){
            square_map["f1"].piece = square_map["h1"].piece;
            square_map["h1"].piece = nullptr;
            square_map["g1"].piece = square_map["e1"].piece;
            square_map["e1"].piece = nullptr;

            square_map["g1"].piece->square = &square_map["g1"];
            square_map["f1"].piece->square = &square_map["f1"];
        }
        else{
            square_map["f8"].piece = square_map["h8"].piece;
            square_map["h8"].piece = nullptr;
            square_map["g8"].piece = square_map["e8"].piece;
            square_map["e8"].piece = nullptr;

            square_map["g8"].piece->square = &square_map["g8"];
            square_map["f8"].piece->square = &square_map["f8"];
        }
        return;
    }
    else if(move.qs_castle){
        if(move_color == white){
            square_map["d1"].piece = square_map["a1"].piece;
            square_map["a1"].piece = nullptr;
            square_map["c1"].piece = square_map["e1"].piece;
            square_map["e1"].piece = nullptr;

            square_map["c1"].piece->square = &square_map["c1"];
            square_map["d1"].piece->square = &square_map["d1"];
        }
        else{
            square_map["d8"].piece = square_map["a8"].piece;
            square_map["a8"].piece = nullptr;
            square_map["c8"].piece = square_map["e8"].piece;
            square_map["e8"].piece = nullptr;

            square_map["c8"].piece->square = &square_map["c8"];
            square_map["d8"].piece->square = &square_map["d8"];
        }
        return;
    }
    else{
        // If it's a capture, remove the captured piece
        if(move.to->piece != nullptr){
            removePiece(move.to->piece);
        }
        move.piece->square->piece = nullptr;
        move.piece->square = move.to;
        move.to->piece = move.piece;
    }

    if(move.promotedTo != p){
        move.piece->type = move.promotedTo;
        // Remove from pawn list
        std::unordered_map<piece_t, std::list<Piece*>, std::hash<int> >* piece_map = move.piece->owner == white ? &white_piece_map : &black_piece_map;
        (*piece_map)[p].remove(move.piece);
        // Add piece to correct piece map list
        (*piece_map)[move.piece->type].push_front(move.piece);
    }
}

void Board::makeMove(std::string move_str){
    Move move = parseMove(move_str);  // This could throw a "Move illformed"

    // If illegal, end
    if(!isLegal(move)){
        throw "Illegal move";
    }

    // Set castle eligibility appropriately
        // If it was a king move, all castling is inelgible
        // If it was a rook move or a rook was captured, castling on that side is ineligible
    if(move_color == white){
        if(move.piece->type == K){
            white_ks_castle_eligible = false;
            white_qs_castle_eligible = false;
        }
        else if(white_ks_castle_eligible){
            if(move.piece->type == R && move.piece->square->name == "h1"){
                white_ks_castle_eligible = false;
            }
        }
        else if(white_qs_castle_eligible){
            if(move.piece->type == R && move.piece->square->name == "a1"){
                white_qs_castle_eligible = false;
            }
        }
        else if(black_ks_castle_eligible){
            if(move.to->name == "h8"){
                black_ks_castle_eligible = false;
            }
        }
        else if(black_qs_castle_eligible){
            if(move.to->name == "a8"){
                black_qs_castle_eligible = false;
            }
        }
    }
    else{
        if(move.piece->type == K){
            black_ks_castle_eligible = false;
            black_qs_castle_eligible = false;
        }
        else if(black_ks_castle_eligible){
            if(move.piece->type == R && move.piece->square->name == "h8"){
                black_ks_castle_eligible = false;
            }
        }
        else if(black_qs_castle_eligible){
            if(move.piece->type == R && move.piece->square->name == "a8"){
                black_qs_castle_eligible = false;
            }
        }
        else if(white_ks_castle_eligible){
            if(move.to->name == "h1"){
                white_ks_castle_eligible = false;
            }
        }
        else if(white_qs_castle_eligible){
            if(move.to->name == "a1"){
                white_qs_castle_eligible = false;
            }
        }
    }
    // Execute the move
    executeMove(move);
    // Set move color
    if(move_color == white){
        move_color = black;
    }
    else{
        move_color = white;
        move_num++;
    }
    // Add to moves vector
    moves.push_back(move_str);

    // Is the board in checkmate?
    // TODO
}

// Checks whether castling is legal and possible, makes sure the move doesn't put the mover in check
bool Board::isLegal(Move& move){
    // First take care of the castling situation
    if(move.ks_castle){
        // Can't castle out of check
        if(inCheck(move_color)){
            return false;
        }
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
            Move kingMove = checkCheck.parseMove("Kf1");
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
            Move kingMove = checkCheck.parseMove("Kf8");
            checkCheck.executeMove(kingMove);
            if(checkCheck.inCheck(move_color)){
                return false;
            }
        }
    }
    else if(move.qs_castle){
        // Can't castle out of check
        if(inCheck(move_color)){
            return false;
        }
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
            Move kingMove = checkCheck.parseMove("Kd1");
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
            Move kingMove = checkCheck.parseMove("Kd8");
            checkCheck.executeMove(kingMove);
            if(checkCheck.inCheck(move_color)){
                return false;
            }
        }
    }
    // Execute the move on a fake board – can't put the mover in check
    Board checkCheck(*this);
    Move checkMove;
    checkMove.ks_castle = move.ks_castle;
    checkMove.qs_castle = move.qs_castle;
    checkMove.piece = (*checkCheck.getSquareMap())[move.piece->square->name].piece;
    checkMove.promotedTo = move.promotedTo;
    checkMove.to = &(*checkCheck.getSquareMap())[move.to->name];
    checkMove.enPassant = "";
    checkCheck.executeMove(checkMove);
    if(checkCheck.inCheck(move_color)){
        return false;
    }

    return true;
}

struct Move Board::parseMove(std::string move){
    Move parsedMove;
    parsedMove.promotedTo = p;  // Like setting it to null – can never be promoted to pawn
    parsedMove.enPassant = "";

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
                // Promotion square has to be empty
                if(square_map[move.substr(0, 2)].piece != nullptr){
                    throw "Move illformed";
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
                // Pushed square has to be empty
                if(square_map[move].piece != nullptr){
                    throw "Move illformed";
                }
                // Find the pawn being moved
                Square* current_square = move_color == white ? square_map[move].s : square_map[move].n;
                if(current_square->piece != nullptr && current_square->piece->type == p && current_square->piece->owner == move_color){
                    parsedMove.piece = current_square->piece;
                }
                else if(current_square->piece == nullptr){
                    if(move_color == white && current_square->rank == 3-1){
                        if(current_square->s->piece != nullptr && current_square->s->piece->type == p && current_square->s->piece->owner == white){
                            parsedMove.piece = current_square->s->piece;
                        }
                        else{
                            throw "Move illformed";
                        }
                    }
                    else if(move_color == black && current_square->rank == 6-1){
                        if(current_square->n->piece != nullptr && current_square->n->piece->type == p && current_square->n->piece->owner == black){
                            parsedMove.piece = current_square->n->piece;
                        }
                        else{
                            throw "Move illformed";
                        }
                    }
                    else{
                        throw "Move illformed";
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
            // unless it's a promotion on capture...
            if(move.length() != 4){
                if(move[move.size() - 1] != 'Q' && move[move.size() - 1] != 'N' && move[move.size() - 1] != 'B' && move[move.size() - 1] != 'R'){
                    throw "Move illformed";
                }
                else{
                    switch(move[move.size() - 1]){
                        case 'Q':
                            parsedMove.promotedTo = Q;
                            break;
                        case 'B':
                            parsedMove.promotedTo = B;
                            break;
                        case 'R':
                            parsedMove.promotedTo = R;
                            break;
                        case 'N':
                            parsedMove.promotedTo = N;
                            break;
                    }
                }
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
            if(square_map[loc_key].piece == nullptr || square_map[loc_key].piece->type != p || square_map[loc_key].piece->owner != move_color){
                throw "Move illformed";
            }
            else{
                parsedMove.piece = square_map[loc_key].piece;
            }
            // Make sure that it's actually a capture
            std::string cap_key = "";
            cap_key += move[2];
            cap_key += move[3];
            if(square_map[cap_key].piece == nullptr){
                // Must be en passant
                std::string emp = "";
                emp += move[2];
                emp += move_color == white ? move[3] - 1 : move[3] + 1;
                if(enPassantLoc != emp){
                    throw "Move illformed";
                }
                parsedMove.enPassant = emp;
            }
            else if(square_map[cap_key].piece->owner == move_color){
                throw "Move illformed";
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
        else if(square_map[key].ne != nullptr && square_map[key].ne->piece != nullptr && square_map[key].ne->piece->type == K && square_map[key].ne->piece->owner == move_color){
            parsedMove.piece = square_map[key].ne->piece;
        }
        else if(square_map[key].nw != nullptr && square_map[key].nw->piece != nullptr && square_map[key].nw->piece->type == K && square_map[key].nw->piece->owner == move_color){
            parsedMove.piece = square_map[key].nw->piece;
        }
        else if(square_map[key].se != nullptr && square_map[key].se->piece != nullptr && square_map[key].se->piece->type == K && square_map[key].se->piece->owner == move_color){
            parsedMove.piece = square_map[key].se->piece;
        }
        else if(square_map[key].sw != nullptr && square_map[key].sw->piece != nullptr && square_map[key].sw->piece->type == K && square_map[key].sw->piece->owner == move_color){
            parsedMove.piece = square_map[key].sw->piece;
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
        while(!found && current->ne != nullptr){
            current = current->ne;
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
        while(!found && current->nw != nullptr){
            current = current->nw;
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
        while(!found && current->se != nullptr){
            current = current->se;
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
        while(!found && current->sw != nullptr){
            current = current->sw;
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
    colors_t attacking = white;
    if(king_col == black){
        king = black_piece_map[K].front();
    }
    else{
        king = white_piece_map[K].front();
        attacking = black;
    }
    return isAttacked(king->square, attacking);
}

// Determines whether a particular square is being attacked by a certain color
bool Board::isAttacked(Square* sq, colors_t attacking_col){
    // Check for attacking pawns
    Square* current = sq;
    if(attacking_col == black){
        if(current->nw != nullptr && current->nw->piece != nullptr && current->nw->piece->type==p && current->nw->piece->owner == black){
            return true;
        }
        if(current->ne != nullptr && current->ne->piece != nullptr && current->ne->piece->type==p && current->ne->piece->owner == black){
            return true;
        }
    }
    else{
        if(current->sw != nullptr && current->sw->piece != nullptr && current->sw->piece->type==p && current->sw->piece->owner == white){
            return true;
        }
        if(current->se != nullptr && current->se->piece != nullptr && current->se->piece->type==p && current->se->piece->owner == white){
            return true;
        }
    }

    // Check nsew for rooks or queens
    // Check n
    while(current->n != nullptr){
        current = current->n;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && (current->piece->type == R || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }
    current = sq;
    while(current->s != nullptr){
        current = current->s;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && (current->piece->type == R || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }
    current = sq;
    while(current->e != nullptr){
        current = current->e;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && (current->piece->type == R || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }
    current = sq;
    while(current->w != nullptr){
        current = current->w;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && (current->piece->type == R || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }

    // Check the diagonals for bishops or queens
    current = sq;
    while(current->n != nullptr && current->n->e != nullptr){
        current = current->n->e;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && (current->piece->type == B || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }
    current = sq;
    while(current->n != nullptr && current->n->w != nullptr){
        current = current->n->w;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && (current->piece->type == B || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }
    current = sq;
    while(current->s != nullptr && current->s->e != nullptr){
        current = current->s->e;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && (current->piece->type == B || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }
    current = sq;
    while(current->s != nullptr && current->s->w != nullptr){
        current = current->s->w;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && (current->piece->type == B || current->piece->type == Q)){
                return true;
            }
            break;
        }
    }

    // Check the knight squares
    current = sq;
    if(current->n != nullptr && current->n->n != nullptr && current->n->n->e != nullptr){
        current = current->n->n->e;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = sq;
    if(current->n != nullptr && current->n->n != nullptr && current->n->n->w != nullptr){
        current = current->n->n->w;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = sq;
    if(current->n != nullptr && current->n->w != nullptr && current->n->w->w != nullptr){
        current = current->n->w->w;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = sq;
    if(current->n != nullptr && current->n->e != nullptr && current->n->e->e != nullptr){
        current = current->n->e->e;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = sq;
    if(current->s != nullptr && current->s->e != nullptr && current->s->e->e != nullptr){
        current = current->s->e->e;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = sq;
    if(current->s != nullptr && current->s->w != nullptr && current->s->w->w != nullptr){
        current = current->s->w->w;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = sq;
    if(current->s != nullptr && current->s->s != nullptr && current->s->s->e != nullptr){
        current = current->s->s->e;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && current->piece->type == N){
                return true;
            }
        }
    }
    current = sq;
    if(current->s != nullptr && current->s->s != nullptr && current->s->s->w != nullptr){
        current = current->s->s->w;
        if(current->piece != nullptr){
            if(current->piece->owner == attacking_col && current->piece->type == N){
                return true;
            }
        }
    }

    // Check for the opponent's king
    Piece* oppo_king = attacking_col == white ? white_piece_map[K].front() : black_piece_map[K].front();
    if(sq->n != nullptr && oppo_king->square == sq->n){
        return true;
    }
    if(sq->s != nullptr && oppo_king->square == sq->s){
        return true;
    }
    if(sq->e != nullptr && oppo_king->square == sq->e){
        return true;
    }
    if(sq->w != nullptr && oppo_king->square == sq->w){
        return true;
    }
    if(sq->ne != nullptr && oppo_king->square == sq->ne){
        return true;
    }
    if(sq->nw != nullptr && oppo_king->square == sq->nw){
        return true;
    }
    if(sq->se != nullptr && oppo_king->square == sq->se){
        return true;
    }
    if(sq->sw != nullptr && oppo_king->square == sq->sw){
        return true;
    }

    return false;
}

// TODO: Determines if the player with the current move is in checkmate
bool Board::inCheckmate(){
    // TODO
    return false;
}

void Board::display(){
    const std::string red("\x1b[31m");
    const std::string green("\x1b[32m");
    const std::string cyan("\x1b[100m");
    const std::string magenta("\x1b[47m");
    const std::string reset("\x1b[0m");

    std::string sq_color = cyan;
    for(int i = 8; i >= 1; i--){
        for(char c = 'a'; c <= 'h'; c++){
            sq_color = sq_color == cyan ? magenta : cyan;
            std::string myStr = " ";
            std::string key = "";
            std::string col = "";
            key += c;
            key += std::to_string(i);
            if(square_map[key].piece == nullptr){
                myStr += " ";
            }
            else{
                myStr += pieceToShortString(square_map[key].piece->type);
                col = square_map[key].piece->owner == white ? green : red;
            }   
            myStr += " ";
            std::cout << sq_color << col << myStr << reset;
        }
        std::cout << std::endl;
        sq_color = sq_color == cyan ? magenta : cyan;
    }
}

colors_t Board::getMoveColor(){
    return move_color;
}

int Board::getMoveNum(){
    return move_num;
}

std::string Board::getEnPassantLoc(){
    return enPassantLoc;
}

std::string Board::getName(Move& move){
    if(move.ks_castle){
        return "0-0";
    }
    else if(move.qs_castle){
        return "0-0-0";
    }
    std::string n = "";
    n += pieceToPrefix(move.piece->type);
    // ADD DISAMBIGUATION
    bool ambig_needed = false;
    bool same_file = false;
    bool same_rank = false;
    // Rooks / queens
    if(move.piece->type == R || move.piece->type == Q){
        // Go north
        Square* current = move.to;
        while(current->n != nullptr){
            current = current->n;
            if(current->piece != nullptr){
                if(current->piece != move.piece && current->piece->owner == move.piece->owner && current->piece->type == move.piece->type){
                    ambig_needed = true;
                    if(current->rank == move.piece->square->rank){
                        same_rank = true;
                    }
                    if(current->file == move.piece->square->file){
                        same_file = true;
                    }
                }
                else{
                    break;
                }
            }
        }
        // Go south
        current = move.to;
        while(current->s != nullptr){
            current = current->s;
            if(current->piece != nullptr){
                if(current->piece != move.piece && current->piece->owner == move.piece->owner && current->piece->type == move.piece->type){
                    ambig_needed = true;
                    if(current->rank == move.piece->square->rank){
                        same_rank = true;
                    }
                    if(current->file == move.piece->square->file){
                        same_file = true;
                    }
                }
                else{
                    break;
                }
            }
        }
        // Go west
        current = move.to;
        while(current->w != nullptr){
            current = current->w;
            if(current->piece != nullptr){
                if(current->piece != move.piece && current->piece->owner == move.piece->owner && current->piece->type == move.piece->type){
                    ambig_needed = true;
                    if(current->rank == move.piece->square->rank){
                        same_rank = true;
                    }
                    if(current->file == move.piece->square->file){
                        same_file = true;
                    }
                }
                else{
                    break;
                }
            }
        }
        // Go east
        current = move.to;
        while(current->e != nullptr){
            current = current->e;
            if(current->piece != nullptr){
                if(current->piece != move.piece && current->piece->owner == move.piece->owner && current->piece->type == move.piece->type){
                    ambig_needed = true;
                    if(current->rank == move.piece->square->rank){
                        same_rank = true;
                    }
                    if(current->file == move.piece->square->file){
                        same_file = true;
                    }
                }
                else{
                    break;
                }
            }
        }
    }
    if(move.piece->type == B || move.piece->type == Q){
        Square* current = move.to;
        while(current->ne != nullptr){
            current = current->ne;
            if(current->piece != nullptr){
                if(current->piece != move.piece && current->piece->owner == move.piece->owner && current->piece->type == move.piece->type){
                    ambig_needed = true;
                    if(current->rank == move.piece->square->rank){
                        same_rank = true;
                    }
                    if(current->file == move.piece->square->file){
                        same_file = true;
                    }
                }
                else{
                    break;
                }
            }
        }
        current = move.to;
        while(current->nw != nullptr){
            current = current->nw;
            if(current->piece != nullptr){
                if(current->piece != move.piece && current->piece->owner == move.piece->owner && current->piece->type == move.piece->type){
                    ambig_needed = true;
                    if(current->rank == move.piece->square->rank){
                        same_rank = true;
                    }
                    if(current->file == move.piece->square->file){
                        same_file = true;
                    }
                }
                else{
                    break;
                }
            }
        }
        current = move.to;
        while(current->se != nullptr){
            current = current->se;
            if(current->piece != nullptr){
                if(current->piece != move.piece && current->piece->owner == move.piece->owner && current->piece->type == move.piece->type){
                    ambig_needed = true;
                    if(current->rank == move.piece->square->rank){
                        same_rank = true;
                    }
                    if(current->file == move.piece->square->file){
                        same_file = true;
                    }
                }
                else{
                    break;
                }
            }
        }
        current = move.to;
        while(current->sw != nullptr){
            current = current->sw;
            if(current->piece != nullptr){
                if(current->piece != move.piece && current->piece->owner == move.piece->owner && current->piece->type == move.piece->type){
                    ambig_needed = true;
                    if(current->rank == move.piece->square->rank){
                        same_rank = true;
                    }
                    if(current->file == move.piece->square->file){
                        same_file = true;
                    }
                }
                else{
                    break;
                }
            }
        }
    }
    // Knights
    if(move.piece->type == N){
        Square* current = move.to;
        std::vector<Square*> squares;
        if(current->ne != nullptr){
            squares.push_back(current->ne->n);
            squares.push_back(current->ne->e);
        }
        if(current->nw != nullptr){
            squares.push_back(current->nw->n);
            squares.push_back(current->nw->w);
        }
        if(current->se != nullptr){
            squares.push_back(current->se->e);
            squares.push_back(current->se->s);
        }
        if(current->sw != nullptr){
            squares.push_back(current->sw->w);
            squares.push_back(current->sw->s);
        }
        
        for(const auto current : squares){
            if(current == nullptr){
                continue;
            }
            if(current->piece != nullptr){
                if(current->piece != move.piece && current->piece->owner == move.piece->owner && current->piece->type == move.piece->type){
                    ambig_needed = true;
                    if(current->rank == move.piece->square->rank){
                        same_rank = true;
                    }
                    if(current->file == move.piece->square->file){
                        same_file = true;
                    }
                }
            }
        }
    }

    if(ambig_needed){
        if(!same_file){
            n += move.piece->square->file;
        }
        else if(!same_rank){
            n += std::to_string(move.piece->square->rank + 1);
        }
        else{
            n += move.piece->square->name;
        }
    }

    if(square_map[move.to->name].piece != nullptr && square_map[move.to->name].piece->owner != move.piece->owner){
        if(move.piece->type == p){
            n += move.piece->square->file;
        }
        n += "x";
    }
    n += move.to->name;
    n += pieceToPrefix(move.promotedTo);
    return n;
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

