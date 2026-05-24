#include "eval.h"

int evaluate(const Board& board)
{
    int score = 0;

    // Basic material evaluation
    score += std::popcount(board.whitePawns) * 100;
    score += std::popcount(board.whiteKnights) * 320;
    score += std::popcount(board.whiteBishops) * 330;
    score += std::popcount(board.whiteRooks) * 500;
    score += std::popcount(board.whiteQueens) * 900;

    score -= std::popcount(board.blackPawns) * 100;
    score -= std::popcount(board.blackKnights) * 320;
    score -= std::popcount(board.blackBishops) * 330;
    score -= std::popcount(board.blackRooks) * 500;
    score -= std::popcount(board.blackQueens) * 900;

    return score;
}