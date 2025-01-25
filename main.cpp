#include <iostream>
#include "Position.h"
#include "Solver.h"

using namespace std;

int main() {
    Position pos1;
    pos1.play(1);

    pos1.printPosition();
    cout << calculateNextMove(pos1) << endl;
    cout << pos1.isWinningMove(0) << endl;
}