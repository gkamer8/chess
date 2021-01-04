#include "../src/board.h"
#include "board_tests.h"
#include <iostream>
#include <unistd.h>

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

void BoardTests::pieceListTest(Board& brd){
    std::cout << std::endl;

    Piece head_piece = *brd.getHeadPiece();
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

    std::cout << std::endl;
}

void BoardTests::squareLocTest(Board& brd){
    std::cout << std::endl;

    std::unordered_map<std::string, Square, CustomHash> square_map = brd.getSquareMap();
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

    std::cout << std::endl;
}

void BoardTests::parseMoveTest(Board& brd){
    std::cout << std::endl;

    std::string move = "0-0";
    try{
        Move mv = brd.parseMove(move);
        std::cout << "Move: " << move << std::endl;
        std::cout << "From: " << mv.piece->square->name << std::endl;
        std::cout << "To: " << mv.to->name << std::endl;
        std::cout << "Piece: " << piece_to_string(mv.piece->type) << std::endl;
        std::cout << "Ks-c: " << mv.ks_castle << std::endl;
        std::cout << "Qs-c: " << mv.qs_castle << std::endl;
    }
    catch(const char* exp){
        std::cout << move << ": " << exp << std::endl;
    }
    std::cout << std::endl;

    move = "0-0-0";
    try{
        Move mv = brd.parseMove(move);
        std::cout << "Move: " << move << std::endl;
        std::cout << "From: " << mv.piece->square->name << std::endl;
        std::cout << "To: " << mv.to->name << std::endl;
        std::cout << "Piece: " << piece_to_string(mv.piece->type) << std::endl;
        std::cout << "Ks-c: " << mv.ks_castle << std::endl;
        std::cout << "Qs-c: " << mv.qs_castle << std::endl;
    }
    catch(const char* exp){
        std::cout << move << ": " << exp << std::endl;
    }
    std::cout << std::endl;

    move = "e4";
    try{
        Move mv = brd.parseMove(move);
        std::cout << "Move: " << move << std::endl;
        std::cout << "From: " << mv.piece->square->name << std::endl;
        std::cout << "To: " << mv.to->name << std::endl;
        std::cout << "Piece: " << piece_to_string(mv.piece->type) << std::endl;
        std::cout << "Ks-c: " << mv.ks_castle << std::endl;
        std::cout << "Qs-c: " << mv.qs_castle << std::endl;
    }
    catch(const char* exp){
        std::cout << move << ": " << exp << std::endl;
    }
    std::cout << std::endl;

    move = "e5";
    brd.setMoveColor(black);
    try{
        Move mv = brd.parseMove(move);
        std::cout << "Move: " << move << std::endl;
        std::cout << "From: " << mv.piece->square->name << std::endl;
        std::cout << "To: " << mv.to->name << std::endl;
        std::cout << "Piece: " << piece_to_string(mv.piece->type) << std::endl;
        std::cout << "Ks-c: " << mv.ks_castle << std::endl;
        std::cout << "Qs-c: " << mv.qs_castle << std::endl;
    }
    catch(const char* exp){
        std::cout << move << ": " << exp << std::endl;
    }
    brd.setMoveColor(white);
    std::cout << std::endl;

    move = "e1";
    try{
        Move mv = brd.parseMove(move);
        std::cout << "Move: " << move << std::endl;
        std::cout << "From: " << mv.piece->square->name << std::endl;
        std::cout << "To: " << mv.to->name << std::endl;
        std::cout << "Piece: " << piece_to_string(mv.piece->type) << std::endl;
        std::cout << "Ks-c: " << mv.ks_castle << std::endl;
        std::cout << "Qs-c: " << mv.qs_castle << std::endl;
    }
    catch(const char* exp){
        std::cout << move << ": " << exp << std::endl;
    }
    std::cout << std::endl;

    move = "e1";
    brd.setMoveColor(black);
    try{
        Move mv = brd.parseMove(move);
        std::cout << "Move: " << move << std::endl;
        std::cout << "From: " << mv.piece->square->name << std::endl;
        std::cout << "To: " << mv.to->name << std::endl;
        std::cout << "Piece: " << piece_to_string(mv.piece->type) << std::endl;
        std::cout << "Ks-c: " << mv.ks_castle << std::endl;
        std::cout << "Qs-c: " << mv.qs_castle << std::endl;
    }
    catch(const char* exp){
        std::cout << move << ": " << exp << std::endl;
    }
    brd.setMoveColor(white);
    std::cout << std::endl;

    std::cout << std::endl;
}

void BoardTests::pieceMapTest(Board& brd){
    std::cout << std::endl;

    std::unordered_map< piece_t, std::list<Piece*>, std::hash<int> > myMap = brd.getPieceMap();
    
    std::cout << "Rooks: " << std::endl;
    for(std::list<Piece*>::iterator it = myMap[R].begin(); it != myMap[R].end(); it++){
        std::cout << ((*it)->square->name) << std::endl;
    }

    std::cout << "Knights: " << std::endl;
    for(std::list<Piece*>::iterator it = myMap[N].begin(); it != myMap[N].end(); it++){
        std::cout << ((*it)->square->name) << std::endl;
    }

    std::cout << "Bishops: " << std::endl;
    for(std::list<Piece*>::iterator it = myMap[B].begin(); it != myMap[B].end(); it++){
        std::cout << ((*it)->square->name) << std::endl;
    }

    std::cout << "Kings: " << std::endl;
    for(std::list<Piece*>::iterator it = myMap[B].begin(); it != myMap[B].end(); it++){
        std::cout << ((*it)->square->name) << std::endl;
    }

    std::cout << "Queens: " << std::endl;
    for(std::list<Piece*>::iterator it = myMap[Q].begin(); it != myMap[Q].end(); it++){
        std::cout << ((*it)->square->name) << std::endl;
    }

    std::cout << "Pawns: " << std::endl;
    for(std::list<Piece*>::iterator it = myMap[p].begin(); it != myMap[p].end(); it++){
        std::cout << ((*it)->square->name) << std::endl;
    }

    std::cout << std::endl;
}

void BoardTests::runTests(){
    std::cout << "Running Board tests..." << std::endl;
    usleep(500000);

    Board myBoard;

    // squareLocTest(myBoard);
    // pieceListTest(myBoard);
    parseMoveTest(myBoard);
    // pieceMapTest(myBoard);

    std::cout << "Board tests completed." << std::endl;
}