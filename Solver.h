#include "Position.h"

int calculateNextMove(Position pos);

int scoreMove(Position& pos, int &visited, int alpha, int beta);

void playNextMove(Position &pos);