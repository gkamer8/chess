#include "chess.h"
#include <string>
#include <unordered_map>

#ifndef board
#define board

enum piece_t {p, Q, K, N, B, R};

struct Piece;  // Because of cross dependency

struct Square{
    std::string name;  // ex. "b4"
    char file;  // ex. 'b'
    int rank;  // ex. 4

    Square* n;  // north
    Square* s;  // south
    Square* e;  // east
    Square* w;  // west
    Square* nw;  // northwest
    Square* ne;  // northeast
    Square* se;  // southeast
    Square* sw;  // southwest

    Piece* piece;  // Piece on the square
};

struct Piece{
    Square* square;  // location of the piece
    piece_t type;  // piece type (p, Q, R, etc.)
    colors_t owner;  // white or black
    Piece* next;  // For doubly linked list – not guaranteed to be any specific piece
    Piece* prev;  // SAA
};

struct Move{
    bool qs_castle;  // whether the move is a kingside castle
    bool ks_castle;  // whether the move is a queenside castle
    Piece* piece;  // piece being moved (includes current square)
    Square* to;  // where the piece is moving to
};

// For optimization - keys are simply algebraic coordinates; no need for complication
struct CustomHash {
    size_t operator()(const std::string coord) const {
        return ((coord[0] - 'a') * 8) + (coord[1] - '1');
    }
};

class Board{
    private:
        int move_num;
        colors_t move_color;
        bool white_ks_castle_eligible;
        bool black_ks_castle_eligible;
        bool white_qs_castle_eligible;
        bool black_qs_castle_eligible;
        bool check;

        // Doubly linked list for keeping track of pieces
        // Pieces should be removed as they are captured
        Piece head_piece;

        // The board's main datatype (this is "the board" in a conventional sense)
        std::unordered_map<std::string, Square, CustomHash> square_map;
    public:
        Board();
        void makeMove();
        bool isLegal(std::string);
        Piece* getHeadPiece();
        Move parseMove(std::string);
        std::unordered_map<std::string, Square, CustomHash> getSquareMap();
};

#endif