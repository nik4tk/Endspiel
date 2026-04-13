#pragma once

struct Move {
    int from;
    int to;
    int piece;
    int capturePiece;
    int flags; // promotion, castle, en passant
};