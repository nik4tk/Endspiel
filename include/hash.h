#pragma once
#include "board.h"
#include <random>

// The lookup tables for our random numbers
extern u64 pieceKeys[13][64];
extern u64 enPassantKeys[8];
extern u64 castleKeys[16];
extern u64 sideKey;

void initZobrist();
u64 generateHash(const Board& board);