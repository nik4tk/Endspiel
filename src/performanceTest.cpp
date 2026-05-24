#include "performanceTest.h"

std::string squareToString(int sq)
{
	char file = 'a' + (sq % 8);
	char rank = '1' + (sq / 8);
	return std::string{ file, rank };
}

u64 perft(Board& board, int depth, bool whiteToMove)
{
	if (depth == 0) return 1ULL;

	u64 nodes = 0;
	MoveList moves = whiteToMove ? generateWhiteLegalMoves(board) : generateBlackLegalMoves(board);

	for (const Move& m : moves)
	{
		board.makeMove(m);
		nodes += perft(board, depth - 1, !whiteToMove);
		board.undoMove(m);
	}

	return nodes;
}

void perftDivide(Board& board, int depth, bool whiteToMove)
{
	if (depth == 0) return;

	std::cout << "--- Performance-Test Divide Depth " << depth << " ---\n";
	auto startTime = std::chrono::high_resolution_clock::now();

	u64 totalNodes = 0;
	MoveList moves = whiteToMove ? generateWhiteLegalMoves(board) : generateBlackLegalMoves(board);

	for (const Move& m : moves)
	{
		board.makeMove(m);
		u64 nodes = perft(board, depth - 1, !whiteToMove);
		board.undoMove(m);

		std::cout << squareToString(m.from) << squareToString(m.to) << ": " << nodes << "\n";
		totalNodes += nodes;
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = endTime - startTime;

	std::cout << "\nTotal Nodes: " << totalNodes << "\n";
	std::cout << "\nTime taken: " << elapsed.count() << " seconds\n";
	std::cout << "\nNodes/sec: " << (totalNodes / elapsed.count()) << "\n\n";
}
