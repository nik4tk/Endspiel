#pragma once
#include "board.h"
#include <vector>

inline u64 RANK_2 = 0ULL;
inline u64 RANK_7 = 0ULL;
inline u64 FILE_A = 0ULL;
inline u64 FILE_H = 0ULL;
inline u64 FILES_AB = 0ULL;
inline u64 FILES_GH = 0ULL;

extern u64 KNIGHT_MOVES[64];
extern u64 KING_MOVES[64];

struct Move {
	int from;
	int to;
};

struct MoveList {
	Move moves[256]; // 218, because a normal chess-position only has ~218 legal moves
	int count = 0;

	void push(Move m) { moves[count++] = m; }
};

void startUp();

MoveList generateWhitePawnMoves(const Board& board);
MoveList generateBlackPawnMoves(const Board& board);

void initKnightMoves();
MoveList generateWhiteKnightMoves(const Board& board);
MoveList generateBlackKnightMoves(const Board& board);

void initKingMoves();
MoveList generateWhiteKingMoves(const Board& board);
MoveList generateBlackKingMoves(const Board& board);

MoveList generateWhiteBishopMoves(const Board& board);
MoveList generateBlackBishopMoves(const Board& board);

MoveList generateWhiteRookMoves(const Board& board);
MoveList generateBlackRookMoves(const Board& board);