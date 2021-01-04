#include "main_tests.h"
#include "board_tests.h"
#include <unistd.h>
#include <iostream>

void Test::runTests(){
    std::cout << "Running all tests..." << std::endl;
    usleep(2000000);  // Unix system sleep command (this is for Mac) - in microseconds
    
    BoardTests boardTests;
    boardTests.runTests();

    std::cout << "All tests completed." << std::endl;
}