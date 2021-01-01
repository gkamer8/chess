#include <iostream>
#include <stdlib.h>
#include "chess.h"

using namespace std;

int cpu_color;

int main(){

    while(true){
        char col_choice;
        cout << "Black (b), white (w), or random (r)?\n";
        if(cin >> col_choice){
            if(col_choice == 'b'){
                cpu_color = BLACK;
                break;
            }
            else if(col_choice == 'w'){
                cpu_color = WHITE;
                break;
            }
            else if(col_choice == 'r'){
                cpu_color = rand() % 2 ? WHITE : BLACK;
                break;
            }
            else{
                cout << "Selection invalid" << "\n";
            } 
        }
        else{
            cout << "Selection invalid";
        }
    }

    cout << cpu_color << "\n";

    // Main game loop
    while(true){
        break;
    }
}