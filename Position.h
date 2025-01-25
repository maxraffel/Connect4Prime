#include <cstdint>
#include <iostream>

using namespace std;

#ifndef POSITION_H
#define POSITION_H

// A class that represents a position in Connect4
// implemented with a bitboard for a main mask and current player mask
// 
// in the main mask a 1 bit represents a piece existing there
// in the player mask a 1 bit represents their piece existing
//
// 
// 


class Position {
    public:
        void printPosition();
        void printMask();
        void printCurrentPlayerMask();
        static const int BOARD_WIDTH = 4;
        static const int BOARD_HEIGHT = 4;
        static const int MAX_MOVES = BOARD_HEIGHT*BOARD_WIDTH;

        int turnsLeft();

        bool isDraw();
        bool hasWon(uint64_t playerMask);
        bool hasWon();
        bool isWinningMove(int x);

        bool canPlay(int x);
        void play(int x);

        int turns();
    private:
        uint64_t mask = 0;
        uint64_t currentPlayerMask = 0;
        int turnsPassed = 0;

        void printBitboard(uint64_t bitboard);
};

#endif