#include <cstdint>
#include <iostream>
#include "Position.h"

class MoveSorter {
    public:

    void add(int moveMask, int score);
    int nextMove();

    int size = 0;

    private:

    
    struct {int move; int score; } entries[Position::BOARD_WIDTH];  
};