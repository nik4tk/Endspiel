#pragma once
#include <cstdint>
#include <bit>

#include "move.h"

using u64 = uint64_t;

#define set_bit(board, sq) ((board) |= (1ULL << sq))
#define clear_bit(board, sq) ((board) &= ~(1ULL << sq))

inline int get_LSB(u64 board) { return std::countr_zero(board); }

enum Square {
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8
};

// Saved before each makeMove so undoMove can restore exactly
struct UndoState {
	bool castleWK, castleWQ, castleBK, castleBQ;
	int enPassantSq;
};

class Board
{
public:
	u64 whitePawns, whiteKnights, whiteBishops, whiteRooks, whiteQueens, whiteKing;
	u64 blackPawns, blackKnights, blackBishops, blackRooks, blackQueens, blackKing;

	// Game state
	bool castleWK, castleWQ, castleBK, castleBQ;
	int enPassantSq; // -1 if none, otherwise the square a pawn can capture to

	// History for undoMove
	UndoState history[512];
	int historyCount;

	// helper-fnct
	u64 whitePieces() const { return whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing; }
	u64 blackPieces() const { return blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing; }
	u64 allPieces() const { return whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing | blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing; }

	int getPieceOnSquare(int sq) const;

	void init();
	void print();
	void makeMove(const Move& m);
	void undoMove(const Move& m);
};
