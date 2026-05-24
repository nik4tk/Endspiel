#include "hash.h"

u64 pieceKeys[12][64];
u64 enPassantKeys[8];
u64 castleKeys[16];
u64 sideKey;


u64 randomU64()
{
    static std::mt19937_64 rng(12345);
    return rng();
}

void initZobrist()
{
    for (int piece = 0; piece < 12; piece++) {
        for (int sq = 0; sq < 64; sq++) {
            pieceKeys[piece][sq] = randomU64();
        }
    }
    for (int i = 0; i < 8; i++) {
        enPassantKeys[i] = randomU64();
    }
    for (int i = 0; i < 16; i++) {
        castleKeys[i] = randomU64();
    }
    sideKey = randomU64();
}

u64 generateHash(const Board& board)
{
    u64 hash = 0;

    for (int sq = 0; sq < 64; sq++)
    {
        int piece = board.getPieceOnSquare(sq);
        if (piece != NO_PIECE)
        {
            hash ^= pieceKeys[piece][sq];
        }
    }

    if (!board.whiteToMove)
    {
        hash ^= sideKey;
    }

    int castleIndex = 0;
    if (board.castleWK) castleIndex |= 1;
    if (board.castleWQ) castleIndex |= 2;
    if (board.castleBK) castleIndex |= 4;
    if (board.castleBQ) castleIndex |= 8;

    hash ^= castleKeys[castleIndex];

    if (board.enPassantSq != -1)
    {
        int epFile = board.enPassantSq % 8;
        hash ^= enPassantKeys[epFile];
    }

    return hash;
}