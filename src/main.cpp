#include "board.h"
#include "movegen.h"
#include "performanceTest.h"

#include <iostream>

int main()
{
	std::cout << "Welcome to 'Zwischenzug'\n\n";

	Board board;
	board.init();
	board.print();
	startUp();


	// TESTING
	int depth = 5;
	bool whiteToMove = true;
	perftDivide(board, depth, whiteToMove);

	return 0;
}