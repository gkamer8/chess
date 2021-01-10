#include "chess.h"
#include <string>
#include <unordered_map>
#include <list>
#include <vector>

#ifndef board
#define board

enum piece_t {p, Q, K, N, B, R};

struct Piece;  // Because of cross dependency

struct Square{
    std::string name;  // ex. "b4"
    char file;  // ex. 'b'
    int rank;  // ex. 4
    sq_colors_t clr;  // color of the square - dark or light

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
};

struct Move{
    bool qs_castle;  // whether the move is a kingside castle
    bool ks_castle;  // whether the move is a queenside castle
    Piece* piece;  // piece being moved (includes current square)
    Square* to;  // where the piece is moving to
    piece_t promotedTo;
    std::string enPassant;  // Coordinates of the piece being taken en passant
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
        std::string enPassantLoc;
        std::vector<std::string> moves;
 
        // The board's main datatype (this is "the board" in a conventional sense)
        std::unordered_map<std::string, Square, CustomHash> square_map;

        // Maps from piece type to Piece pointers (i.e. K --> [(King, e2, etc.)])
        std::unordered_map< piece_t, std::list<Piece*>, std::hash<int> > white_piece_map;
        std::unordered_map< piece_t, std::list<Piece*>, std::hash<int> > black_piece_map;

    public:
        Board();
        Board(Board&);
        void makeMove(std::string);
        Move parseMove(std::string);
        std::unordered_map<std::string, Square, CustomHash>* getSquareMap();
        std::unordered_map< piece_t, std::list<Piece*>, std::hash<int> >* getBlackPieceMap();
        std::unordered_map< piece_t, std::list<Piece*>, std::hash<int> >* getWhitePieceMap();
        void setMoveColor(colors_t);
        void removePiece(Piece*);
        void addPiece(Piece*);
        bool inCheck(colors_t);
        void executeMove(Move&);
        bool isLegal(Move&);
        bool inCheckmate();
        void display();
        colors_t getMoveColor();
        int getMoveNum();
};

#endif