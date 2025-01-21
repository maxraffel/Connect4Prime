#include <iostream>
#include "Position.h"
#include "Solver.h"

using namespace std;

int main() {
    Position pos1;

    pos1.printPosition();
    cout << calculateNextMove(pos1) << endl;
}