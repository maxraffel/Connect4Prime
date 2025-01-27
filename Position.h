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
        Position();
        void printPosition();
        void printBitboard(uint64_t bitboard);
        void printBitboard();
        void printMask();
        void printCurrentPlayerMask();
        static const int BOARD_WIDTH = 6;
        static const int BOARD_HEIGHT = 6;
        static const int MAX_MOVES = BOARD_HEIGHT*BOARD_WIDTH;

        uint64_t winningSpaces(uint64_t playerMask, uint64_t fullMask);

        int turnsLeft();

        bool isDraw();
        bool hasWon(uint64_t playerMask);
        bool hasWon();
        bool isWinningMove(int x);

        bool canPlay(int x);
        void play(int x);
        uint64_t possibleMoveMask();
        // a 'good' move is one that does not directly lead to a loss
        uint64_t goodMoveMask();

        int turns();
        static constexpr uint64_t columnMask(int col) {
            return ((UINT64_C(1) << BOARD_HEIGHT)-1) << col*(BOARD_HEIGHT+1);
        }

        int popcount(uint64_t x);
        int orderScore(uint64_t moveMask);

    //private:
        uint64_t mask = 0;
        uint64_t currentPlayerMask = 0;
        uint64_t bottomMask = 0;
        // used to eliminate values in the buffer row
        uint64_t boardMask = 0 - 1ULL;

        int turnsPassed = 0;
};

#endif