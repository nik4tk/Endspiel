#pragma once
#include "board.h"
#include "movegen.h"

#include <iostream>
#include <string>
#include <chrono>

std::string squareToString(int sq);
u64 perft(Board& board, int depth, bool whiteToMove);
void perftDivide(Board& board, int depth, bool whiteToMove);
