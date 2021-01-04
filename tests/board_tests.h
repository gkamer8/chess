#include "main_tests.h"
#include "../src/board.h"

#ifndef board_tests
#define board_tests

class BoardTests : public Test{
    private:
        void pieceListTest(Board&);
        void squareLocTest(Board&);
        void parseMoveTest(Board&);
    public:
        void runTests();
};

#endif