#include "search.h"
#include <algorithm>

const int CHECKMATE = 100000;
const int STALEMATE = 0;

TTEntry TT[TT_SIZE];

void clearTT()
{
    for (int i = 0; i < TT_SIZE; i++)
    {
        TT[i].hashKey = 0;
        TT[i].depth = 0;
        TT[i].flag = 0;
        TT[i].score = 0;
        TT[i].bestMove = Move{ -1, -1, 0, 0, 0, 0 };
    }
}

void storeTT(u64 hash, int depth, int flag, int score, Move bestMove)
{
    int index = hash % TT_SIZE;

    if (TT[index].hashKey == 0 || TT[index].depth <= depth) {
        TT[index].hashKey = hash;
        TT[index].depth = depth;
        TT[index].flag = flag;
        TT[index].score = score;
        TT[index].bestMove = bestMove;
    }
}

bool probeTT(u64 hash, int depth, int alpha, int beta, int& returnScore, Move& returnMove) {
    int index = hash % TT_SIZE;
    TTEntry entry = TT[index];

    if (entry.hashKey == hash) {
        returnMove = entry.bestMove;

        if (entry.depth >= depth) {
            if (entry.flag == TT_EXACT) {
                returnScore = entry.score;
                return true;
            }
            if (entry.flag == TT_ALPHA && entry.score <= alpha) {
                returnScore = entry.score;
                return true;
            }
            if (entry.flag == TT_BETA && entry.score >= beta) {
                returnScore = entry.score;
                return true;
            }
        }
    }
    return false;
}

int getPieceValue(int piece)
{
    if (piece == WP || piece == BP) return 100;
    if (piece == WN || piece == BN) return 320;
    if (piece == WB || piece == BB) return 330;
    if (piece == WR || piece == BR) return 500;
    if (piece == WQ || piece == BQ) return 900;
    return 0;
}

int scoreMove(const Move& m)
{
    int score = 0;
    if (m.flags & CAPTURE)
    {
        score = 1000 + getPieceValue(m.capturePiece) - getPieceValue(m.piece);
    }
    if (m.flags & PROMOTION) score += 900;
    return score;
}

int minimax(Board& board, int depth, int alpha, int beta)
{
    if (depth == 0) return evaluate(board);

    int ttScore;
    Move ttMove;
    if (probeTT(board.hashKey, depth, alpha, beta, ttScore, ttMove)) {
        return ttScore;
    }

    MoveList moves = board.whiteToMove ? generateWhiteLegalMoves(board) : generateBlackLegalMoves(board);

    std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b)
    {
        return scoreMove(a) > scoreMove(b);
    });

    if (moves.count == 0)
    {
        if (isCheckmate(board)) return board.whiteToMove ? -CHECKMATE : CHECKMATE;
        return STALEMATE;
    }

    int flag = TT_ALPHA;
    Move bestMoveFound = { -1, -1, 0, 0, 0, 0 };

    if (board.whiteToMove)
    {
        int flag = TT_ALPHA;
        int maxScore = -1000000;
        for (const Move& m : moves)
        {
            board.makeMove(m);
            int score = minimax(board, depth - 1, alpha, beta);
            board.undoMove(m);

            if (score > maxScore)
            {
                maxScore = score;
                bestMoveFound = m;
            }
            if (score > alpha)
            {
                alpha = score;
                flag = TT_EXACT;
            }
            if (alpha >= beta)
            {
                flag = TT_BETA;
                break;
            }
        }
        storeTT(board.hashKey, depth, flag, maxScore, bestMoveFound);
        return maxScore;
    }
    else
    {
        int flag = TT_BETA;
        int minScore = 1000000;
        for (const Move& m : moves)
        {
            board.makeMove(m);
            int score = minimax(board, depth - 1, alpha, beta);
            board.undoMove(m);

            if (score < minScore)
            {
                minScore = score;
                bestMoveFound = m;
            }
            if (score < beta)
            {
                beta = score;
                flag = TT_EXACT;
            }
            if (alpha >= beta)
            {
                flag = TT_ALPHA;
                break;
            }
        }
        storeTT(board.hashKey, depth, flag, minScore, bestMoveFound);
        return minScore;
    }
}

Move bestMove(Board& board, int depth)
{
    clearTT();

    MoveList moves = board.whiteToMove ? generateWhiteLegalMoves(board) : generateBlackLegalMoves(board);

    std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b)
    {
    return scoreMove(a) > scoreMove(b);
    });

    Move best = { -1, -1, 0, 0, 0, 0 };
    int alpha = -1000000;
    int beta = 1000000;

    if (board.whiteToMove)
    {
        int bestScore = -1000000;
        for (const Move& m : moves)
        {
            board.makeMove(m);
            int score = minimax(board, depth - 1, alpha, beta);
            board.undoMove(m);

            if (score > bestScore || best.from == -1)
            {
                bestScore = score;
                best = m;
            }
            alpha = std::max(alpha, bestScore);
        }
    }
    else
    {
        int bestScore = 1000000;
        for (const Move& m : moves)
        {
            board.makeMove(m);
            int score = minimax(board, depth - 1, alpha, beta);
            board.undoMove(m);

            if (score < bestScore || best.from == -1)
            {
                bestScore = score;
                best = m;
            }
            beta = std::min(beta, bestScore);
        }
    }
    return best;
}