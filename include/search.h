#pragma once
#include "move.h"
#include "board.h"
#include "movegen.h"
#include "eval.h"

enum TTFlag
{
	TT_EXACT,
	TT_ALPHA,
	TT_BETA
};

struct TTEntry
{
	u64 hashKey;
	int depth;
	int flag;
	int score;
	Move bestMove;
};

const int TT_SIZE = 1000000;
extern TTEntry TT[TT_SIZE];

void clearTT();
void storeTT(u64 hash, int depth, int flag, int score, Move bestMove);
bool probeTT(u64 hash, int depth, int alpha, int beta, int& returnScore, Move& returnMove);

int minimax(Board& board, int depth, int alpha, int beta);
Move bestMove(Board& board, int depth);