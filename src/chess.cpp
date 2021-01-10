#include <iostream>
#include <stdlib.h>
#include "chess.h"
#include "cpu.h"
#include "board.h"
#include "../tests/main_tests.h"

using namespace std;

colors_t cpu_color;
colors_t prompt_color();
bool cpu_turn = false;

// If true, only tests are run before program closes.
bool test_run = true;

int main(){

    if(test_run){
        Test allTests;
        allTests.runTests();
        exit(0);
    }

    // Instantiate a board
    Board brd;
    
    // Choose color
    cpu_color = prompt_color();

    Cpu comp(cpu_color, &brd);

    if(cpu_color == white){
        cout << "I am white." << endl;
        cpu_turn = true;
    }
    else{
        cout << "I am black." << endl;
    }

    // User game loop
    while(true){
        if(brd.getMoveColor() == white){
            cout << std::to_string(brd.getMoveNum()) << ": ";
        }
        else{  
            cout << std::to_string(brd.getMoveNum()) << ": ... ";
        }
        // Wait for user input
        std::string user_move;
        cin >> user_move;
        if(user_move == "show"){
            brd.display();
        }
        else{
            try{
                brd.makeMove(user_move);
            }
            catch(const char* e){
                cout << e << std::endl;
            }
        }
    }

    // Main game loop
    exit(0);
    while(true){
        if(cpu_turn){
            cout << comp.getMove() << endl;
            break;
        }
        else{
            // Wait for user input
            std::string user_move;
            cin >> user_move;
            try{
                brd.makeMove(user_move);
            }
            catch(const char* e){
                cout << e;
            }
        }
    }
}

// Returns the oppposite color from what the user selects
colors_t prompt_color(){
    while(true){
        char col_choice;
        cout << "Black (b), white (w), or random (r)?\n";
        // The input choice is the opposite of the cpu color
        if(cin >> col_choice){
            if(col_choice == 'b')
                return white;
            else if(col_choice == 'w')
                return black;
            else if(col_choice == 'r'){
                srand((unsigned int)time(NULL));  // Seed
                return (rand() % 2 == 0) ? white : black;
            }
            else
                cout << "Selection invalid" << endl;
        }
        else{
            cout << "Selection invalid" << endl;
        }
    }
}