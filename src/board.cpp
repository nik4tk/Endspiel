#include "board.h"
#include <iostream>

// enum assigns int values from 0; e.g. A1 = 0, A2 = 1, ...
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

// ULL = unsigned long long; << is a bit-shift operator, shifts bits of 1ULL to the left by e.g. A1 positions
void Board::init()
{
	u64 wP = 0ULL;
	for (int sq = 8; sq <= 15; sq++)
	{
		set_bit(wP, sq);
	}
	whitePawns = wP;
	set_bit(whiteKnights, B1);
	set_bit(whiteKnights, G1);
	set_bit(whiteBishops, C1);
	set_bit(whiteBishops, F1);
	set_bit(whiteRooks, A1);
	set_bit(whiteRooks, H1);
	set_bit(whiteQueens, D1);
	set_bit(whiteKing, E1);

	u64 bP = 0ULL;
	for (int sq = 48; sq <= 55; sq++)
	{
		set_bit(bP, sq);
	}
	blackPawns = bP;
	set_bit(whiteKnights, B8);
	set_bit(whiteKnights, G8);
	set_bit(whiteBishops, C8);
	set_bit(whiteBishops, F8);
	set_bit(whiteRooks, A8);
	set_bit(whiteRooks, H8);
	set_bit(whiteQueens, D8);
	set_bit(whiteKing, E8);
}

void Board::print()
{
	for (int rank = 7; rank >= 0; rank--)
	{
		std::cout << rank + 1 << " ";
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;
			u64 squarePtr = 1ULL << square;
			char piece = '.';

			// check white pieces
			if (whitePawns & squarePtr) piece = 'P';
			else if (whiteKnights & squarePtr) piece = 'N';
			else if (whiteBishops & squarePtr) piece = 'B';
			else if (whiteRooks & squarePtr) piece = 'R';
			else if (whiteQueens & squarePtr) piece = 'Q';
			else if (whiteKing & squarePtr) piece = 'K';

			// check black pieces
			if (blackPawns & squarePtr) piece = 'p';
			else if (blackKnights & squarePtr) piece = 'n';
			else if (blackBishops & squarePtr) piece = 'b';
			else if (blackRooks & squarePtr) piece = 'r';
			else if (blackQueens & squarePtr) piece = 'q';
			else if (blackKing & squarePtr) piece = 'k';

			std::cout << piece << " ";
		}
		std::cout << "\n";
	}
	std::cout << "  a b c d e f g h\n";
}
