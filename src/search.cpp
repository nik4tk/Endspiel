#include "search.h"

int minimax(Board& board, int depth, bool whiteToMove)
{
	if (depth == 0) return evaluate(board);

	MoveList moves = whiteToMove ? generateWhiteLegalMoves(board) : generateBlackLegalMoves(board);

	if (moves.count == 0)
	{
		int kingSq = get_LSB(whiteToMove ? board.whiteKing : board.blackKing);
	}
}

Move bestMove(Board& board, int depth, bool whiteToMove)
{
	return Move();
}