# chess

Homemade chess engine.

## General implementation

The file chess.cpp contains main and begins by prompting the user for a color. Then, a board object is instantiated along with a cpu object (constructed with the color and board object). Turns alternate between the player and the CPU. Moves are inputted with algebraic notation. 

## Board representation

The board is internally represented as an unordered map called square_map from strings (algebraic coordinates) to Square structs, which store, among other things, pointers to adjacent squares in the map. Occupied squares also store a pointer to the occupying Piece struct should there by a piece on that square. The piece structs store their type (Q, R, etc.), owner (black, white), and a pointer back to their square. There also is a hash map for black and white pieces (black_piece_map and white_piece_map) that hash piece types (Q, R, etc.) to lists of piece pointers.

This structure allows for a natural traversal of the board as well as a quick way to survey pieces with quick access to the squares that they're on.

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
       \ _______         \ ___
   ...    ...    \      ...    \                    ...    ...     ...
    |      |      \      |      \                    |      |       |
| Piece  Piece  Piece  Piece  Piece  ...           Piece  Piece  Piece  ...
     |     |       |     |        |                   |     |       |
| [Piece*, ...] [Piece*, ...]  [Piece*, ...] ...   [Piece*, ...] [Piece*, ...] ...
|       |               |             |                 |               |
|   ____|_______________|_____________|____ ...     ____|_______________|___ ...
|                     |                                         |
|             Hash (black_piece_map)                 Hash (white_piece_map)
                          |                                     |
                          |                                     |
              piece types (i.e., p, R)              piece types (i.e., p, R)

</pre>

