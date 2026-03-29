#include "board.h"
#include "movegen.h"
#include <iostream>

int main()
{
	std::cout << "Chess Engine started\n\n";

	Board board;
	board.init();
	board.print();

	startUp();

	return 0;
}