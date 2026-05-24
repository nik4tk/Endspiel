#include "board.h"
#include "movegen.h"
#include "search.h"
#include "performanceTest.h"

#include <iostream>
#include <string>
#include <sstream>

Move parseMove(Board& board, std::string moveString)
{
	//MoveList moves = board.whiteToMove
	return Move();
}

int main()
{
	std::cout << "Welcome to 'Zwischenzug'\n\n";

	Board board;
	board.init();
	board.print();
	startUp();

	std::cout << std::endl;


	// TESTING
	int depth = 1;
	bool whiteToMove = true;
	perftDivide(board, depth, whiteToMove);

	return 0;
}