#include "main_tests.h"
#include "../src/board.h"

#ifndef board_tests
#define board_tests

class BoardTests : public Test{
    private:
        void squareLocTest(Board&);
        void parseMoveTest(Board&);
        void pieceMapTest(Board&);
        void deletePieceTest(Board&);
        void inCheckTest(Board&);
        void copyTest(Board&);
        void isLegalTest(Board&);
        void displayTest(Board&);
        void moveTest(Board&);
    public:
        void runTests();
};

#endif