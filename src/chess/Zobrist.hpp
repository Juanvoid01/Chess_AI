#pragma once

#include <random>
#include "Move.hpp"

static const int BOARD_SIZE = 8;
static const int NUM_PIECES = 12;

// Zobrist hashing implementation for transposition tables
class Zobrist
{
public:
    // generate random keys to calculate hashValue
    Zobrist();
    ~Zobrist() {}

    // put hashValue doint XOR of all pieces on board
    void InitializeHashWithPos(const PieceArray &pieces);

    // returns the zobrist value of the position
    inline uint64_t GetHashValue() const { return hashValue; }

    // modifies the HashValue of the position, it performs XOR with the key value of the movement
    inline void AddPiece(short row, short col, PieceInfo piece);

    // modifies the HashValue of the position, it performs XOR with the key value of the movement
    inline void DeletePiece(short row, short col, PieceInfo piece);

    // hashValue = newHashValue
    inline void PutHashValue(uint64_t newHashValue) { hashValue = newHashValue; }

    // CALL BEFORE MAKING THE MOVE
    // modifies the HashValue, it calls Add and Delete pieces that the move produces
    void AddMove(Move move, const PieceArray &pieces);

private:
    uint64_t hashValue;
    uint64_t keys[BOARD_SIZE][BOARD_SIZE][NUM_PIECES];

    // returns the index in the array of the piece
    int PieceToNumber(PieceInfo piece) const;
};
