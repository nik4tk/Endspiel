#pragma once
#include "board.h"
#include "move.h"

inline u64 RANK_2 = 0ULL;
inline u64 RANK_7 = 0ULL;
inline u64 FILE_A = 0ULL;
inline u64 FILE_H = 0ULL;
inline u64 FILES_AB = 0ULL;
inline u64 FILES_GH = 0ULL;

extern u64 KNIGHT_MOVES[64];
extern u64 KING_MOVES[64];

struct MoveList {
	Move moves[256]{}; // 218, because a normal chess-position only has ~218 legal moves
	int count = 0;

	Move* begin() { return moves; }
	Move* end() { return moves + count; }
	const Move* begin() const { return moves; }
	const Move* end() const { return moves + count;	}

	void push(Move m) { if (count < 256) moves[count++] = m; }
};

void startUp();

// General moves
static void promotions(MoveList& moves, int from, int to, int pawn, int captured);

void generateWhitePawnMoves(const Board& board, MoveList& moves);
void generateBlackPawnMoves(const Board& board, MoveList& moves);

void initKnightMoves();
void generateWhiteKnightMoves(const Board& board, MoveList& moves);
void generateBlackKnightMoves(const Board& board, MoveList& moves);

void initKingMoves();
void generateWhiteKingMoves(const Board& board, MoveList& moves);
void generateBlackKingMoves(const Board& board, MoveList& moves);

static void generateSlidingMoves(const Board& board, MoveList& moves, u64 pieces, int pieceType, u64 ownPieces, u64 enemyPieces, const int* directions, int directionsAmount);

void generateWhiteBishopMoves(const Board& board, MoveList& moves);
void generateBlackBishopMoves(const Board& board, MoveList& moves);

void generateWhiteRookMoves(const Board& board, MoveList& moves);
void generateBlackRookMoves(const Board& board, MoveList& moves);

void generateWhiteQueenMoves(const Board& board, MoveList& moves);
void generateBlackQueenMoves(const Board& board, MoveList& moves);

// Generate all pseudo/legal-moves
MoveList generatePseudoMoves(const Board& board, bool whiteToMove);
MoveList generateWhiteLegalMoves(Board& board);
MoveList generateBlackLegalMoves(Board& board);

// King-safety
bool isSquareAttacked(const Board& board, int sq, bool byWhite);

bool isCheckmate(Board& board, bool whiteToMove);
bool isStalemate(Board& board, bool whiteToMove);
