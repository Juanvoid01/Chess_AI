#pragma once

#include "PieceType.hpp"
#include <string>
#include <array>

enum class MoveType
{
    INVALID = -1,
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
    short evaluation; // used in the moveOrder class

    Move(short iniRow = -1, short iniCol = -1, short endRow = -1, short endCol = -1, MoveType type = MoveType::QUIET);

    Move(Move &&other) noexcept;

    Move(const Move &other);

    ~Move() {}

    Move &operator=(const Move &other);

    Move &operator=(Move &&other) noexcept;

    bool operator==(const Move &other) const;

    bool IsValid() const;

    bool IsPromotion() const;

    std::string ToBasicString() const;
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
    PieceInfo capturedPiece;

    short kingRowW;
    short kingColW;
    short kingRowB;
    short kingColB;

    bool check;

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
            capturedPiece = other.capturedPiece;
            kingRowW = other.kingRowW;
            kingColW = other.kingColW;
            kingRowB = other.kingRowB;
            kingColB = other.kingColB;
            check = other.check;
        }
        return *this;
    }
};

typedef std::array<std::array<PieceInfo, 8>, 8> PieceArray;

const Move InvalidMove = Move(-1, -1, -1, -1, MoveType::INVALID);
