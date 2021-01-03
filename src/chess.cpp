#include <iostream>
#include <stdlib.h>
#include "chess.h"
#include "cpu.h"
#include "board.h"

using namespace std;

colors_t cpu_color;
colors_t prompt_color();
bool cpu_turn = false;

int main(){

    // Instantiate a board
    Board brd;
    
    // Choose color
    cpu_color = prompt_color();

    // "cpu.cpp" is the file, "Cpu" is the class/constructor, and "comp" is the instance
    Cpu comp(cpu_color, brd);

    if(cpu_color == white){
        cout << "I am white." << endl;
        cpu_turn = true;
    }
    else{
        cout << "I am black." << endl;
    }

    // Main game loop
    while(true){
        if(cpu_turn){
            cout << comp.getMove() << endl;
            break;
        }
        else{
            // Wait for user input
            break;
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