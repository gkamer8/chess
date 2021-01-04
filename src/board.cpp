#include "board.h"
#include "chess.h"
#include <string>
#include <iostream>
#include <unordered_map>

enum piece_t {p, Q, K, N, B, R};

struct Piece;  // Because of cross dependency

struct Square{
    char name[2];  // ex. "b4"
    char file;  // ex. 'b'
    int rank;  // ex. 4

    Square* n;  // north
    Square* s;  // south
    Square* e;  // east
    Square* w;  // west
    Square* nw;  // northwest
    Square* ne;  // northeast
    Square* se;  // southeast
    Square* sw;  // southwest

    // For knight moves
    Square* nne;  // north-north-east
    Square* nnw;  // north-north-west
    Square* nee;  // north-east-east
    Square* nww;  // north-west-west
    Square* sww;  // south-west-west
    Square* see;  // south-east-east
    Square* sse;  // south-south-east
    Square* ssw;  // south-south-west

    Piece* piece;  // Piece on the square
};

struct Piece{
    Square* square;  // location of the piece
    piece_t type;  // piece type (p, Q, R, etc.)
    colors_t owner;  // white or black
    Piece* next;  // For doubly linked list – not guaranteed to be any specific piece
    Piece* prev;  // SAA
};

struct Move{
    bool qs_castle;  // whether the move is a kingside castle
    bool ks_castle;  // whether the move is a queenside castle
    Piece piece;  // piece being moved
    bool check;  // whether the move results in check
    bool checkmate;  // whether the move results in checkmate
};

// For optimization - keys are simply algebraic coordinates; no need for complication
struct CustomHash {
    size_t operator()(const char coord[2]) const {
        return ((coord[0] - 'a') * 8) + (coord[1] - '1');
    }
};

Board::Board(){
    move_color = white;
    move_num = 1;
    check = false;
    white_ks_castle_eligible = true;
    black_ks_castle_eligible = true;
    white_qs_castle_eligible = true;
    black_qs_castle_eligible = true;

    // The board's main datatype (this is "the board" in a conventional sense)
    std::unordered_map<char[2], Square, CustomHash> square_map;
    
    // Doubly linked list for keeping track of pieces
    // Pieces should be removed as they are captured
    Piece head_piece;

    // Initialize board
    for(int rank; rank<8; rank++){
        for(char file = 'a'; file<'i'; file++){
            char key[2] = {file, (char)rank};

            Square new_square;
            new_square.file = file;
            new_square.rank = rank;
            std::strcpy(new_square.name, key);
            if(rank == 6 || rank == 1){
                Piece pie;
                pie.square = &new_square;
                pie.type = p;
                pie.owner = rank == 6 ? black : white;
                new_square.piece = &pie;
            }
            else if(rank == 0 || rank == 7){
                Piece pie;
                pie.square = &new_square;
                switch(file){
                    case 'a':
                    case 'h':
                        pie.type = R;
                        break;
                    case 'b':
                    case 'g':
                        pie.type = N;
                        break;
                    case 'c':
                    case 'f':
                        pie.type = B;
                        break;
                    case 'd':
                        pie.type = Q;
                        break;
                    case 'e':
                        pie.type = K;
                }
                pie.owner = rank == 7 ? black : white;
                new_square.piece = &pie;
            }
            square_map.at(key) = new_square;
        }
    }

    // Now add attributes that point to other squares

    // Now put together the piece list

}

void Board::makeMove(){

}

bool Board::isLegal(std::string move){
    return true;  // placeholder
}

