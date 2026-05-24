#include "search.h"
#include <algorithm>

const int CHECKMATE = 100000;
const int STALEMATE = 0;

int minimax(Board& board, int depth, int alpha, int beta)
{
    if (depth == 0) return evaluate(board);

    MoveList moves = board.whiteToMove ? generateWhiteLegalMoves(board) : generateBlackLegalMoves(board);

    if (moves.count == 0)
    {
        if (isCheckmate(board)) return board.whiteToMove ? -CHECKMATE : CHECKMATE;
        return STALEMATE;
    }

    if (board.whiteToMove)
    {
        int maxScore = -1000000;
        for (const Move& m : moves)
        {
            board.makeMove(m);
            int score = minimax(board, depth - 1, alpha, beta);
            board.undoMove(m);
            maxScore = std::max(maxScore, score);
            alpha = std::max(alpha, score);
            if (beta <= alpha) break;
        }
        return maxScore;
    }
    else
    {
        int minScore = 1000000;
        for (const Move& m : moves)
        {
            board.makeMove(m);
            int score = minimax(board, depth - 1, alpha, beta);
            board.undoMove(m);
            minScore = std::min(minScore, score);
            beta = std::min(beta, score);
            if (beta <= alpha) break;
        }
        return minScore;
    }
}

Move bestMove(Board& board, int depth)
{
    MoveList moves = board.whiteToMove ? generateWhiteLegalMoves(board) : generateBlackLegalMoves(board);
    Move best;
    int bestScore = board.whiteToMove ? -1000000 : 1000000;

    for (const Move& m : moves)
    {
        board.makeMove(m);
        int score = minimax(board, depth - 1, -1000000, 1000000);
        board.undoMove(m);

        if (board.whiteToMove)
        {
            if (score > bestScore)
            {
                bestScore = score;
                best = m;
            }
        }
        else
        {
            if (score < bestScore)
            {
                bestScore = score;
                best = m;
            }
        }
    }

    return best;
}