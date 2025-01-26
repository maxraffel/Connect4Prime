#include <iostream>
#include "Position.h"
#include "Solver.h"

using namespace std;

int main() {
    Position pos1;
    Solver sol;
    // pos1.play(0);
    // pos1.play(1);
    // pos1.play(1);
    // pos1.play(2);
    // pos1.play(3);
    // pos1.play(2);
    // pos1.play(2);
    // pos1.play(3);
    // pos1.play(3);
    // pos1.play(3);
    // pos1.play(4);
    // pos1.play(3);
    // //pos1.play(1);

    // pos1.printBitboard();
    // pos1.printBitboard(pos1.winningSpaces(pos1.currentPlayerMask^pos1.mask, pos1.mask));
    // pos1.printBitboard(pos1.columnMask(2));

    while (!pos1.hasWon()) {
        pos1.printPosition();
        int pMove;
        cin >> pMove;

        pos1.play(pMove);
        pos1.play(sol.calculateNextMove(pos1));
    }

    pos1.printPosition();
}