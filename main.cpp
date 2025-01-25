#include <iostream>
#include "Position.h"
#include "Solver.h"

using namespace std;

int main() {
    Position pos1;

    while (!pos1.hasWon()) {
        pos1.printPosition();
        int pMove;
        cin >> pMove;

        pos1.play(pMove);
        pos1.play(calculateNextMove(pos1));
    }

    pos1.printPosition();
}