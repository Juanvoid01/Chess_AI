#pragma once

#include "PieceType.hpp"

enum class MoveType;

class Move
{
public:
    short iniRow;
    short iniCol;
    short endRow;
    short endCol;
    MoveType type;

    Move(short iniRow, short iniCol, short endRow, short endCol, MoveType type);

    Move(Move &&other) noexcept;

    Move(const Move &other);

    ~Move(){}

    Move &operator=(Move &&other) noexcept;
};

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

struct PieceInfo
{
    PieceType type;
    PieceColor color;
};