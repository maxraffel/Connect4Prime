#include "Position.h"

class Solver {
    public:
    Solver() {
        for(int i = 0; i < Position::BOARD_WIDTH; i++)
        columnOrder[i] = Position::BOARD_WIDTH/2 + (1-2*(i%2))*(i+1)/2;   
    }
    int calculateNextMove(Position pos);

    int scoreMove(Position& pos, int &visited, int alpha, int beta);

    void playNextMove(Position &pos);

    int columnOrder[Position::BOARD_WIDTH];
};

