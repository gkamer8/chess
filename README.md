# chess

Homemade chess engine.

## General implementation

The file chess.cpp contains main and begins by prompting the user for a color. Then, a board object is instantiated along with a cpu object (constructed with the color and board object). Turns alternate between the player and the CPU. Moves are inputted with algebraic notation. 

## Board representation

The board is internally represented as an unordered map called square_map from strings (algebraic coordinates) to Square structs, which store, among other things, pointers to adjacent squares in the map. Occupied squares also store a pointer to the occupying Piece struct. The piece structs store pointers to other pieces (and back to their Square) in the form of a doubly linked list. The head of the list is stored as the head_piece variable. The pieces are also pointed to by piece_map, which maps piece types (Q, R, N, etc.) to lists of piece pointers.

This structure allows for a natural traversal of the board as well as a quick way to survey all the pieces with quick access to the squares that they're on.

<pre>
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
                    |           |
|                [Piece*, ...] [Piece*, ...]
|                      |             |
|                      |_____________|__________ ...
|                         |
|                   Hash (piece_map)
                          |
                          |
              piece types (i.e., p, R, N, etc.)

</pre>

