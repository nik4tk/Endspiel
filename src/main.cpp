#include "board.h"
#include "movegen.h"
#include <iostream>

int main()
{
	std::cout << "Welcome to 'Zwischenzug'\n\n";

	Board board;
	board.init();
	board.print();

	startUp();

	return 0;
}