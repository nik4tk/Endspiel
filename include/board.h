#pragma once
#include <cstdint>
#include <bit>

#include "move.h"

using u64 = uint64_t;

#define set_bit(board, sq) ((board) |= (1ULL << sq))
#define clear_bit(board, sq) ((board) &= ~(1ULL << sq))

inline int get_LSB(u64 board)
{
	return std::countr_zero(board);
}


class Board
{
public:
	u64 whitePawns, whiteKnights, whiteBishops, whiteRooks, whiteQueens, whiteKing;
	u64 blackPawns, blackKnights, blackBishops, blackRooks, blackQueens, blackKing;

	// helper-fnct
	u64 whitePieces() const
	{
		return whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;
	}

	u64 blackPieces() const
	{
		return blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;
	}

	u64 allPieces() const
	{
		return whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing | blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;
	}

	void init();
	void print();

	int getPieceOnSquare(int sq) const;

	void makeMove(const Move& m);
	void undoMove(const Move& m);
};
