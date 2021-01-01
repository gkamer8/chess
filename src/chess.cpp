#include <iostream>
#include <stdlib.h>
#include "chess.h"

using namespace std;

colors_t cpu_color;

int main(){

    // Choose color
    while(true){
        char col_choice;
        cout << "Black (b), white (w), or random (r)?\n";
        // The input choice is the opposite of the cpu color
        if(cin >> col_choice){
            if(col_choice == 'b'){
                cpu_color = white;
                break;
            }
            else if(col_choice == 'w'){
                cpu_color = black;
                break;
            }
            else if(col_choice == 'r'){
                srand((unsigned int)time(NULL));  // Seed
                cpu_color = (rand() % 2 == 0) ? white : black;
                break;
            }
            else{
                cout << "Selection invalid" << endl;
            } 
        }
        else{
            cout << "Selection invalid" << endl;
        }
    }

    if(cpu_color == white){
        cout << "I am white." << endl;
    }
    else{
        cout << "I am black." << endl;
    }

    // Main game loop
    while(true){
        break;
    }
}