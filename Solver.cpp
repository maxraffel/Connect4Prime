#include <iostream>
#include "Position.h"
#include "Solver.h"
#include "MoveSorter.h"




void Solver::playNextMove(Position &pos) {
    pos.play(calculateNextMove(pos));
}

int Solver::calculateNextMove(Position pos) {
    int bestMove;
    int baseline = -(pos.BOARD_WIDTH*pos.BOARD_HEIGHT - pos.turns());
    int nodesVisited = 0;
    for (int i = 0; i < pos.BOARD_WIDTH; i++) {
        Position tempPos(pos);
        if (pos.canPlay(i)) {
            if (tempPos.isWinningMove(i)) return i;
            tempPos.play(i);
            int score = -scoreMove(tempPos, nodesVisited, -pos.turnsLeft(), pos.turnsLeft());

            if (score >= baseline) {
                baseline = score;
                bestMove = i;
            }
        }
    }
    cout << "Visited nodes: " << nodesVisited << endl;
    return bestMove;
}

// score the position for the current player - + score means current player wins with X plays remaining, negative means opponent wins with X plays remaining
int Solver::scoreMove(Position& pos, int &visited, int alpha, int beta) {
    visited++;
    uint64_t goodMoves = pos.goodMoveMask();
    int remainingMoves = Position::MAX_MOVES - pos.turns();

    // we lose next turn
    if (goodMoves == 0) {
        return -remainingMoves / 2;
    }

    if (pos.isDraw()) return 0;

    // worst case opponent wins the turn AFTER their next turn (we already verified they cannot win next turn)
    int minimumScore = -(remainingMoves - 2) / 2;

    if (alpha < minimumScore) {
        alpha = minimumScore;
        if (alpha >= beta) {
            return alpha;
        }
    }

    // best case scenario is we win the turn after our opponent plays (they will have verified no winning move)
    int maximumScore = (remainingMoves - 1) / 2;

    if (beta > maximumScore) {
        beta = maximumScore;
        if (alpha >= beta) {
            return beta;
        }
    }

    // for (int i = 0; i < pos.BOARD_WIDTH; i++) {
    //     if (pos.canPlay(i) && pos.isWinningMove(i)) {
    //         return pos.turnsLeft();
    //     }
    // }

    // now actually start looking at next moves
    MoveSorter sorter;
    for (int i = 0; i < pos.BOARD_WIDTH; i++) {
        uint64_t moveMask = goodMoves & Position::columnMask(columnOrder[i]);
        if (moveMask /*pos.canPlay(columnOrder[i])*/) {
            sorter.add(columnOrder[i], pos.orderScore(moveMask));


            // Position tempPos(pos);
            // tempPos.play(columnOrder[i]);
            // int score = -scoreMove(tempPos, visited, -beta, -alpha);

            // if (score >= beta) {
            //     return score;
            // }
            // if (score > alpha) {
            //     alpha = score;
            // }
        }
    }

    for (int i = 0; i < sorter.size; i++) {
        Position tempPos(pos);
        tempPos.play(sorter.nextMove());
        int score = -scoreMove(tempPos, visited, -beta, -alpha);
        if (score >= beta) {
            return score;
        }
        if (score > alpha) {
            alpha = score;
        }
    }
    return alpha;
}

