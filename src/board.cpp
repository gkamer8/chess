#include "board.h"
#include "chess.h"
#include <string>
#include <iostream>
#include <unordered_map>

// for tests
std::string piece_to_string(piece_t pie){
    switch(pie){
        case p:
            return "pawn";
        case Q:
            return "Queen";
        case K:
            return "King";
        case N:
            return "Knight";
        case R:
            return "Rook";
        case B:
            return "Bishop";
    }
}

Board::Board(){
    move_color = white;
    move_num = 1;
    check = false;
    white_ks_castle_eligible = true;
    black_ks_castle_eligible = true;
    white_qs_castle_eligible = true;
    black_qs_castle_eligible = true;

    // Initialize board
    for(int rank = 0; rank<8; rank++){
        for(char file = 'a'; file<'i'; file++){
            std::string key = "";
            key += file;
            key += std::to_string(rank + 1);

            Square new_square;
            new_square.file = file;
            new_square.rank = rank;
            new_square.name = key;
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

    runTests();
}

void Board::makeMove(){

}

bool Board::isLegal(std::string move){
    return true;  // placeholder
}

void Board::pieceListTest(){
    Piece current_piece = *(head_piece.next);
    bool gone_forward = false;
    while(true){
        std::cout << piece_to_string(current_piece.type) << ", " << current_piece.square->name << ";" << std::endl;
        if(gone_forward){
            // Note: the first piece has head piece as prev
            if(current_piece.prev->prev == nullptr){
                break;
            }
            else{
                current_piece = *current_piece.prev;
            }
        }
        else{
            if(current_piece.next == nullptr){
                gone_forward = true;
            }
            else{
                current_piece = *current_piece.next;
            }
        }
    }
}

void Board::squareLocTest(){
    std::cout << "coord: n, s, e, w, ne, nw, se, sw;" << std::endl;
    for(int rank = 0; rank<8; rank++){
        for(char file = 'a'; file<'i'; file++){
            // check
            std::string key = "";
            key += file;
            key += std::to_string(rank + 1);

            std::string nstr = square_map[key].n == nullptr ? "" : square_map[key].n->name;
            std::string sstr = square_map[key].s == nullptr ? "" : square_map[key].s->name;
            std::string estr = square_map[key].e == nullptr ? "" : square_map[key].e->name;
            std::string wstr = square_map[key].w == nullptr ? "" : square_map[key].w->name;
            std::string nestr = square_map[key].ne == nullptr ? "" : square_map[key].ne->name;
            std::string nwstr = square_map[key].nw == nullptr ? "" : square_map[key].nw->name;
            std::string sestr = square_map[key].se == nullptr ? "" : square_map[key].se->name;
            std::string swstr = square_map[key].sw == nullptr ? "" : square_map[key].sw->name;

            std::cout << key << ": " << nstr << ", " << sstr << ", " << estr << ", " << wstr << ", " << nestr << ", " << nwstr << ", " << sestr << ", " << swstr << ";" << std::endl;
        }
    }
}

void Board::runTests(){
    // squareLocTest();
    pieceListTest();
}

