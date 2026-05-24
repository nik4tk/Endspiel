#pragma once
#include "move.h"
#include "board.h"
#include "movegen.h"
#include "eval.h"

int minimax(Board& board, int depth, int alpha, int beta, bool whiteToMove);
Move bestMove(Board& board, int depth, bool whiteToMove);