#include <iostream>
#include "Position.h"
#include "Solver.h"

void playNextMove(Position &pos) {
    pos.play(calculateNextMove(pos));
}

int calculateNextMove(Position pos) {
    int bestMove;
    int baseline = -(pos.BOARD_WIDTH*pos.BOARD_HEIGHT - pos.turns());
    int nodesVisited = 0;
    for (int i = 0; i < pos.BOARD_WIDTH; i++) {
        Position tempPos(pos);
        if (pos.canPlay(i)) {
            tempPos.play(i);
            int score = -scoreMove(tempPos, nodesVisited);

            if (score >= baseline) {
                baseline = score;
                bestMove = i;
            }
        }
    }
    cout << "Visited nodes: " << nodesVisited << endl;
    return bestMove;
}

// score the position for the current player
int scoreMove(Position pos, int &visited) {
    visited++;
    if (pos.isDraw()) return 0;

    for (int i = 0; i < pos.BOARD_WIDTH; i++) {
        if (pos.canPlay(i) && pos.isWinningMove(i)) {
            return pos.turnsLeft();
        }
    }

    int baseline = -(pos.turnsLeft());
    for (int i = 0; i < pos.BOARD_WIDTH; i++) {
        if (pos.canPlay(i)) {
            Position tempPos(pos);
            tempPos.play(i);
            int score = -scoreMove(tempPos, visited);

            if (score >= baseline) {
                baseline = score;
            }
        }
    }
    return baseline;
}