#include "MoveSorter.h"

void MoveSorter::add(int move, int score)
{
    int pos = size++;
    for (; pos && entries[pos - 1].score > score; --pos) {
        entries[pos] = entries[pos - 1];
    }
        
    entries[pos].move = move;
    entries[pos].score = score;
}

int MoveSorter::nextMove()
{
    if (size)
        return entries[--size].move;
    else
        return 0;
};