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

void BoardTests::squareLocTest(Board& brd){
    std::cout << std::endl;

    std::unordered_map<std::string, Square, CustomHash> square_map = (*brd.getSquareMap());
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

    move = "e2";
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

    move = "e7";
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

    move = "c8Q";
    try{
        Move mv = brd.parseMove(move);
        std::cout << "Move: " << move << std::endl;
        std::cout << "From: " << mv.piece->square->name << std::endl;
        std::cout << "To: " << mv.to->name << std::endl;
        std::cout << "Piece: " << piece_to_string(mv.piece->type) << std::endl;
        std::cout << "Ks-c: " << mv.ks_castle << std::endl;
        std::cout << "Qs-c: " << mv.qs_castle << std::endl;
        std::cout << "Prom: " << piece_to_string(mv.promotedTo) << std::endl;
    }
    catch(const char* exp){
        std::cout << move << ": " << exp << std::endl;
    }
    std::cout << std::endl;

    move = "exd3";
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

    move = "Qe5";
    Piece* play_queen = new Piece();
    play_queen->owner = white;
    play_queen->type = Q;
    play_queen->square = &((*brd.getSquareMap())["a5"]);
    brd.addPiece(play_queen);
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

    move = "Qdd5";
    brd.removePiece((*brd.getSquareMap())["d2"].piece);
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
    Piece* add_back_pawn = new Piece();
    add_back_pawn->owner = white;
    add_back_pawn->type = p;
    add_back_pawn->square = &((*brd.getSquareMap())["d2"]);
    brd.addPiece(add_back_pawn);
    brd.removePiece(play_queen);
    std::cout << std::endl;

    move = "Qad4";
    play_queen = new Piece();
    play_queen->owner = white;
    play_queen->type = Q;
    play_queen->square = &((*brd.getSquareMap())["a4"]);
    brd.addPiece(play_queen);
    brd.removePiece((*brd.getSquareMap())["d2"].piece);
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
    add_back_pawn = new Piece();
    add_back_pawn->owner = white;
    add_back_pawn->type = p;
    add_back_pawn->square = &(*brd.getSquareMap())["d2"];
    brd.addPiece(add_back_pawn);
    brd.removePiece(play_queen);
    std::cout << std::endl;

    move = "Bc4";
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

    move = "Bc4";
    brd.removePiece((*brd.getSquareMap())["e2"].piece);
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
    add_back_pawn = new Piece();
    add_back_pawn->owner = white;
    add_back_pawn->type = p;
    add_back_pawn->square = &(*brd.getSquareMap())["e2"];
    brd.addPiece(add_back_pawn);
    std::cout << std::endl;

    move = "Ra5";
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

    move = "Ra5";
    brd.removePiece((*brd.getSquareMap())["a2"].piece);
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
    add_back_pawn = new Piece();
    add_back_pawn->owner = white;
    add_back_pawn->type = p;
    add_back_pawn->square = &(*brd.getSquareMap())["a2"];
    brd.addPiece(add_back_pawn);
    std::cout << std::endl;

    move = "Nc3";
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

    move = "Nc5";
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

    move = "Nd2";
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

    move = "e4Q";
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

    std::cout << std::endl;
}

void BoardTests::pieceMapTest(Board& brd){
    std::cout << std::endl;

    std::unordered_map< piece_t, std::list<Piece*>, std::hash<int> > myMap = *brd.getBlackPieceMap();

    std::cout << "Black: " << std::endl; 
    
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

    myMap = *brd.getWhitePieceMap();

    std::cout << "White: " << std::endl; 
    
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

void BoardTests::deletePieceTest(Board& brd){
    std::cout << std::endl;

    Piece* toDelete = (*brd.getSquareMap())["a1"].piece;
    brd.removePiece(toDelete);

    // Note: if the memory is freed but the pointers aren't erased, this will result in a seg fault!

    // Does it get removed from the square properly?
    if((*brd.getSquareMap())["a1"].piece == nullptr){
        std::cout << "Deleting from square map was a success." << std::endl;
    }
    else{
        piece_t dumb = (*brd.getSquareMap())["a1"].piece->type;
        std::cout << "Piece (" << piece_to_string(dumb) << ") not deleted properly from square map!" << std::endl;
    }

    // Does it get removed from the piece map?
    if((*brd.getWhitePieceMap())[R].size() == 1){
        std::cout << "Deleting from piece map was a success." << std::endl;
    }
    else if((*brd.getWhitePieceMap())[R].back() == nullptr || (*brd.getWhitePieceMap())[R].front() == nullptr){
        std::cout << "Deleting from piece map was a half success." << std::endl;
    }
    else{
        std::string dumb1 = (*brd.getWhitePieceMap())[R].back()->square->name;
        std::string dumb2 = (*brd.getWhitePieceMap())[R].front()->square->name;
        std::cout << "Piece not deleted properly from piece map!" << " Rooks at: " << dumb1 << ", " << dumb2 << std::endl;
    }

    std::cout << std::endl;
}

void BoardTests::inCheckTest(Board& brd){
    std::cout << std::endl;

    std::cout << "Expected: " << "Actual" << std::endl;
    std::cout << "False: " << brd.inCheck(white) << std::endl;

    Piece* add_knight = new Piece();
    add_knight->owner = white;
    add_knight->type = N;
    add_knight->square = &(*brd.getSquareMap())["d6"];
    brd.addPiece(add_knight);
    std::cout << "True: " << brd.inCheck(black) << std::endl;
    brd.removePiece(add_knight);

    Piece* add_queen = new Piece();
    add_queen->owner = white;
    add_queen->type = Q;
    add_queen->square = &(*brd.getSquareMap())["a4"];
    brd.addPiece(add_queen);
    brd.removePiece((*brd.getSquareMap())["d7"].piece);
    std::cout << "True: " << brd.inCheck(black) << std::endl;
    Piece* add_back_pawn = new Piece();
    add_back_pawn->owner = black;
    add_back_pawn->type = p;
    add_back_pawn->square = &(*brd.getSquareMap())["d7"];
    brd.addPiece(add_back_pawn);
    brd.removePiece(add_queen);

    add_queen = new Piece();
    add_queen->owner = black;
    add_queen->type = Q;
    add_queen->square = &(*brd.getSquareMap())["a4"];
    brd.addPiece(add_queen);
    brd.removePiece((*brd.getSquareMap())["d7"].piece);
    std::cout << "False: " << brd.inCheck(black) << std::endl;
    add_back_pawn = new Piece();
    add_back_pawn->owner = black;
    add_back_pawn->type = p;
    add_back_pawn->square = &(*brd.getSquareMap())["d7"];
    brd.addPiece(add_back_pawn);
    brd.removePiece(add_queen);

    std::cout << std::endl;
}

void BoardTests::copyTest(Board& brd){
    std::cout << std::endl;

    brd.removePiece((*(brd.getSquareMap()))["d2"].piece);
    bool truth = (*brd.getSquareMap())["d2"].piece == nullptr;

    std::cout << "Should be 1: " << truth << std::endl;

    std::cout << std::endl;
}

void BoardTests::runTests(){
    std::cout << "Running Board tests..." << std::endl;
    usleep(500000);

    Board myBoard;

    // squareLocTest(myBoard);
    // parseMoveTest(myBoard);
    // pieceMapTest(myBoard);
    // deletePieceTest(myBoard);
    // inCheckTest(myBoard);
    copyTest(myBoard);

    std::cout << "Board tests completed." << std::endl;
}