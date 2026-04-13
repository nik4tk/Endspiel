#include "movegen.h"

u64 KNIGHT_MOVES[64];
u64 KING_MOVES[64];

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

	for (int sq = 0; sq <= 56; sq+=8)
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
	initKingMoves();
}

void generateWhitePawnMoves(const Board& board, MoveList& moves)
{
	u64 emptySquares = ~(board.whitePieces() | board.blackPieces());
	u64 pawns = board.whitePawns;

	// Single push
	u64 singlePush = (pawns << 8) & emptySquares;
	while (singlePush)
	{
		int to = get_LSB(singlePush);
		clear_bit(singlePush, to);
		moves.push({ to - 8, to });
	}

	// Double push
	u64 doublePush = ((pawns & RANK_2) << 8) & emptySquares;
	doublePush = (doublePush << 8) & emptySquares;
	while (doublePush)
	{
		int to = get_LSB(doublePush);
		clear_bit(doublePush, to);
		moves.push({ to - 16, to });
	}

	// Captures
	u64 capturesLeft = ((pawns << 7) & ~FILE_H) & board.blackPieces();
	while (capturesLeft)
	{
		int to = get_LSB(capturesLeft);
		clear_bit(capturesLeft, to);
		moves.push({ to - 7, to });
	}

	u64 capturesRight = ((pawns << 9) & ~FILE_A) & board.blackPieces();
	while (capturesRight)
	{
		int to = get_LSB(capturesRight);
		clear_bit(capturesRight, to);
		moves.push({ to - 9, to });
	}
}

void generateBlackPawnMoves(const Board& board, MoveList& moves)
{
	u64 emptySquares = ~(board.whitePieces() | board.blackPieces());
	u64 pawns = board.blackPawns;

	// Single push
	u64 singlePush = (pawns >> 8) & emptySquares;
	while (singlePush)
	{
		int to = get_LSB(singlePush);
		clear_bit(singlePush, to);
		moves.push({ to + 8, to });
	}

	// Double push
	u64 doublePush = ((pawns & RANK_7) >> 8) & emptySquares;
	doublePush = (doublePush >> 8) & emptySquares;
	while (doublePush)
	{
		int to = get_LSB(doublePush);
		clear_bit(doublePush, to);
		moves.push({ to + 16, to });
	}

	// Captures
	u64 capturesLeft = ((pawns >> 9) & ~FILE_A) & board.whitePieces();
	while (capturesLeft)
	{
		int to = get_LSB(capturesLeft);
		clear_bit(capturesLeft, to);
		moves.push({ to + 7, to });
	}

	u64 capturesRight = ((pawns >> 7) & ~FILE_H) & board.whitePieces();
	while (capturesRight)
	{
		int to = get_LSB(capturesRight);
		clear_bit(capturesRight, to);
		moves.push({ to + 9, to });
	}
}

void initKnightMoves()
{
	for (int sq = 0; sq < 64; sq++)
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

void generateWhiteKnightMoves(const Board& board, MoveList& moves)
{
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
			moves.push({ from, to });
		}
	}
}

void generateBlackKnightMoves(const Board& board, MoveList& moves)
{
	u64 knights = board.blackKnights;
	while (knights)
	{
		int from = get_LSB(knights);
		clear_bit(knights, from);
		u64 captures = KNIGHT_MOVES[from] & ~board.blackPieces();
		while (captures)
		{
			int to = get_LSB(captures);
			clear_bit(captures, to);
			moves.push({ from, to });
		}
	}
}

void initKingMoves()
{
	for (int sq = 0; sq < 64; sq++)
	{
		u64 king = 1ULL << sq;
		KING_MOVES[sq] =
			(king << 8) |
			(king >> 8) |
			((king << 1) & ~FILE_A) |
			((king >> 1) & ~FILE_H) |
			((king << 9) & ~FILE_A) |
			((king << 7) & ~FILE_H) |
			((king >> 7) & ~FILE_A) |
			((king >> 9) & ~FILE_H);
	}
}

void generateWhiteKingMoves(const Board& board, MoveList& moves)
{
	u64 king = board.whiteKing;
	int from = get_LSB(king);
	clear_bit(king, from);
	u64 captures = KING_MOVES[from] & ~board.whitePieces();
	while (captures)
	{
		int to = get_LSB(captures);
		clear_bit(captures, to);
		moves.push({ from, to });
	}
}

void generateBlackKingMoves(const Board& board, MoveList& moves)
{
	u64 king = board.blackKing;
	int from = get_LSB(king);
	clear_bit(king, from);
	u64 captures = KING_MOVES[from] & ~board.blackPieces();
	while (captures)
	{
		int to = get_LSB(captures);
		clear_bit(captures, to);
		moves.push({ from, to });
	}
}

void generateWhiteBishopMoves(const Board& board, MoveList& moves)
{
	u64 bipshops = board.whiteBishops;
	u64 ownPieces = board.whitePieces();
	u64 enemyPieces = board.blackPieces();

	while (bipshops)
	{
		int from = get_LSB(bipshops);
		clear_bit(bipshops, from);

		// Up-right (+9)
		int sq = from;
		while (sq % 8 != 7 && sq < 56)
		{
			sq += 9;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Up-left (+7)
		sq = from;
		while (sq % 8 != 0 && sq < 56)
		{
			sq += 7;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Down-right (-7)
		sq = from;
		while (sq % 8 != 7 && sq > 7)
		{
			sq -= 7;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Down-left (-9)
		sq = from;
		while (sq % 8 != 0 && sq > 7)
		{
			sq -= 9;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}
	}
}

void generateBlackBishopMoves(const Board& board, MoveList& moves)
{
	u64 bipshops = board.blackBishops;
	u64 ownPieces = board.blackPieces();
	u64 enemyPieces = board.whitePieces();

	while (bipshops)
	{
		int from = get_LSB(bipshops);
		clear_bit(bipshops, from);

		// Up-right (+9)
		int sq = from;
		while (sq % 8 != 7 && sq < 56)
		{
			sq += 9;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Up-left (+7)
		sq = from;
		while (sq % 8 != 0 && sq < 56)
		{
			sq += 7;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Down-right (-7)
		sq = from;
		while (sq % 8 != 7 && sq > 7)
		{
			sq -= 7;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Down-left (-9)
		sq = from;
		while (sq % 8 != 0 && sq > 7)
		{
			sq -= 9;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}
	}
}

void generateWhiteRookMoves(const Board& board, MoveList& moves)
{
	u64 rooks = board.whiteRooks;
	u64 ownPieces = board.whitePieces();
	u64 enemyPieces = board.blackPieces();

	while (rooks)
	{
		int from = get_LSB(rooks);
		clear_bit(rooks, from);

		// Up (+8)
		int sq = from;
		while (sq < 56)
		{
			sq += 8;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Down (-8)
		sq = from;
		while (sq > 7)
		{
			sq -= 8;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Right (+1)
		sq = from;
		while (sq % 8 != 7)
		{
			sq += 1;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Left (-1)
		sq = from;
		while (sq % 8 != 0)
		{
			sq -= 1;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}
	}
}

void generateBlackRookMoves(const Board& board, MoveList& moves)
{
	u64 rooks = board.blackRooks;
	u64 ownPieces = board.blackPieces();
	u64 enemyPieces = board.whitePieces();

	while (rooks)
	{
		int from = get_LSB(rooks);
		clear_bit(rooks, from);

		// Up (+8)
		int sq = from;
		while (sq < 56)
		{
			sq += 8;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}
		
		// Down (-8)
		sq = from;
		while (sq > 7)
		{
			sq -= 8;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Right (+1)
		sq = from;
		while (sq % 8 != 7)
		{
			sq += 1;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Left (-1)
		sq = from;
		while (sq % 8 != 0)
		{
			sq -= 1;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}
	}
}

void generateWhiteQueenMoves(const Board& board, MoveList& moves)
{
	u64 queens = board.whiteQueens;
	u64 ownPieces = board.whitePieces();
	u64 enemyPieces = board.blackPieces();

	while (queens)
	{
		int from = get_LSB(queens);
		clear_bit(queens, from);

		// Up (+8)
		int sq = from;
		while (sq < 56)
		{
			sq += 8;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Down (-8)
		sq = from;
		while (sq > 7)
		{
			sq -= 8;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Right (+1)
		sq = from;
		while (sq % 8 != 7)
		{
			sq += 1;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Left (-1)
		sq = from;
		while (sq % 8 != 0)
		{
			sq -= 1;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Up-right (+9)
		sq = from;
		while (sq % 8 != 7 && sq < 56)
		{
			sq += 9;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Up-left (+7)
		sq = from;
		while (sq % 8 != 0 && sq < 56)
		{
			sq += 7;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Down-right (-7)
		sq = from;
		while (sq % 8 != 7 && sq > 7)
		{
			sq -= 7;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Down-left (-9)
		sq = from;
		while (sq % 8 != 0 && sq > 7)
		{
			sq -= 9;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}
	}
}

void generateBlackQueenMoves(const Board& board, MoveList& moves)
{
	u64 queens = board.blackQueens;
	u64 ownPieces = board.blackPieces();
	u64 enemyPieces = board.whitePieces();

	while (queens)
	{
		int from = get_LSB(queens);
		clear_bit(queens, from);

		// Up (+8)
		int sq = from;
		while (sq < 56)
		{
			sq += 8;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Down (-8)
		sq = from;
		while (sq > 7)
		{
			sq -= 8;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Right (+1)
		sq = from;
		while (sq % 8 != 7)
		{
			sq += 1;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Left (-1)
		sq = from;
		while (sq % 8 != 0)
		{
			sq -= 1;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Up-right (+9)
		sq = from;
		while (sq % 8 != 7 && sq < 56)
		{
			sq += 9;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Up-left (+7)
		sq = from;
		while (sq % 8 != 0 && sq < 56)
		{
			sq += 7;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Down-right (-7)
		sq = from;
		while (sq % 8 != 7 && sq > 7)
		{
			sq -= 7;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}

		// Down-left (-9)
		sq = from;
		while (sq % 8 != 0 && sq > 7)
		{
			sq -= 9;
			if (ownPieces & (1ULL << sq)) break;
			moves.push({ from, sq });
			if (enemyPieces & (1ULL << sq)) break;
		}
	}
}

MoveList generatePseudoMoves(const Board& board, bool whiteToMove)
{
	MoveList pseudo;

	if (whiteToMove)
	{
		generateWhitePawnMoves(board, pseudo);
		generateWhiteKnightMoves(board, pseudo);
		generateWhiteBishopMoves(board, pseudo);
		generateWhiteRookMoves(board, pseudo);
		generateWhiteQueenMoves(board, pseudo);
		generateWhiteKingMoves(board, pseudo);
	}
	else
	{
		generateBlackPawnMoves(board, pseudo);
		generateBlackKnightMoves(board, pseudo);
		generateBlackBishopMoves(board, pseudo);
		generateBlackRookMoves(board, pseudo);
		generateBlackQueenMoves(board, pseudo);
		generateBlackKingMoves(board, pseudo);
	}

	return pseudo;
}

MoveList generateWhiteLegalMoves(const Board& board)
{
	MoveList pseudo = generatePseudoMoves(board, true);
	MoveList legal;

	for (const Move& m : pseudo)
	{
		Board copy = board;
		copy.makeMove(m);

		int kingSq = get_LSB(copy.whiteKing);

		if (!isSquareAttacked(copy, kingSq, false)) legal.push(m);
	}

	return legal;
}

MoveList generateBlackLegalMoves(const Board& board)
{
	MoveList pseudo = generatePseudoMoves(board, false);
	MoveList legal;

	for (const Move& m : pseudo)
	{
		Board copy = board;
		copy.makeMove(m);

		int kingSq = get_LSB(copy.blackKing);

		if (!isSquareAttacked(copy, kingSq, true)) legal.push(m);
	}

	return legal;
}

bool isSquareAttacked(const Board& board, int sq, bool byWhite)
{
	u64 target = 1ULL << sq;
	u64 occupied = board.allPieces();

	// Pawns
	if (byWhite)
	{
		if (((board.whitePawns << 7) & ~FILE_H) & target) return true;
		if (((board.whitePawns << 9) & ~FILE_A) & target) return true;
	}
	else
	{
		if (((board.blackPawns >> 9) & ~FILE_A) & target) return true;
		if (((board.blackPawns >> 7) & ~FILE_H) & target) return true;
	}

	// Knights
	u64 knights = byWhite ? board.whiteKnights : board.blackKnights;
	while (knights)
	{
		int from = get_LSB(knights);
		clear_bit(knights, from);
		if (KNIGHT_MOVES[from] & target) return true;
	}

	// King
	u64 king = byWhite ? board.whiteKing : board.blackKing;
	if (KING_MOVES[get_LSB(king)] & target) return true;

	// Horizontally/Vertically sliding pieces (rook/queen)
	u64 hvSlidingPieces = byWhite ? board.whiteRooks | board.whiteQueens : board.blackRooks | board.blackQueens;

	// Up
	for (int s = sq + 8; s < 64; s += 8)
	{
		if (occupied & (1ULL << s))
		{
			if (hvSlidingPieces & (1ULL << s)) return true;
			break;
		}
	}

	// Down
	for (int s = sq - 8; s >= 0; s -= 8)
	{
		if (occupied & (1ULL << s))
		{
			if (hvSlidingPieces & (1ULL << s)) return true;
			break;
		}
	}

	// Right
	for (int s = sq + 1; s % 8 != 7; s += 1)
	{
		if (occupied & (1ULL << s))
		{
			if (hvSlidingPieces & (1ULL << s)) return true;
			break;
		}
	}

	// Left
	for (int s = sq - 1; s % 8 != 0; s -= 1)
	{
		if (occupied & (1ULL << s))
		{
			if (hvSlidingPieces & (1ULL << s)) return true;
			break;
		}
	}

	// Diagonally sliding pieces (bishop/queen)
	u64 dSlidingPieces = byWhite ? board.whiteBishops | board.whiteQueens : board.blackBishops| board.blackQueens;

	// Up-Right
	for (int s = sq + 9; s < 64 && s % 8 != 7; s += 9)
	{
		if (occupied & (1ULL << s))
		{
			if (dSlidingPieces & (1ULL << s)) return true;
			break;
		}
	}

	// Up-Left
	for (int s = sq + 7; s < 64 && s % 8 != 0; s += 7)
	{
		if (occupied & (1ULL << s))
		{
			if (dSlidingPieces & (1ULL << s)) return true;
			break;
		}
	}

	// Down-Right
	for (int s = sq - 9; s >= 0 && s % 8 != 7; s -= 9)
	{
		if (occupied & (1ULL << s))
		{
			if (dSlidingPieces & (1ULL << s)) return true;
			break;
		}
	}

	// Down-Left
	for (int s = sq - 7; s >= 0 && s % 8 != 0; s -= 7)
	{
		if (occupied & (1ULL << s))
		{
			if (dSlidingPieces & (1ULL << s)) return true;
			break;
		}
	}

	return false;
}
