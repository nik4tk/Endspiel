# Logical Errors in Chess Engine Implementation - RESOLVED

All previously identified logical errors have been fixed.

## 1. Board Management (`src/board.cpp`) - FIXED

### Undo History Indexing
Fixed: Now uses pre-decrement `history[--historyCount]` to correctly access the last stored state.

### Promotion Undo Logic
Fixed: Added logic to properly remove the promoted piece (e.g., Queen) using `&= ~toBB` before restoring the pawn.

### En Passant Double Restoration
Fixed: Added a condition `!(m.flags & EN_PASSANT)` to the general capture restoration logic to prevent placing a ghost pawn at the target square during an en passant undo.

---

## 2. Move Generation (`src/movegen.cpp`) - FIXED

### Black Piece Logic Errors
Fixed:
- `generateBlackPawnMoves` now correctly uses `BP` for en passant moves.
- `generateBlackKingMoves` now correctly uses `BK` for castling moves.
- Castling safety check now correctly passes `true` to `isSquareAttacked` to check for white piece attacks.

### Sliding Piece Boundary Errors
Fixed: Updated loop conditions (e.g., `s % 8 != 0` for Right and `s % 8 != 7` for Left) to ensure pieces on the board edges are correctly identified as attackers.

---

## 3. Search Algorithm (`src/search.cpp`) - FIXED

### Functional Minimax
Implemented a full Minimax algorithm with Alpha-Beta pruning:
- Recursive depth search.
- Proper handling of checkmate (returns high/low score) and stalemate (returns 0).
- `bestMove` now correctly searches the move tree and returns the optimal move.

### Evaluation Function (`src/eval.cpp`)
Implemented a basic material-based evaluation function using C++20 `std::popcount`.
