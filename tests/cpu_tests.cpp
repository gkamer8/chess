#include "../src/cpu.h"
#include "cpu_tests.h"
#include "../src/board.h"
#include <iostream>
#include <unistd.h>
#include <vector>
using namespace std;

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
    brd.makeMove("d3");
    brd.makeMove("Nd7");
    brd.makeMove("Bd2");
    brd.display();
    std::string mvs = "";
    vector<Move> lst = comp.getLegalMoves(brd);
    for(int i = 0; i < lst.size(); i++){
        mvs += brd.getName(lst.at(i));
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