#include "Move.hpp"

Move::Move(short iniRow, short iniCol, short endRow, short endCol, MoveType type)
    : iniRow(iniRow), iniCol(iniCol), endRow(endRow), endCol(endCol), type(type), evaluation(0)
{
}

Move::Move(const Move &other)
    : iniRow(other.iniRow), iniCol(other.iniCol), endRow(other.endRow), endCol(other.endCol), type(other.type), evaluation(other.evaluation)
{
}

Move::Move(Move &&other) noexcept
    : iniRow(other.iniRow), iniCol(other.iniCol), endRow(other.endRow), endCol(other.endCol), type(other.type), evaluation(other.evaluation)
{
}

bool Move::IsValid() const
{
    return type != MoveType::INVALID && iniRow >= 0 && iniCol >= 0 && endRow >= 0 && endCol >= 0;
}

bool Move::IsPromotion() const
{
    return type == MoveType::QUEENPROMOTION ||
           type == MoveType::KNIGHTPROMOTION ||
           type == MoveType::ROOKPROMOTION ||
           type == MoveType::BISHOPPROMOTION ||
           type == MoveType::QUEENPROMOCAPTURE ||
           type == MoveType::KNIGHTPROMOCAPTURE ||
           type == MoveType::ROOKPROMOCAPTURE ||
           type == MoveType::BISHOPPROMOCAPTURE;
}

Move &Move::operator=(const Move &other)
{
    if (this != &other)
    {
        iniRow = other.iniRow;
        iniCol = other.iniCol;
        endRow = other.endRow;
        endCol = other.endCol;
        type = other.type;
        evaluation = other.evaluation;
    }
    return *this;
}

Move &Move::operator=(Move &&other) noexcept
{
    if (this != &other)
    {
        iniRow = other.iniRow;
        iniCol = other.iniCol;
        endRow = other.endRow;
        endCol = other.endCol;
        type = other.type;
        evaluation = other.evaluation;
    }
    return *this;
}

std::string Move::ToBasicString() const
{
    std::string result;
    result.reserve(4);

    result.push_back('a' + iniCol);
    result.push_back('1' + iniRow);
    result.push_back('a' + endCol);
    result.push_back('1' + endRow);

    return result;
}

// inline char colToChar(int col) const { return 'a' + col - 1; }

/*std::string Move::ToString()
{
    std::string result;
       result.reserve(8);

       if (shortCastle)
       {
           result.append("0-0");
       }
       else if (longCastle)
       {
           result.append("0-0-0");
       }
       else
       {
           if (piece == PieceType::PAWN)
           {
               result.append("" + colToChar(iniCol));
           }
           else
           {
               result.append("" + pieceTypeToChar(piece));
           }

           if (specifyIniCol)
           {
               result.append("" + colToChar(iniCol));
           }

           if (specifyIniRow)
           {
               result.append("" + ('0' + iniRow));
           }

           if (capture)
           {
               result.append("" + 'x');
           }

           result.append("" + colToChar(endCol) + ('0' + endRow));

           if (promotion)
           {
               result.append("=" + pieceTypeToChar(piece));
           }
       }

       if (checkMate)
       {
           result.append("#");
       }
       else if (check)
       {
           result.append("+");
       }

    return result;
}*/
/*
char Move::pieceTypeToChar(PieceType type) const
{
    switch (type)
    {
    case PieceType::KING:
        return 'K';
        break;
    case PieceType::QUEEN:
        return 'Q';
        break;
    case PieceType::KNIGHT:
        return 'N';
        break;
    case PieceType::BISHOP:
        return 'B';
        break;
    case PieceType::ROOK:
        return 'R';
        break;
    default:
        return '0';
        break;
    }
}
*/
