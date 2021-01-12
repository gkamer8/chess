#include "../src/cpu.h"
#include "cpu_tests.h"
#include "../src/board.h"
#include <iostream>
#include <unistd.h>
#include <vector>
using namespace std;

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

// Imperfect move to string function
std::string getNaiveName(Move& move, Board* brd){
    if(move.ks_castle){
        return "0-0";
    }
    else if(move.qs_castle){
        return "0-0-0";
    }
    std::string n = "";
    n += pieceToPrefix(move.piece->type);
    if((*(brd->getSquareMap()))[move.to->name].piece != nullptr && (*(brd->getSquareMap()))[move.to->name].piece->owner != move.piece->owner){
        if(move.piece->type == p){
            n += move.piece->square->file;
        }
        n += "x";
    }
    n += move.to->name;
    return n;
}

void getLegalMovesTest(){
    cout << endl;
    Board brd;
    Cpu comp(white, &brd);
    brd.makeMove("e4");
    brd.makeMove("e5");
    brd.makeMove("Nf3");
    brd.makeMove("Nc6");
    brd.makeMove("Bb5");
    brd.makeMove("Nf6");
    brd.makeMove("Nc3");
    brd.makeMove("d5");
    brd.display();
    std::string mvs = "";
    vector<Move> lst = comp.getLegalMoves(brd);
    for(int i = 0; i < lst.size(); i++){
        mvs += getNaiveName(lst.at(i), &brd);
        mvs += ", ";
    }
    cout << mvs.substr(0, mvs.size() - 2) << endl;
    cout << endl;

}

void CpuTests::runTests(){
    cout << "Starting cpu tests..." << endl;
    usleep(500000);

    getLegalMovesTest();

    cout << "Cpu tests completed." << endl;
}