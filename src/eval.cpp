#include "eval.h"

int evaluate(const Board& board)
{
    int score = 0;

    // White pieces
    u64 wp = board.whitePawns;
    while (wp)
    {
        int sq = get_LSB(wp);
        score += 100 + PawnTable[sq];
        clear_bit(wp, sq);
    }

    u64 wn = board.whiteKnights;
    while (wn)
    {
        int sq = get_LSB(wn);
        score += 320 + KnightTable[sq];
        clear_bit(wn, sq);
    }

    u64 wb = board.whiteBishops;
    while (wb)
    {
        int sq = get_LSB(wb);
        score += 330 + BishopTable[sq];
        clear_bit(wb, sq);
    }

    u64 wr = board.whiteRooks;
    while (wr)
    {
        int sq = get_LSB(wr);
        score += 500 + KnightTable[sq];
        clear_bit(wr, sq);
    }

    score += std::popcount(board.whiteQueens) * 900;

    u64 wk = board.whiteKing;
    if (wk)
    {
        int sq = get_LSB(wk);
        score += KingTable[sq];
    }


    // Black Pieces
    u64 bp = board.blackPawns;
    while (bp)
    {
        int sq = get_LSB(bp);
        score -= (100 + PawnTable[sq ^ 56]);
        clear_bit(bp, sq);
    }

    u64 bn = board.blackKnights;
    while (bn)
    {
        int sq = get_LSB(bn);
        score -= (320 + KnightTable[sq ^ 56]);
        clear_bit(bn, sq);
    }

    u64 bb = board.blackBishops;
    while (bb)
    {
        int sq = get_LSB(bb);
        score -= (330 + BishopTable[sq ^ 56]);
        clear_bit(bb, sq);
    }

    u64 br = board.blackRooks;
    while (br)
    {
        int sq = get_LSB(br);
        score -= (500 + KnightTable[sq ^ 56]);
        clear_bit(br, sq);
    }

    score -= std::popcount(board.blackQueens) * 900;

    u64 bk = board.blackKing;
    if (bk)
    {
        int sq = get_LSB(bk);
        score -= KingTable[sq ^ 56];
    }

    return score;
}