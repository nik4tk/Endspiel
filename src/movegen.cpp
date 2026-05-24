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

    for (int sq = 0; sq <= 56; sq += 8)
    {
        set_bit(FILE_A, sq);
        set_bit(FILES_AB, sq);
    }
    for (int sq = 1; sq <= 49; sq += 8)
    {
        set_bit(FILES_AB, sq);
    }

    for (int sq = 7; sq <= 63; sq += 8)
    {
        set_bit(FILE_H, sq);
        set_bit(FILES_GH, sq);
    }
    for (int sq = 6; sq < 62; sq += 8)
    {
        set_bit(FILES_GH, sq);
    }

    initKnightMoves();
    initKingMoves();
}

void promotions(MoveList& moves, int from, int to, int pawn, int captured)
{
    int flag = (captured != NO_PIECE) ? (PROMOTION | CAPTURE) : PROMOTION;
    bool white = (pawn == WP);
    moves.push({ from, to, pawn, captured, flag, white ? WQ : BQ });
    moves.push({ from, to, pawn, captured, flag, white ? WR : BR });
    moves.push({ from, to, pawn, captured, flag, white ? WB : BB });
    moves.push({ from, to, pawn, captured, flag, white ? WN: BN });
}

void generateWhitePawnMoves(const Board& board, MoveList& moves)
{
    u64 emptySquares = ~board.allPieces();
    u64 pawns = board.whitePawns;

    // Single push
    u64 singlePush = (pawns << 8) & emptySquares;
    while (singlePush)
    {
        int to = get_LSB(singlePush);
        clear_bit(singlePush, to);
        if (to >= A8) promotions(moves, to - 8, to, WP, NO_PIECE);
        else moves.push({ to - 8, to, WP, NO_PIECE, QUIET });
    }

    // Double push
    u64 doublePush = ((pawns & RANK_2) << 8) & emptySquares;
    doublePush = (doublePush << 8) & emptySquares;
    while (doublePush)
    {
        int to = get_LSB(doublePush);
        clear_bit(doublePush, to);
        moves.push({ to - 16, to, WP, NO_PIECE, DOUBLE_PAWN });
    }

    // Captures
    u64 capturesLeft = ((pawns << 7) & ~FILE_H) & board.blackPieces();
    while (capturesLeft)
    {
        int to = get_LSB(capturesLeft);
        clear_bit(capturesLeft, to);
        int captured = board.getPieceOnSquare(to);
        if (to >= A8) promotions(moves, to - 7, to, WP, captured);
        else moves.push({ to - 7, to , WP, captured, CAPTURE });
    }

    u64 capturesRight = ((pawns << 9) & ~FILE_A) & board.blackPieces();
    while (capturesRight)
    {
        int to = get_LSB(capturesRight);
        clear_bit(capturesRight, to);
        int captured = board.getPieceOnSquare(to);
        if (to >= A8) promotions(moves, to - 9, to, WP, captured);
        else moves.push({ to - 9, to , WP, captured, CAPTURE });
    }

    // En passant
    if (board.enPassantSq != -1)
    {
        u64 epBB = 1ULL << board.enPassantSq;
        u64 epLeft = ((pawns << 7 & ~FILE_H) & epBB);
        u64 epRight = ((pawns << 9 & ~FILE_A) & epBB);
        if (epLeft) moves.push({ board.enPassantSq - 7, board.enPassantSq, WP, BP, EN_PASSANT });
        if (epRight) moves.push({ board.enPassantSq - 9, board.enPassantSq, WP, BP, EN_PASSANT });
    }
}

void generateBlackPawnMoves(const Board& board, MoveList& moves)
{
    u64 emptySquares = ~board.allPieces();
    u64 pawns = board.blackPawns;

    // Single push
    u64 singlePush = (pawns >> 8) & emptySquares;
    while (singlePush)
    {
        int to = get_LSB(singlePush);
        clear_bit(singlePush, to);
        if (to <= H1) promotions(moves, to + 8, to, BP, NO_PIECE);
        else
            moves.push({ to + 8, to, BP, NO_PIECE, QUIET });
    }

    // Double push
    u64 doublePush = ((pawns & RANK_7) >> 8) & emptySquares;
    doublePush = (doublePush >> 8) & emptySquares;
    while (doublePush)
    {
        int to = get_LSB(doublePush);
        clear_bit(doublePush, to);
        moves.push({ to + 16, to, BP, NO_PIECE, DOUBLE_PAWN });
    }

    // Captures
    u64 capturesLeft = ((pawns >> 9) & ~FILE_A) & board.whitePieces();
    while (capturesLeft)
    {
        int to = get_LSB(capturesLeft);
        clear_bit(capturesLeft, to);
        int captured = board.getPieceOnSquare(to);
        if (to <= H1) promotions(moves, to + 9, to, BP, captured);
        else moves.push({ to + 9, to, BP, captured, CAPTURE });
    }

    u64 capturesRight = ((pawns >> 7) & ~FILE_H) & board.whitePieces();
    while (capturesRight)
    {
        int to = get_LSB(capturesRight);
        clear_bit(capturesRight, to);
        int captured = board.getPieceOnSquare(to);
        if (to <= H1) promotions(moves, to + 7, to, BP, captured);
        else moves.push({ to + 7, to, BP, captured, CAPTURE });
    }

    // En passant
    if (board.enPassantSq != -1)
    {
        u64 epBB = 1ULL << board.enPassantSq;
        u64 epLeft = ((pawns >> 9 & ~FILE_A) & epBB);
        u64 epRight = ((pawns >> 7 & ~FILE_H) & epBB);
        if (epLeft) moves.push({ board.enPassantSq + 9, board.enPassantSq, BP, WP, EN_PASSANT });
        if (epRight) moves.push({ board.enPassantSq + 7, board.enPassantSq, BP, WP, EN_PASSANT });
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
            int captured = board.getPieceOnSquare(to);
            moves.push({ from, to, WN, captured, captured == NO_PIECE ? QUIET : CAPTURE });
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
            int captured = board.getPieceOnSquare(to);
            moves.push({ from, to, BN, captured, captured == NO_PIECE ? QUIET : CAPTURE });
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
    int from = get_LSB(board.whiteKing);
    u64 captures = KING_MOVES[from] & ~board.whitePieces();
    while (captures)
    {
        int to = get_LSB(captures);
        clear_bit(captures, to);
        int captured = board.getPieceOnSquare(to);
        moves.push({ from, to, WK, captured, captured == NO_PIECE ? QUIET : CAPTURE });
    }

    if (board.castleWK)
    {
        u64 occ = board.allPieces();
        if (!(occ & ((1ULL << F1) | (1ULL << G1))))
            if (!isSquareAttacked(board, E1, false) && !isSquareAttacked(board, F1, false) && !isSquareAttacked(board, G1, false))
            {
                moves.push({ E1, G1, WK, NO_PIECE, CASTLING });
            }
    }
    if (board.castleWQ)
    {
        u64 occ = board.allPieces();
        if (!(occ & ((1ULL << B1) | (1ULL << C1) | (1ULL << D1))))
            if (!isSquareAttacked(board, E1, false) && !isSquareAttacked(board, D1, false) && !isSquareAttacked(board, C1, false))
            {
                moves.push({ E1, C1, WK, NO_PIECE, CASTLING });
            }
    }
}

void generateBlackKingMoves(const Board& board, MoveList& moves)
{
    int from = get_LSB(board.blackKing);
    u64 captures = KING_MOVES[from] & ~board.blackPieces();
    while (captures)
    {
        int to = get_LSB(captures);
        clear_bit(captures, to);
        int captured = board.getPieceOnSquare(to);
        moves.push({ from, to, BK, captured, captured != NO_PIECE ? CAPTURE : QUIET });
    }

    if (board.castleBK)
    {
        u64 occ = board.allPieces();
        if (!(occ & ((1ULL << F8) | (1ULL << G8))))
            if (!isSquareAttacked(board, E8, true) && !isSquareAttacked(board, F8, true) && !isSquareAttacked(board, G8, true))
            {
                moves.push({ E8, G8, BK, NO_PIECE, CASTLING });
            }
    }
    if (board.castleBQ)
    {
        u64 occ = board.allPieces();
        if (!(occ & ((1ULL << B8) | (1ULL << C8) | (1ULL << D8))))
            if (!isSquareAttacked(board, E8, true) && !isSquareAttacked(board, D8, true) && !isSquareAttacked(board, C8, true))
            {
                moves.push({ E8, C8, BK, NO_PIECE, CASTLING });
            }
    }
}

void generateSlidingMoves(const Board& board, MoveList& moves, u64 pieces, int pieceType, u64 ownPieces, u64 enemyPieces, const int* directions, int directionsAmount)
{
    while (pieces)
    {
        int from = get_LSB(pieces);
        clear_bit(pieces, from);
        for (int d = 0; d < directionsAmount; d++)
        {
            int sq = from;
            while (true)
            {
                int prev = sq;
                sq += directions[d];
                if (sq < 0 || sq >= 64) break;

                if (abs(directions[d]) == 1 && (sq / 8) != (prev / 8)) break;
                if (abs(directions[d]) == 7 && abs((sq % 8) - (prev % 8)) != 1) break;
                if (abs(directions[d]) == 9 && abs((sq % 8) - (prev % 8)) != 1) break;

                if (ownPieces & (1ULL << sq)) break;
                int captured = (enemyPieces & (1ULL << sq) ? board.getPieceOnSquare(sq) : NO_PIECE);
                moves.push({ from, sq, pieceType, captured, captured != NO_PIECE ? CAPTURE : QUIET });
                if (captured != NO_PIECE) break;
            }
        }
    }
}

void generateWhiteBishopMoves(const Board& board, MoveList& moves)
{
    const int dirs[] = { 9, 7, -7, -9 };
    generateSlidingMoves(board, moves, board.whiteBishops, WB, board.whitePieces(), board.blackPieces(), dirs, 4);
}

void generateBlackBishopMoves(const Board& board, MoveList& moves)
{
    const int dirs[] = { 9, 7, -7, -9 };
    generateSlidingMoves(board, moves, board.blackBishops, BB, board.blackPieces(), board.whitePieces(), dirs, 4);
}

void generateWhiteRookMoves(const Board& board, MoveList& moves)
{
    const int dirs[] = { 8, 1, -1, -8 };
    generateSlidingMoves(board, moves, board.whiteRooks, WR, board.whitePieces(), board.blackPieces(), dirs, 4);
}

void generateBlackRookMoves(const Board& board, MoveList& moves)
{
    const int dirs[] = { 8, 1, -1, -8 };
    generateSlidingMoves(board, moves, board.blackRooks, BR, board.blackPieces(), board.whitePieces(), dirs, 4);
}

void generateWhiteQueenMoves(const Board& board, MoveList& moves)
{
    const int dirs[] = { 9, 7, -7, -9, 8, 1, -1, -8 };
    generateSlidingMoves(board, moves, board.whiteQueens, WQ, board.whitePieces(), board.blackPieces(), dirs, 8);
}

void generateBlackQueenMoves(const Board& board, MoveList& moves)
{
    const int dirs[] = { 9, 7, -7, -9, 8, 1, -1, -8 };
    generateSlidingMoves(board, moves, board.blackQueens, BQ, board.blackPieces(), board.whitePieces(), dirs, 8);
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

MoveList generateWhiteLegalMoves(Board& board)
{
    MoveList pseudo = generatePseudoMoves(board, true);
    MoveList legal;

    for (const Move& m : pseudo)
    {
        board.makeMove(m);

        int kingSq = get_LSB(board.whiteKing);

        if (!isSquareAttacked(board, kingSq, false)) legal.push(m); // Check if white king is attacked by black (byWhite = false)

        board.undoMove(m);
    }

    return legal;
}

MoveList generateBlackLegalMoves(Board& board)
{
    MoveList pseudo = generatePseudoMoves(board, false);
    MoveList legal;

    for (const Move& m : pseudo)
    {
        board.makeMove(m);

        int kingSq = get_LSB(board.blackKing);

        if (!isSquareAttacked(board, kingSq, true)) legal.push(m); // Check if black king is attacked by white (byWhite = true)

        board.undoMove(m);
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
    if (KNIGHT_MOVES[sq] & knights) return true;

    // King
    u64 king = byWhite ? board.whiteKing : board.blackKing;
    if (KING_MOVES[sq] & king) return true;

    // Horizontally/Vertically sliding pieces (rook/queen)
    u64 hvPieces = byWhite ? board.whiteRooks | board.whiteQueens : board.blackRooks | board.blackQueens;

    // Up
    for (int s = sq + 8; s < 64; s += 8)
    {
        if (occupied & (1ULL << s))
        {
            if (hvPieces & (1ULL << s)) return true;
            break;
        }
    }

    // Down
    for (int s = sq - 8; s >= 0; s -= 8)
    {
        if (occupied & (1ULL << s))
        {
            if (hvPieces & (1ULL << s)) return true;
            break;
        }
    }

    // Right
    for (int s = sq + 1; s % 8 != 0; s += 1)
    {
        if (occupied & (1ULL << s))
        {
            if (hvPieces & (1ULL << s)) return true;
            break;
        }
    }

    // Left
    for (int s = sq - 1; s % 8 != 7 && s >= 0; s -= 1)
    {
        if (occupied & (1ULL << s))
        {
            if (hvPieces & (1ULL << s)) return true;
            break;
        }
    }

    // Diagonally sliding pieces (bishop/queen)
    u64 dPieces = byWhite ? board.whiteBishops | board.whiteQueens : board.blackBishops | board.blackQueens;

    // Up-Right
    for (int s = sq + 9; s < 64 && s % 8 != 0; s += 9)
    {
        if (occupied & (1ULL << s))
        {
            if (dPieces & (1ULL << s)) return true;
            break;
        }
    }

    // Up-Left
    for (int s = sq + 7; s < 64 && s % 8 != 7; s += 7)
    {
        if (occupied & (1ULL << s))
        {
            if (dPieces & (1ULL << s)) return true;
            break;
        }
    }

    // Down-Right
    for (int s = sq - 7; s >= 0 && s % 8 != 0; s -= 7)
    {
        if (occupied & (1ULL << s))
        {
            if (dPieces & (1ULL << s)) return true;
            break;
        }
    }

    // Down-Left
    for (int s = sq - 9; s >= 0 && s % 8 != 7; s -= 9)
    {
        if (occupied & (1ULL << s))
        {
            if (dPieces & (1ULL << s)) return true;
            break;
        }
    }

    return false;
}

bool isCheckmate(Board& board, bool whiteToMove)
{
    MoveList moves = whiteToMove ? generateWhiteLegalMoves(board) : generateBlackLegalMoves(board);
    if (moves.count != 0) return false;
    int kingSq = get_LSB(whiteToMove ? board.whiteKing : board.blackKing);
    return isSquareAttacked(board, kingSq, !whiteToMove);
}

bool isStalemate(Board& board, bool whiteToMove)
{
    MoveList moves = whiteToMove ? generateWhiteLegalMoves(board) : generateBlackLegalMoves(board);
    if (moves.count != 0) return false;
    int kingSq = get_LSB(whiteToMove ? board.whiteKing : board.blackKing);
    return !isSquareAttacked(board, kingSq, !whiteToMove);
}
