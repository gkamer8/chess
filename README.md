# chess

Homemade chess engine.

## General implementation

The file chess.cpp contains main and begins by prompting the user for a color. Then, a board object is instantiated along with a cpu object (constructed with the color and board object). Turns alternate between the player and the CPU. Moves are inputted with algebraic notation. 

## Board representation

The board is internally represented as an unordered map called square_map from strings (algebraic coordinates) to Square structs, which store, among other things, pointers adjacent squares in the map. The squares also store pointers to Piece structs. The piece structs store pointers to other pieces in the form of a doubly linked list. The head of the list is stored as the head_piece variable.

| Square <--
|   |
| Square <-- 
| ...        <-- Hash (square_map) <-- algebraic coordinates
| Square <--
|   |   \ ____________
| Square <--          \
     \                 \
      \                 \
       \ _________       \ ____
                  \            \
| head_piece <--> Piece <--> Piece <--> ...
