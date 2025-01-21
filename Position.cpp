#include <iostream>
#include "Position.h"
#include <bitset>
#include <cstdint>

using namespace std;


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
    //cout << maskBitset << endl;
    for (int h = BOARD_HEIGHT - 1; h >= 0; h--) {
        for (int w = 0; w < BOARD_WIDTH; w++) {
            if (currentPlayerBitset[(BOARD_HEIGHT+1)*w + h]) {
                if (turnsPassed % 2 == 0) {
                    cout << "🔴";
                } else {
                    cout << "🟡";
                }
            } else if (maskBitset[(BOARD_WIDTH+1)*w + h]) { // first player to move is red
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

    for (int h = BOARD_HEIGHT - 1; h >= 0; h--) {
        for (int w = 0; w < BOARD_WIDTH; w++) {
            cout << bitboard[(BOARD_HEIGHT+1)*w + h] << " ";
        }
        cout << endl;
    }
    cout << endl;
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