#include <iostream>
#include "Position.h"
#include <bitset>
#include <cstdint>

using namespace std;

Position::Position() {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        bottomMask += 1ULL << (i*(BOARD_HEIGHT+1));
    }
    boardMask -= (bottomMask << BOARD_HEIGHT);
}

int Position::turnsLeft() {
    return MAX_MOVES - turnsPassed;
}

void Position::play(int x) {
    currentPlayerMask ^= mask; //swap which player is in current player mask (so now mask represents old player)
    mask |= mask + (1ULL << ((BOARD_HEIGHT + 1)*x));
    turnsPassed++;
}

bool Position::hasWon(uint64_t playerMask) {
    // checks ever axis by &ing shifted versions twice
    // result will be 0 unless there is a run of 4 in that axis 

    // check horizontal
    uint64_t temp = (playerMask << ((BOARD_HEIGHT+1)*2)) & playerMask;
    if (temp & (temp << (BOARD_HEIGHT+1))) {
        return true;
    }
    // check vertical
    temp = (playerMask << 2) & playerMask;
    if (temp & (temp << 1)) {
        return true;
    }
    //check ++ diagonal (shift by BOARD_HEIGHT+1 + 1 for one diagonal)
    temp = (playerMask << ((BOARD_HEIGHT+2)*2)) & playerMask;
    if (temp & (temp << (BOARD_HEIGHT+2))) {
        return true;
    }

    //check +- diagonal (shift by BOARD_HEIGHT + 1 - 1)
    temp = (playerMask << (BOARD_HEIGHT*2)) & playerMask;
    if (temp & (temp << BOARD_HEIGHT)) {
        return true;
    }

    return false;
}

bool Position::hasWon() {
    return hasWon(currentPlayerMask^mask);
}

bool Position::isWinningMove(int x) {
    // simulate making the move on a temporary bitboard
    uint64_t tempPlayerMask = mask^currentPlayerMask;
    uint64_t tempMask = mask;
    tempMask |= tempMask + (1ULL << ((BOARD_HEIGHT + 1)*x));
    //cout << "isWinningMove: " << tempMask << ", " << tempPlayerMask << endl;
    //cout << "isWinningMove mask: " << (tempMask ^ tempPlayerMask) << endl;
    return hasWon(tempMask ^ tempPlayerMask);
}

bool Position::canPlay(int x) {
    return !(mask & (1ULL << (BOARD_HEIGHT - 1 + (BOARD_HEIGHT+1)*x)));
}


bool Position::isDraw() {
    return turnsPassed >= BOARD_WIDTH*BOARD_HEIGHT;
}

void Position::printPosition() { //figure out static
    // can redo with shift but speed of this function is pretty inconsequential
    bitset<64> maskBitset = bitset<64>(mask);
    bitset<64> currentPlayerBitset = bitset<64>(currentPlayerMask);
    for (int h = BOARD_HEIGHT - 1; h >= 0; h--) {
        for (int w = 0; w < BOARD_WIDTH; w++) {
            if (currentPlayerBitset[(BOARD_HEIGHT+1)*w + h]) {
                if (turnsPassed % 2 == 0) {
                    cout << "🔴";
                } else {
                    cout << "🟡";
                }
            } else if (maskBitset[(BOARD_HEIGHT+1)*w + h]) { // first player to move is red
                if (turnsPassed % 2 == 0) {
                    cout << "🟡";
                } else {
                    cout << "🔴";
                }
            } else {
                cout << "⚫";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void Position::printBitboard(uint64_t bitboardInt) {
    bitset<64>bitboard = bitset<64>(bitboardInt);

    for (int w = 0; w < BOARD_WIDTH; w++) {
            cout << bitboard[(BOARD_HEIGHT+1)*w + BOARD_HEIGHT] << " ";
    }

    cout << endl << "^buffer^" << endl;

    for (int h = BOARD_HEIGHT - 1; h >= 0; h--) {
        for (int w = 0; w < BOARD_WIDTH; w++) {
            cout << bitboard[(BOARD_HEIGHT+1)*w + h] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Position::printBitboard() {
    printBitboard(mask);
}

void Position::printMask() {
    printBitboard(mask);
}

void Position::printCurrentPlayerMask() {
    printBitboard(currentPlayerMask);
}

int Position::turns() {
    return turnsPassed;
}

uint64_t Position::winningSpaces(uint64_t playerMask, uint64_t fullMask) {
    uint64_t spaces = 0;

    // left shift by 1 is equivalent to shifting up vertically one
    // left shift by BOARD_HEIGHT+1 is equivalent to shifting right horizontally one

    // only need to check top of vertical - bar of 0s at the top blocks overflow from other columns
    spaces |= (playerMask << 1) & (playerMask << 2) & (playerMask << 3);

    // horizontal - first collect groups of 2 
    int64_t doubles = (playerMask << (BOARD_HEIGHT+1)) & (playerMask << (BOARD_HEIGHT+1)*2);
    // check on right side
    spaces |= doubles & (playerMask >> (BOARD_HEIGHT+1));
    spaces |= (doubles & playerMask) << (BOARD_HEIGHT+1);
    // left side
    doubles >>= (BOARD_HEIGHT+1)*3;
    spaces |= doubles & (playerMask << (BOARD_HEIGHT+1));
    spaces |= (doubles & playerMask) >> (BOARD_HEIGHT+1);

    //up-right diagonal
    doubles = (playerMask << (BOARD_HEIGHT+2)) & (playerMask << (BOARD_HEIGHT+2)*2);
    // check on right side
    spaces |= doubles & (playerMask >> (BOARD_HEIGHT+2));
    spaces |= (doubles & playerMask) << (BOARD_HEIGHT+2);
    // left side
    doubles >>= (BOARD_HEIGHT+2)*3;
    spaces |= doubles & (playerMask << (BOARD_HEIGHT+2));
    spaces |= (doubles & playerMask) >> (BOARD_HEIGHT+2);

    //down-left diagonal
    doubles = (playerMask << (BOARD_HEIGHT)) & (playerMask << (BOARD_HEIGHT)*2);
    // check on right side
    spaces |= doubles & (playerMask >> (BOARD_HEIGHT));
    spaces |= (doubles & playerMask) << (BOARD_HEIGHT);
    // left side
    doubles >>= (BOARD_HEIGHT)*3;
    spaces |= doubles & (playerMask << (BOARD_HEIGHT));
    spaces |= (doubles & playerMask) >> (BOARD_HEIGHT);

    return spaces & (spaces ^ fullMask);
}

uint64_t Position::possibleMoveMask() {
    return (mask + bottomMask) & boardMask;
}

uint64_t Position::goodMoveMask() {
    uint64_t possibleMoves = possibleMoveMask();
    uint64_t opponentWins = winningSpaces(currentPlayerMask^mask, mask);
    uint64_t forcedMoves = possibleMoves & opponentWins;
    if (forcedMoves) {
        if (forcedMoves & (forcedMoves - 1)) {
        return 0;
        } else {
            possibleMoves = forcedMoves;
        }
    }
    return possibleMoves & ~(opponentWins >> 1);
}

// uint64_t Position::columnMask(int x) {
//     return ((1ULL << BOARD_HEIGHT)-1) << x*(BOARD_HEIGHT+1);
// }

int Position::popcount(uint64_t x)
{
    int count;
    for (count=0; x; count++)
        x &= x - 1;
    return count;
}

int Position::orderScore(uint64_t moveMask) {
    return popcount(winningSpaces(currentPlayerMask | moveMask, mask));
}