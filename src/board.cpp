#include "board.h"
#include <iostream>

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

    for (int sq = A2; sq <= H2; sq++) set_bit(whitePawns, sq);
    set_bit(whiteKnights, B1); set_bit(whiteKnights, G1);
    set_bit(whiteBishops, C1); set_bit(whiteBishops, F1);
    set_bit(whiteRooks, A1); set_bit(whiteRooks, H1);
    set_bit(whiteQueens, D1);
    set_bit(whiteKing, E1);

    for (int sq = A7; sq <= H7; sq++) set_bit(blackPawns, sq);
    set_bit(blackKnights, B8); set_bit(blackKnights, G8);
    set_bit(blackBishops, C8); set_bit(blackBishops, F8);
    set_bit(blackRooks, A8); set_bit(blackRooks, H8);
    set_bit(blackQueens, D8);
    set_bit(blackKing, E8);

    castleWK = castleWQ = castleBK = castleBQ = true;
    enPassantSq = -1;
    historyCount = 0;
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
    case WP: whitePawns ^= fromBB; whitePawns |= toBB; break;
    case WN: whiteKnights ^= fromBB; whiteKnights |= toBB; break;
    case WB: whiteBishops ^= fromBB; whiteBishops |= toBB; break;
    case WR: whiteRooks ^= fromBB; whiteRooks |= toBB; break;
    case WQ: whiteQueens ^= fromBB; whiteQueens |= toBB; break;
    case WK: whiteKing ^= fromBB;  whiteKing |= toBB; break;

    case BP: blackPawns ^= fromBB; blackPawns |= toBB; break;
    case BN: blackKnights ^= fromBB; blackKnights |= toBB; break;
    case BB: blackBishops ^= fromBB; blackBishops |= toBB; break;
    case BR: blackRooks ^= fromBB; blackRooks |= toBB; break;
    case BQ: blackQueens ^= fromBB; blackQueens |= toBB; break;
    case BK: blackKing ^= fromBB; blackKing |= toBB; break;
    }

    // Remove captured piece
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
            if (m.to == G1) { whiteRooks &= ~(1ULL << H1); whiteRooks |= (1ULL << F1); }
            else { whiteRooks &= ~(1ULL << A1); whiteRooks |= (1ULL << D1); }
        }
        else
        {
            if (m.to == G8) { blackRooks &= ~(1ULL << H8); blackRooks |= (1ULL << F8); }
            else { blackRooks &= ~(1ULL << A8); blackRooks |= (1ULL << D8); }
        }
    }

    // Promotion
    if (m.flags & PROMOTION)
    {
        if (m.piece == WP)
        {
            whitePawns &= ~toBB;
            switch (m.promotionPiece)
            {
            case WQ: whiteQueens |= toBB; break;
            case WR: whiteRooks |= toBB; break;
            case WB: whiteBishops |= toBB; break;
            case WN: whiteKnights |= toBB; break;
            }
        }
        else
        {
            blackPawns &= ~toBB;
            switch (m.promotionPiece)
            {
            case BQ: blackQueens|= toBB; break;
            case BR: blackRooks |= toBB; break;
            case BB: blackBishops |= toBB; break;
            case BN: blackKnights |= toBB; break;
            }
        }
    }

    // Update casling rights
    if (m.piece == WK) { castleWK = false; castleWQ = false; }
    if (m.piece == BK) { castleBK = false; castleBQ = false; }

    if (m.from == A1 || m.to == A1) castleWQ = false;
    if (m.from == H1 || m.to == H1) castleWK = false;
    if (m.from == A8 || m.to == A8) castleBQ = false;
    if (m.from == H8 || m.to == H8) castleBK = false;

    if (m.flags & DOUBLE_PAWN) enPassantSq = (m.piece == WP) ? m.to - 8 : m.to + 8;
    else enPassantSq = -1;
}

void Board::undoMove(const Move& m)
{
    u64 fromBB = 1ULL << m.from;
    u64 toBB = 1ULL << m.to;

    UndoState& u = history[historyCount--];
    castleWK = u.castleWK;
    castleWQ = u.castleWQ;
    castleBK = u.castleBK;
    castleBQ = u.castleBQ;
    enPassantSq = u.enPassantSq;

    // Undo promotion first: remove promoted piece, restore pawn at from
    if (m.flags & PROMOTION)
    {
        switch (m.promotionPiece)
        {
        case WQ: whiteQueens |= toBB; break;
        case WR: whiteRooks |= toBB; break;
        case WB: whiteBishops |= toBB; break;
        case WN: whiteKnights |= toBB; break;

        case BQ: blackQueens |= toBB; break;
        case BR: blackRooks |= toBB; break;
        case BB: blackBishops |= toBB; break;
        case BN: blackKnights |= toBB; break;
        }
        if (m.piece == WP) whitePawns |= fromBB;
        else blackPawns |= fromBB;
    }
    else
    {
        // Move piece back from -> to becomes to -> from
        switch (m.piece)
        {
        case WP: whitePawns ^= toBB; whitePawns |= fromBB; break;
        case WN: whiteKnights ^= toBB; whiteKnights |= fromBB; break;
        case WB: whiteBishops ^= toBB; whiteBishops |= fromBB; break;
        case WR: whiteRooks ^= toBB; whiteRooks |= fromBB; break;
        case WQ: whiteQueens ^= toBB; whiteQueens |= fromBB; break;
        case WK: whiteKing ^= toBB; whiteKing |= fromBB; break;
        case BP: blackPawns ^= toBB; blackPawns |= fromBB; break;
        case BN: blackKnights ^= toBB; blackKnights |= fromBB; break;
        case BB: blackBishops ^= toBB; blackBishops |= fromBB; break;
        case BR: blackRooks ^= toBB; blackRooks |= fromBB; break;
        case BQ: blackQueens ^= toBB; blackQueens |= fromBB; break;
        case BK: blackKing ^= toBB; blackKing |= fromBB; break;
        }
    }

    // Restore captured piece at 'to'
    if (m.capturePiece != NO_PIECE)
    {
        switch (m.capturePiece)
        {
        case WP: whitePawns |= toBB; break;
        case WN: whiteKnights |= toBB; break;
        case WB: whiteBishops |= toBB; break;
        case WR: whiteRooks |= toBB; break;
        case WQ: whiteQueens |= toBB; break;
        case WK: whiteKing |= toBB; break;
        case BP: blackPawns |= toBB; break;
        case BN: blackKnights |= toBB; break;
        case BB: blackBishops |= toBB; break;
        case BR: blackRooks |= toBB; break;
        case BQ: blackQueens |= toBB; break;
        case BK: blackKing |= toBB; break;
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
            if (m.to == G1) { whiteRooks |= (1ULL << H1); whiteRooks &= ~(1ULL << F1); }
            else { whiteRooks |= (1ULL << A1); whiteRooks &= ~(1ULL << D1); }
        }
        else
        {
            if (m.to == G8) { blackRooks |= (1ULL << H8); blackRooks &= ~(1ULL << F8); }
            else { blackRooks |= (1ULL << A8); blackRooks &= ~(1ULL << D8); }
        }
    }
}