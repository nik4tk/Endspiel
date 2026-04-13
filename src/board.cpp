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
	whitePawns = whiteKnights = whiteBishops = whiteRooks = whiteQueens = whiteKing = 0ULL;
	blackPawns = blackKnights = blackBishops = blackRooks = blackQueens = blackKing = 0ULL;

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
	set_bit(blackKnights, B8);
	set_bit(blackKnights, G8);
	set_bit(blackBishops, C8);
	set_bit(blackBishops, F8);
	set_bit(blackRooks, A8);
	set_bit(blackRooks, H8);
	set_bit(blackQueens, D8);
	set_bit(blackKing, E8);
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

void Board::makeMove(const Move& m)
{
    u64 fromBB = 1ULL << m.from;
    u64 toBB = 1ULL << m.to;

    // Move the piece
    switch (m.piece)
    {
    case WP: whitePawns ^= fromBB; whitePawns |= toBB; break;
    case WN: whiteKnights ^= fromBB; whiteKnights |= toBB; break;
    case WB: whiteBishops ^= fromBB; whiteBishops |= toBB; break;
    case WR: whiteRooks ^= fromBB; whiteRooks |= toBB; break;
    case WQ: whiteQueens ^= fromBB; whiteQueens |= toBB; break;
    case WK: whiteKing ^= fromBB; whiteKing |= toBB; break;

    case BP: blackPawns ^= fromBB; blackPawns |= toBB; break;
    case BN: blackKnights ^= fromBB; blackKnights |= toBB; break;
    case BB: blackBishops ^= fromBB; blackBishops |= toBB; break;
    case BR: blackRooks ^= fromBB; blackRooks |= toBB; break;
    case BQ: blackQueens ^= fromBB; blackQueens |= toBB; break;
    case BK: blackKing ^= fromBB; blackKing |= toBB; break;
    }

    // Capture handling
    if (m.capturePiece != NO_PIECE)
    {
        switch (m.capturePiece)
        {
        case WP: whitePawns &= ~toBB; break;
        case WN: whiteKnights &= ~toBB; break;
        case WB: whiteBishops &= ~toBB; break;
        case WR: whiteRooks &= ~toBB; break;
        case WQ: whiteQueens &= ~toBB; break;
        case WK: whiteKing &= ~toBB; break;

        case BP: blackPawns &= ~toBB; break;
        case BN: blackKnights &= ~toBB; break;
        case BB: blackBishops &= ~toBB; break;
        case BR: blackRooks &= ~toBB; break;
        case BQ: blackQueens &= ~toBB; break;
        case BK: blackKing &= ~toBB; break;
        }
    }

    // Special moves

    // En passant
    if (m.flags & (1 << 2))
    {
        if (m.piece == WP)
        {
            u64 capSq = 1ULL << (m.to - 8);
            blackPawns &= ~capSq;
        }
        else if (m.piece == BP)
        {
            u64 capSq = 1ULL << (m.to + 8);
            whitePawns &= ~capSq;
        }
    }

    // Castling
    if (m.flags & (1 << 3))
    {
        // White
        if (m.piece == WK)
        {
            // King side
            if (m.to == 6) // g1
            {
                whiteRooks &= ~(1ULL << 7); // h1
                whiteRooks |= (1ULL << 5); // f1
            }
            // Queen side
            else if (m.to == 2) // c1
            {
                whiteRooks &= ~(1ULL << 0); // a1
                whiteRooks |= (1ULL << 3); // d1
            }
        }

        // Black
        if (m.piece == BK)
        {
            if (m.to == 62) // g8
            {
                blackRooks &= ~(1ULL << 63);
                blackRooks |= (1ULL << 61);
            }
            else if (m.to == 58) // c8
            {
                blackRooks &= ~(1ULL << 56);
                blackRooks |= (1ULL << 59);
            }
        }
    }

    // Promotion
    if (m.flags & (1 << 4))
    {
        // Remove pawn
        if (m.piece == WP) whitePawns &= ~toBB;
        if (m.piece == BP) blackPawns &= ~toBB;

        // Promote to queen (for now)
        if (m.piece == WP) whiteQueens |= toBB;
        if (m.piece == BP) blackQueens |= toBB;
    }
}

void Board::undoMove(const Move& m)
{
}
