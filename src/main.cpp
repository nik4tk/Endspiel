#include "board.h"
#include "movegen.h"
#include "search.h"
#include "performanceTest.h"

#include <iostream>
#include <string>
#include <sstream>

Move parseMove(Board& board, std::string moveString)
{
	MoveList moves = board.whiteToMove ? generateWhiteLegalMoves(board) : generateBlackLegalMoves(board);

	for (const Move& m : moves)
	{
		char fromFile = 'a' + (m.from % 8);
		char fromRank = '1' + (m.from / 8);
		char toFile = 'a' + (m.to % 8);
		char toRank = '1' + (m.to / 8);
		std::string mStr = { fromFile, fromRank, toFile, toRank };

		if (m.flags & PROMOTION) {
			if (m.promotionPiece == WQ || m.promotionPiece == BQ) mStr += 'q';
			else if (m.promotionPiece == WR || m.promotionPiece == BR) mStr += 'r';
			else if (m.promotionPiece == WB || m.promotionPiece == BB) mStr += 'b';
			else if (m.promotionPiece == WN || m.promotionPiece == BN) mStr += 'n';
		}

		if (moveString == mStr) return m;
	}
	return Move{ -1, -1, NO_PIECE, NO_PIECE, 0, NO_PIECE };
}

void uciLoop()
{
	Board board;
	board.init();
	std::string line;

	std::cout << "Zwischenzug UCI active. Waiting for GUI..." << std::endl;

	while (std::getline(std::cin, line))
	{
		std::istringstream iss(line);
		std::string command;
		iss >> command;

		if (command == "uci")
		{
			std::cout << "id name Zwischenzug" << std::endl;
			std::cout << "id author nik4tk" << std::endl;
			std::cout << "uciok" << std::endl;
		}
		else if (command == "isready")
		{
			std::cout << "readyok" << std::endl;
		}
		else if (command == "position") {
			std::string subCommand;
			iss >> subCommand;

			if (subCommand == "startpos") {
				board.init(); // Reset board
			}

			iss >> subCommand; // Check if the word "moves" is next
			if (subCommand == "moves") {
				std::string moveStr;
				while (iss >> moveStr) {
					Move m = parseMove(board, moveStr);
					if (m.from != -1) {
						board.makeMove(m);
					}
				}
			}
		}
		else if (command == "go")
		{
			Move best = bestMove(board, 4);

			char fromFile = 'a' + (best.from % 8);
			char fromRank = '1' + (best.from / 8);
			char toFile = 'a' + (best.to % 8);
			char toRank = '1' + (best.to / 8);

			std::cout << "bestmove " << fromFile << fromRank << toFile << toRank;

			if (best.flags & PROMOTION) {
				if (best.promotionPiece == WQ || best.promotionPiece == BQ) std::cout << "q";
				else if (best.promotionPiece == WR || best.promotionPiece == BR) std::cout << "r";
				else if (best.promotionPiece == WB || best.promotionPiece == BB) std::cout << "b";
				else if (best.promotionPiece == WN || best.promotionPiece == BN) std::cout << "n";
			}
			std::cout << std::endl;
		}
		else if (command == "quit")
		{
			break;
		}
	}
}

int main()
{
	startUp();
	uciLoop();
	return 0;
}