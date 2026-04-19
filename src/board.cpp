#include "board.h"
#include <iostream>

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

int Board::getPieceOnSquare(int sq) const
{
    u64 bb = 1ULL << sq;
    if (whitePawns & bb) return WP;
    if (whiteKnights & bb) return WN;
    if (whiteBishops & bb) return WB;
    if (whiteRooks & bb) return WR;
    if (whiteQueens & bb) return WQ;
    if (whiteKing & bb) return WK;
    if (blackPawns & bb) return BP;
    if (blackKnights & bb) return BN;
    if (blackBishops & bb) return BB;
    if (blackRooks & bb) return BR;
    if (blackQueens & bb) return BQ;
    if (blackKing & bb) return BK;
    return NO_PIECE;
}

void Board::init()
{
    whitePawns = whiteKnights = whiteBishops = whiteRooks = whiteQueens = whiteKing = 0ULL;
    blackPawns = blackKnights = blackBishops = blackRooks = blackQueens = blackKing = 0ULL;

    for (int sq = 8; sq <= 15; sq++) set_bit(whitePawns, sq);
    set_bit(whiteKnights, B1); set_bit(whiteKnights, G1);
    set_bit(whiteBishops, C1); set_bit(whiteBishops, F1);
    set_bit(whiteRooks, A1); set_bit(whiteRooks, H1);
    set_bit(whiteQueens, D1);
    set_bit(whiteKing, E1);

    for (int sq = 48; sq <= 55; sq++) set_bit(blackPawns, sq);
    set_bit(blackKnights, B8); set_bit(blackKnights, G8);
    set_bit(blackBishops, C8); set_bit(blackBishops, F8);
    set_bit(blackRooks, A8); set_bit(blackRooks, H8);
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
            u64 bit = 1ULL << square;
            char piece = '.';
            if (whitePawns & bit) piece = 'P';
            else if (whiteKnights & bit) piece = 'N';
            else if (whiteBishops & bit) piece = 'B';
            else if (whiteRooks & bit) piece = 'R';
            else if (whiteQueens & bit) piece = 'Q';
            else if (whiteKing & bit) piece = 'K';
            else if (blackPawns & bit) piece = 'p';
            else if (blackKnights & bit) piece = 'n';
            else if (blackBishops & bit) piece = 'b';
            else if (blackRooks & bit) piece = 'r';
            else if (blackQueens & bit) piece = 'q';
            else if (blackKing & bit) piece = 'k';
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
    case WP:
        whitePawns ^= fromBB;
        whitePawns |= toBB;
        break;
    case WN:
        whiteKnights ^= fromBB;
        whiteKnights |= toBB;
        break;
    case WB: whiteBishops ^= fromBB;
        whiteBishops |= toBB;
        break;
    case WR:
        whiteRooks ^= fromBB;
        whiteRooks |= toBB;
        break;
    case WQ:
        whiteQueens ^= fromBB;
        whiteQueens |= toBB;
        break;
    case WK:
        whiteKing ^= fromBB;
        whiteKing |= toBB;
        break;

    case BP:
        blackPawns ^= fromBB;
        blackPawns |= toBB;
        break;
    case BN:
        blackKnights ^= fromBB;
        blackKnights |= toBB;
        break;
    case BB:
        blackBishops ^= fromBB;
        blackBishops |= toBB;
        break;
    case BR:
        blackRooks ^= fromBB;
        blackRooks |= toBB;
        break;
    case BQ:
        blackQueens ^= fromBB;
        blackQueens |= toBB;
        break;
    case BK:
        blackKing ^= fromBB;
        blackKing |= toBB;
        break;
    }

    // Remove captured piece
    if (m.capturePiece != NO_PIECE)
    {
        switch (m.capturePiece)
        {
        case WP:
            whitePawns &= ~toBB;
            break;
        case WN:
            whiteKnights &= ~toBB;
            break;
        case WB:
            whiteBishops &= ~toBB;
            break;
        case WR:
            whiteRooks &= ~toBB;
            break;
        case WQ:
            whiteQueens &= ~toBB;
            break;
        case WK:
            whiteKing &= ~toBB;
            break;

        case BP:
            blackPawns &= ~toBB;
            break;
        case BN: 
            blackKnights &= ~toBB;
            break;
        case BB:
            blackBishops &= ~toBB;
            break;
        case BR:
            blackRooks &= ~toBB;
            break;
        case BQ:
            blackQueens &= ~toBB;
            break;
        case BK:
            blackKing &= ~toBB;
            break;
        }
    }

    // En passant
    if (m.flags & EN_PASSANT)
    {
        if (m.piece == WP) blackPawns &= ~(1ULL << (m.to - 8));
        else whitePawns &= ~(1ULL << (m.to + 8));
    }

    // Castling
    if (m.flags & CASTLING)
    {
        if (m.piece == WK)
        {
            if (m.to == 6) { whiteRooks &= ~(1ULL << 7); whiteRooks |= (1ULL << 5); }
            else { whiteRooks &= ~(1ULL << 0); whiteRooks |= (1ULL << 3); }
        }
        else
        {
            if (m.to == 62) { blackRooks &= ~(1ULL << 63); blackRooks |= (1ULL << 61); }
            else { blackRooks &= ~(1ULL << 56); blackRooks |= (1ULL << 59); }
        }
    }

    // Promotion
    if (m.flags & PROMOTION)
    {
        if (m.piece == WP) { whitePawns &= ~toBB; whiteQueens |= toBB; }
        else { blackPawns &= ~toBB; blackQueens |= toBB; }
    }
}

void Board::undoMove(const Move& m)
{
    u64 fromBB = 1ULL << m.from;
    u64 toBB = 1ULL << m.to;

    // Undo promotion first: remove promoted piece, restore pawn at from
    if (m.flags & PROMOTION)
    {
        if (m.piece == WP) { whiteQueens &= ~toBB; whitePawns |= fromBB; }
        else { blackQueens &= ~toBB; blackPawns |= fromBB; }
    }
    else
    {
        // Move piece back from -> to becomes to -> from
        switch (m.piece)
        {
        case WP:
            whitePawns ^= toBB;
            whitePawns |= fromBB;
            break;
        case WN:
            whiteKnights ^= toBB;
            whiteKnights |= fromBB;
            break;
        case WB:
            whiteBishops ^= toBB;
            whiteBishops |= fromBB;
            break;
        case WR:
            whiteRooks ^= toBB;
            whiteRooks |= fromBB;
            break;
        case WQ:
            whiteQueens ^= toBB;
            whiteQueens |= fromBB;
            break;
        case WK:
            whiteKing ^= toBB;
            whiteKing |= fromBB;
            break;
        case BP:
            blackPawns ^= toBB;
            blackPawns |= fromBB;
            break;
        case BN:
            blackKnights ^= toBB;
            blackKnights |= fromBB;
            break;
        case BB:
            blackBishops ^= toBB;
            blackBishops |= fromBB;
            break;
        case BR:
            blackRooks ^= toBB;
            blackRooks |= fromBB;
            break;
        case BQ:
            blackQueens ^= toBB;
            blackQueens |= fromBB;
            break;
        case BK:
            blackKing ^= toBB;
            blackKing |= fromBB;
            break;
        }
    }

    // Restore captured piece at 'to'
    if (m.capturePiece != NO_PIECE)
    {
        switch (m.capturePiece)
        {
        case WP:
            whitePawns |= toBB;
            break;
        case WN:
            whiteKnights |= toBB;
            break;
        case WB:
            whiteBishops |= toBB;
            break;
        case WR:
            whiteRooks |= toBB;
            break;
        case WQ:
            whiteQueens |= toBB;
            break;
        case WK:
            whiteKing |= toBB;
            break;
        case BP:
            blackPawns |= toBB;
            break;
        case BN:
            blackKnights |= toBB;
            break;
        case BB:
            blackBishops |= toBB;
            break;
        case BR:
            blackRooks |= toBB;
            break;
        case BQ:
            blackQueens |= toBB;
            break;
        case BK:
            blackKing |= toBB;
            break;
        }
    }

    // Undo en passant: restore the captured pawn at its real square
    if (m.flags & EN_PASSANT)
    {
        if (m.piece == WP) blackPawns |= (1ULL << (m.to - 8));
        else whitePawns |= (1ULL << (m.to + 8));
    }

    // Undo castling: move rook back
    if (m.flags & CASTLING)
    {
        if (m.piece == WK)
        {
            if (m.to == 6)
            {
                whiteRooks |= (1ULL << 7);
                whiteRooks &= ~(1ULL << 5);
            }
            else
            {
                whiteRooks |= (1ULL << 0);
                whiteRooks &= ~(1ULL << 3);
            }
        }
        else
        {
            if (m.to == 62)
            {
                blackRooks |= (1ULL << 63);
                blackRooks &= ~(1ULL << 61);
            }
            else
            {
                blackRooks |= (1ULL << 56);
                blackRooks &= ~(1ULL << 59);
            }
        }
    }
}