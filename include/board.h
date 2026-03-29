#pragma once
#include <cstdint>


using u64 = uint64_t;

#define set_bit(board, sq) ((board) |= (1ULL << sq))
#define clear_bit(board, sq) ((board) &= ~(1ULL << sq))
#define get_LSB(board) (__builtin_ctzll(board))

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

	void init();
	void print();
};
