#pragma once
#include "board.h"
#include <vector>

u64 RANK_2 = 0ULL;
u64 RANK_7 = 0ULL;
u64 FILE_A = 0ULL;
u64 FILE_H = 0ULL;
u64 FILES_AB = 0ULL;
u64 FILES_GH = 0ULL;

extern u64 KNIGHT_MOVES[64];

struct Move {
	int from;
	int to;
};

void startUp();

std::vector<Move> generateWhitePawnMoves(const Board& board);
std::vector<Move> generateWhitePawnMoves(const Board& board);
void initKnightMoves();
std::vector<Move> generateWhiteKnightMoves(const Board& board);
