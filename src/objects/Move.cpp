#include "Move.hpp"

Move::Move(short iniRow, short iniCol, short endRow, short endCol, MoveType type)
    : iniRow(iniRow), iniCol(iniCol), endRow(endRow), endCol(endCol), type(type)
{
}

Move::Move(const Move &other)
    : iniRow(other.iniRow), iniCol(other.iniCol), endRow(other.endRow), endCol(other.endCol), type(other.type)
{
}

Move::Move(Move &&other) noexcept
    : iniRow(other.iniRow), iniCol(other.iniCol), endRow(other.endRow), endCol(other.endCol), type(other.type)
{
}

Move& Move::operator=(const Move &other)
{
    if (this != &other)
    { 
        iniRow = other.iniRow;
        iniCol = other.iniCol;
        endRow = other.endRow;
        endCol = other.endCol;
        type = other.type;
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
    }
    return *this;
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
