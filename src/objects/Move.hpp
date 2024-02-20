#pragma once

#include "PieceType.hpp"

enum class MoveType
{
    QUIET = 0,
    KINGCASTLE = 1,
    QUEENCASTLE = 2,
    DOUBLEPAWNPUSH = 3,
    ENPASSANT = 4,
    CAPTURE = 5,
    QUEENPROMOTION = 6,
    ROOKPROMOTION = 7,
    BISHOPPROMOTION = 8,
    KNIGHTPROMOTION = 9,
    QUEENPROMOCAPTURE = 10,
    ROOKPROMOCAPTURE = 11,
    BISHOPPROMOCAPTURE = 12,
    KNIGHTPROMOCAPTURE = 13
};

class Move
{
public:
    short iniRow;
    short iniCol;
    short endRow;
    short endCol;
    MoveType type;

    Move(short iniRow = -1, short iniCol = -1, short endRow = -1, short endCol = -1, MoveType type = MoveType::QUIET);

    Move(Move &&other) noexcept;

    Move(const Move &other);

    ~Move() {}

    Move &operator=(const Move &other);

    Move &operator=(Move &&other) noexcept;
};

struct PieceInfo
{
    PieceType type;
    PieceColor color;

    bool operator==(const PieceInfo &other) const
    {
        return this->type == other.type && this->color == other.color;
    }

    bool operator==(const PieceInfo &&other) const
    {
        return this->type == other.type && this->color == other.color;
    }

    bool operator!=(const PieceInfo &&other) const
    {
        return !(*this == other);
    }
};

struct PosStateInfo
{
    bool kingMovedW;
    bool kingMovedB;
    bool rookKMovedW;
    bool rookQMovedW;
    bool rookKMovedB;
    bool rookQMovedB;

    int halfMoveCounter;
    int moveCounter;
    Move lastMove;
    PieceColor turn;

    PosStateInfo &operator=(const PosStateInfo &other)
    {
        if (this != &other)
        {
            kingMovedW = other.kingMovedW;
            kingMovedB = other.kingMovedB;
            rookKMovedW = other.rookKMovedW;
            rookQMovedW = other.rookQMovedW;
            rookKMovedB = other.rookKMovedB;
            rookQMovedB = other.rookQMovedB;
            halfMoveCounter = other.halfMoveCounter;
            lastMove = other.lastMove;
            moveCounter = other.moveCounter;
            turn = other.turn;
        }
        return *this;
    }
};