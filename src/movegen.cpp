#include "movegen.h"

void startUp()
{
	for (int sq = 8; sq <= 15; sq++)
	{
		set_bit(RANK_2, sq);
	}

	for (int sq = 48; sq <= 55; sq++)
	{
		set_bit(RANK_7, sq);
	}

	for (int sq = 0; sq <= 48; sq+=8)
	{
		set_bit(FILE_A, sq);
		set_bit(FILES_AB, sq);
	}
	for (int sq = 1; sq <= 49; sq+=8)
	{
		set_bit(FILES_AB, sq);
	}

	for (int sq = 7; sq <= 63; sq+=8)
	{
		set_bit(FILE_H, sq);
		set_bit(FILES_GH, sq);
	}
	for (int sq = 6; sq < 62; sq+=8)
	{
		set_bit(FILES_GH, sq);
	}
	

	initKnightMoves();
}

std::vector<Move> generateWhitePawnMoves(const Board& board)
{
	std::vector<Move> moves;

	u64 emptySquares = ~(board.whitePieces() | board.blackPieces());
	u64 pawns = board.whitePawns;

	// Single push
	u64 singlePush = (pawns << 8) & emptySquares;
	while (singlePush)
	{
		int to = get_LSB(singlePush);
		clear_bit(singlePush, to);
		moves.push_back({ to - 8, to });
	}

	// Double push
	u64 doublePush = ((pawns & RANK_2) << 8) & emptySquares;
	doublePush = (doublePush << 8) & emptySquares;
	while (doublePush)
	{
		int to = get_LSB(doublePush);
		clear_bit(doublePush, to);
		moves.push_back({ to - 16, to });
	}

	// Captures
	u64 capturesLeft = ((pawns << 7) & ~FILE_H) & board.blackPieces();
	while (capturesLeft)
	{
		int to = get_LSB(capturesLeft);
		clear_bit(capturesLeft, to);
		moves.push_back({ to - 7, to });
	}

	u64 capturesRight = ((pawns << 9) & ~FILE_A) & board.blackPieces();
	while (capturesRight)
	{
		int to = get_LSB(capturesRight);
		clear_bit(capturesRight, to);
		moves.push_back({ to - 9, to });
	}

	return moves;
}

std::vector<Move> generateBlackPawnMoves(const Board& board)
{
	std::vector<Move> moves;

	u64 emptySquares = ~(board.whitePieces() | board.blackPieces());
	u64 pawns = board.blackPawns;

	// Single push
	u64 singlePush = (pawns >> 8) & emptySquares;
	while (singlePush)
	{
		int to = get_LSB(singlePush);
		clear_bit(singlePush, to);
		moves.push_back({ to + 8, to });
	}

	// Double push
	u64 doublePush = ((pawns & RANK_7) >> 8) & emptySquares;
	doublePush = (doublePush << 8) & emptySquares;
	while (doublePush)
	{
		int to = get_LSB(doublePush);
		clear_bit(doublePush, to);
		moves.push_back({ to + 16, to });
	}

	// Captures
	u64 capturesLeft = ((pawns >> 7) & ~FILE_A) & board.whitePieces();
	while (capturesLeft)
	{
		int to = get_LSB(capturesLeft);
		clear_bit(capturesLeft, to);
		moves.push_back({ to + 7, to });
	}

	u64 capturesRight = ((pawns >> 9) & ~FILE_H) & board.whitePieces();
	while (capturesRight)
	{
		int to = get_LSB(capturesRight);
		clear_bit(capturesRight, to);
		moves.push_back({ to + 9, to });
	}

	return moves;
}

void initKnightMoves()
{
	for (int sq = 0; sq < 63; sq++)
	{
		u64 knight = 1ULL << sq;
		KNIGHT_MOVES[sq] =
			((knight << 17) & ~FILE_A) |
			((knight << 15) & ~FILE_H) |
			((knight << 10) & ~FILES_AB) |
			((knight << 6) & ~FILES_GH) |
			((knight >> 17) & ~FILE_H) |
			((knight >> 15) & ~FILE_A) |
			((knight >> 10) & ~FILES_GH) |
			((knight >> 6) & ~FILES_AB);
	}
}

std::vector<Move> generateWhiteKnightMoves(const Board& board)
{
	std::vector<Move> moves;
	u64 knights = board.whiteKnights;
	while (knights)
	{
		int from = get_LSB(knights);
		clear_bit(knights, from);
		u64 captures = KNIGHT_MOVES[from] & ~board.whitePieces();
		while (captures)
		{
			int to = get_LSB(captures);
			clear_bit(captures, to);
			moves.push_back({ from, to });
		}
	}
	return moves;
}


