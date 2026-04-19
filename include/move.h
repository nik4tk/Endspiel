#pragma once

enum Piece {
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK,
    NO_PIECE = -1
};

enum MoveFlags {
    QUIET = 0,
    CAPTURE = 1 << 0,
    DOUBLE_PAWN = 1 << 1,
    EN_PASSANT = 1 << 2,
    CASTLING = 1 << 3,
    PROMOTION = 1 << 4
};

struct Move {
    int from;
    int to;
    int piece;
    int capturePiece;
    int flags; // captures, en passant, castling, ect...
    int promotionPiece;
};