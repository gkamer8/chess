#include "main_tests.h"
#include "../src/board.h"

#ifndef board_tests
#define board_tests

class BoardTests : public Test{
    private:
        void squareLocTest(Board&);
        void parseMoveTest(Board&);
        void pieceMapTest(Board&);
    public:
        void runTests();
};

#endif