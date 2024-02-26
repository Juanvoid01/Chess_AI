#include "MoveGenerator.hpp"
#include <iostream>

MoveGenerator::MoveGenerator(const std::string &FEN)
{
    LoadFEN(FEN);
}

void MoveGenerator::SetInitialPosition()
{
    for (short row = 2; row < 6; row++)
    {
        for (short col = 0; col < 8; col++)
        {
            pieces[row][col] = {PieceType::EMPTY, PieceColor::WHITE};
        }
    }

    for (short col = 0; col < 8; col++)
    {
        pieces[1][col] = {PieceType::PAWN, PieceColor::WHITE};
        pieces[6][col] = {PieceType::PAWN, PieceColor::BLACK};
    }

    pieces[0][0] = {PieceType::ROOK, PieceColor::WHITE};
    pieces[0][1] = {PieceType::KNIGHT, PieceColor::WHITE};
    pieces[0][2] = {PieceType::BISHOP, PieceColor::WHITE};
    pieces[0][3] = {PieceType::QUEEN, PieceColor::WHITE};
    pieces[0][4] = {PieceType::KING, PieceColor::WHITE};
    stateInfo.kingRowW = 0;
    stateInfo.kingColW = 4;
    pieces[0][5] = {PieceType::BISHOP, PieceColor::WHITE};
    pieces[0][6] = {PieceType::KNIGHT, PieceColor::WHITE};
    pieces[0][7] = {PieceType::ROOK, PieceColor::WHITE};

    pieces[7][0] = {PieceType::ROOK, PieceColor::BLACK};
    pieces[7][1] = {PieceType::KNIGHT, PieceColor::BLACK};
    pieces[7][2] = {PieceType::BISHOP, PieceColor::BLACK};
    pieces[7][3] = {PieceType::QUEEN, PieceColor::BLACK};
    pieces[7][4] = {PieceType::KING, PieceColor::BLACK};
    stateInfo.kingRowB = 7;
    stateInfo.kingColB = 4;

    pieces[7][5] = {PieceType::BISHOP, PieceColor::BLACK};
    pieces[7][6] = {PieceType::KNIGHT, PieceColor::BLACK};
    pieces[7][7] = {PieceType::ROOK, PieceColor::BLACK};

    stateInfo.kingMovedW = false;
    stateInfo.kingMovedB = false;
    stateInfo.rookKMovedW = false;
    stateInfo.rookQMovedW = false;
    stateInfo.rookKMovedB = false;
    stateInfo.rookQMovedB = false;
};

void MoveGenerator::ClearBoard()
{
    for (short row = 0; row < 8; row++)
    {
        for (short col = 0; col < 8; col++)
        {
            ClearPos(row, col);
        }
    }
}

void MoveGenerator::MakeMove(Move move)
{

    stateInfo.capturedPiece = pieces[move.endRow][move.endCol];

    if (move.type == MoveType::KINGCASTLE)
    {
        pieces[move.endRow][move.endCol - 1] = {PieceType::ROOK, stateInfo.turn};
        ClearPos(move.endRow, 7);
        pieces[move.endRow][move.endCol] = pieces[move.iniRow][move.iniCol];
    }
    else if (move.type == MoveType::QUEENCASTLE)
    {

        pieces[move.endRow][move.endCol + 1] = {PieceType::ROOK, stateInfo.turn};
        ClearPos(move.endRow, 0);
        pieces[move.endRow][move.endCol] = pieces[move.iniRow][move.iniCol];
    }
    else if (move.type == MoveType::ENPASSANT)
    {
        ClearPos(move.iniRow, move.endCol);
        pieces[move.endRow][move.endCol] = pieces[move.iniRow][move.iniCol];
    }
    else if (move.type == MoveType::QUEENPROMOTION || move.type == MoveType::QUEENPROMOCAPTURE)
    {
        pieces[move.endRow][move.endCol] = {PieceType::QUEEN, pieces[move.iniRow][move.iniCol].color};
    }
    else if (move.type == MoveType::KNIGHTPROMOTION || move.type == MoveType::KNIGHTPROMOCAPTURE)
    {
        pieces[move.endRow][move.endCol] = {PieceType::KNIGHT, pieces[move.iniRow][move.iniCol].color};
    }
    else if (move.type == MoveType::BISHOPPROMOTION || move.type == MoveType::BISHOPPROMOCAPTURE)
    {
        pieces[move.endRow][move.endCol] = {PieceType::BISHOP, pieces[move.iniRow][move.iniCol].color};
    }
    else if (move.type == MoveType::ROOKPROMOTION || move.type == MoveType::ROOKPROMOCAPTURE)
    {
        pieces[move.endRow][move.endCol] = {PieceType::ROOK, pieces[move.iniRow][move.iniCol].color};
    }
    else
    {
        pieces[move.endRow][move.endCol] = pieces[move.iniRow][move.iniCol];
    }

    ClearPos(move.iniRow, move.iniCol);

    if (stateInfo.turn == PieceColor::WHITE)
    {
        if (pieces[move.endRow][move.endCol].type == PieceType::KING)
        {
            stateInfo.kingRowW = move.endRow;
            stateInfo.kingColW = move.endCol;
            stateInfo.kingMovedW = true;
        }

        if (move.type == MoveType::KINGCASTLE)
        {
            stateInfo.rookKMovedW = true;
        }
        else if (move.type == MoveType::QUEENCASTLE)
        {
            stateInfo.rookQMovedW = true;
        }

        stateInfo.turn = PieceColor::BLACK;
    }
    else
    {
        if (pieces[move.endRow][move.endCol].type == PieceType::KING)
        {
            stateInfo.kingRowB = move.endRow;
            stateInfo.kingColB = move.endCol;
            stateInfo.kingMovedB = true;
        }

        if (move.type == MoveType::KINGCASTLE)
        {
            stateInfo.rookKMovedB = true;
        }
        else if (move.type == MoveType::QUEENCASTLE)
        {
            stateInfo.rookQMovedB = true;
        }

        stateInfo.turn = PieceColor::WHITE;
        stateInfo.moveCounter++;
    }

    if (isCapture(move.type))
    {
        stateInfo.halfMoveCounter = 0;
    }
    else if (pieces[move.endRow][move.endCol].type == PieceType::PAWN)
    {
        stateInfo.halfMoveCounter = 0;
    }
    else
    {
        stateInfo.halfMoveCounter++;
    }

    stateInfo.lastMove = move;
}

void MoveGenerator::UnMakeMove(Move move, const PosStateInfo &lastStateInfo)
{
   
    if (move.type == MoveType::KINGCASTLE)
    {
        pieces[move.iniRow][move.iniCol] = pieces[move.endRow][move.endCol];
        pieces[move.endRow][7] = {PieceType::ROOK, pieces[move.iniRow][move.iniCol].color};
        ClearPos(move.endRow, move.endCol - 1);
    }
    else if (move.type == MoveType::QUEENCASTLE)
    {
        pieces[move.iniRow][move.iniCol] = pieces[move.endRow][move.endCol];
        pieces[move.endRow][0] = {PieceType::ROOK, pieces[move.iniRow][move.iniCol].color};
        ClearPos(move.endRow, move.endCol + 1);
    }
    else if (move.type == MoveType::ENPASSANT)
    {
        pieces[move.iniRow][move.iniCol] = pieces[move.endRow][move.endCol];
        pieces[move.iniRow][move.endCol] = {PieceType::PAWN, stateInfo.turn};
    }
    else if (move.type == MoveType::QUEENPROMOTION || move.type == MoveType::QUEENPROMOCAPTURE)
    {
        pieces[move.iniRow][move.iniCol] = {PieceType::PAWN, pieces[move.endRow][move.endCol].color};
    }
    else if (move.type == MoveType::KNIGHTPROMOTION || move.type == MoveType::KNIGHTPROMOCAPTURE)
    {
        pieces[move.iniRow][move.iniCol] = {PieceType::PAWN, pieces[move.endRow][move.endCol].color};
    }
    else if (move.type == MoveType::BISHOPPROMOTION || move.type == MoveType::BISHOPPROMOCAPTURE)
    {
        pieces[move.iniRow][move.iniCol] = {PieceType::PAWN, pieces[move.endRow][move.endCol].color};
    }
    else if (move.type == MoveType::ROOKPROMOTION || move.type == MoveType::ROOKPROMOCAPTURE)
    {
        pieces[move.iniRow][move.iniCol] = {PieceType::PAWN, pieces[move.endRow][move.endCol].color};
    }
    else
    {
        pieces[move.iniRow][move.iniCol] = pieces[move.endRow][move.endCol];
    }

    ClearPos(move.endRow, move.endCol);

    // there is no way to know the lastMove and halfMoveCounter and castling rights

    if (isCapture(move.type) && move.type != MoveType::ENPASSANT)
    {
        if (stateInfo.capturedPiece.type != PieceType::EMPTY)
            pieces[move.endRow][move.endCol] = stateInfo.capturedPiece;
    }

    stateInfo = lastStateInfo;
}

bool MoveGenerator::IsCheckMate() const
{
    return numLegalMoves == 0 && checkersNum > 0;
}

bool MoveGenerator::IsStaleMate() const
{
    return numLegalMoves == 0 && checkersNum == 0;
}

void MoveGenerator::LoadFEN(const std::string &fen)
{
    stateInfo.kingRowW = -1, stateInfo.kingColW = -1;
    stateInfo.kingRowB = -1, stateInfo.kingColB = -1;
    stateInfo.halfMoveCounter = 0;
    stateInfo.moveCounter = 1;
    stateInfo.capturedPiece = {PieceType::EMPTY, PieceColor::WHITE};
    short row = 7;
    short col = 0;
    int i = 0;
    for (char c : fen)
    {
        if (c == ' ')
        {
            break;
        }
        else if (c == '/')
        {
            row--;
            col = 0;
        }
        else if (isdigit(c))
        {
            short limit = col + (short)(c - '0');
            for (col; col < limit; col++)
            {
                ClearPos(row, col);
            }
        }
        else
        {
            PieceColor pieceColor = islower(c) ? PieceColor::BLACK : PieceColor::WHITE;
            switch (c)
            {
            case 'P':
            case 'p':
                pieces[row][col] = {PieceType::PAWN, pieceColor};
                break;
            case 'N':
            case 'n':
                pieces[row][col] = {PieceType::KNIGHT, pieceColor};
                break;
            case 'B':
            case 'b':
                pieces[row][col] = {PieceType::BISHOP, pieceColor};
                break;
            case 'R':
            case 'r':
                pieces[row][col] = {PieceType::ROOK, pieceColor};
                break;
            case 'Q':
            case 'q':
                pieces[row][col] = {PieceType::QUEEN, pieceColor};
                break;
            case 'K':
            case 'k':
                pieces[row][col] = {PieceType::KING, pieceColor};
                if (pieceColor == PieceColor::WHITE)
                {
                    stateInfo.kingRowW = row;
                    stateInfo.kingColW = col;
                }
                else
                {
                    stateInfo.kingRowB = row;
                    stateInfo.kingColB = col;
                }
                break;
            default:
                break;
            }

            col++;
        }
        i++;
    }

    i++;

    stateInfo.turn = fen[i] == 'w' ? PieceColor::WHITE : PieceColor::BLACK;

    i += 2;
    char c;
    stateInfo.rookKMovedW = true;
    stateInfo.rookQMovedW = true;
    stateInfo.kingMovedW = true;
    stateInfo.rookKMovedB = true;
    stateInfo.rookQMovedB = true;
    stateInfo.kingMovedB = true;

    while ((c = fen[i]) != ' ')
    {
        if (c == 'K')
        {
            stateInfo.rookKMovedW = false;
            stateInfo.kingMovedW = false;
        }
        else if (c == 'Q')
        {
            stateInfo.rookQMovedW = false;
            stateInfo.kingMovedW = false;
        }
        else if (c == 'k')
        {
            stateInfo.rookKMovedB = false;
            stateInfo.kingMovedB = false;
        }
        else if (c == 'q')
        {
            stateInfo.rookQMovedB = false;
            stateInfo.kingMovedB = false;
        }
        i++;
    }

    i++;

    if (fen[i] != '-')
    {
        PieceColor color = fen[i + 1] == '3' ? PieceColor::WHITE : PieceColor::BLACK;
        short startRow = color == PieceColor::WHITE ? 1 : 6;
        short endRow = color == PieceColor::WHITE ? 3 : 4;

        short col = (short)fen[i] - 'a';
        stateInfo.lastMove = Move(startRow, col, endRow, col, MoveType::DOUBLEPAWNPUSH);
    }
    else
    {
        stateInfo.lastMove = Move(-1, -1, -1, -1, MoveType::QUIET);
    }

    i += 2;
    stateInfo.halfMoveCounter = (int)(fen[i] - '0');
    i++;
    while ((c = fen[i]) != ' ')
    {
        stateInfo.halfMoveCounter = 10 * stateInfo.halfMoveCounter + (int)(fen[i] - '0');
        i++;
    }
    i++;
    stateInfo.moveCounter = (int)(fen[i] - '0');
    i++;
    while ((c = fen[i]) != '\0' && (c = fen[i]) != ' ')
    {
        stateInfo.moveCounter = 10 * stateInfo.moveCounter + (int)(fen[i] - '0');
        i++;
    }
}

inline bool MoveGenerator::isCapture(MoveType type) const
{
    return type == MoveType::CAPTURE ||
           type == MoveType::KNIGHTPROMOCAPTURE ||
           type == MoveType::BISHOPPROMOCAPTURE ||
           type == MoveType::ROOKPROMOCAPTURE ||
           type == MoveType::QUEENPROMOCAPTURE ||
           type == MoveType::ENPASSANT;
}

void MoveGenerator::updateDangers()
{

    PieceColor color;

    for (short row = 0; row < 8; row++)
    {
        for (short col = 0; col < 8; col++)
        {
            color = pieces[row][col].color;

            if (PosEmpty(row, col) || color == stateInfo.turn)
                continue;

            switch (pieces[row][col].type)
            {
            case PieceType::PAWN:
                UpdatePawnDangers(row, col, color);
                break;
            case PieceType::KNIGHT:
                UpdateKnightDangers(row, col, color);
                break;
            case PieceType::KING:
                UpdateKingDangers(row, col, color);
                break;
            case PieceType::QUEEN:
                UpdateQueenDangers(row, col, color);
                break;
            case PieceType::ROOK:
                UpdateRookDangers(row, col, color);
                break;
            case PieceType::BISHOP:
                UpdateBishopDangers(row, col, color);
                break;
            default:
                break;
            }
        }
    }
};

void MoveGenerator::UpdatePawnDangers(short row, short col, PieceColor color)
{
    short dir = color == PieceColor::WHITE ? 1 : -1;

    short nextRow = row + dir;

    if (ValidPos(nextRow, col + 1))
    {
        SetAttackedSquare(nextRow, col + 1, true);
        SetkingDangerSquare(nextRow, col + 1, true);

        if (isEnemyKing(nextRow, col + 1, color))
        {
            AddChecker(row, col);
        }
    }

    if (ValidPos(nextRow, col - 1))
    {
        SetAttackedSquare(nextRow, col - 1, true);
        SetkingDangerSquare(nextRow, col - 1, true);

        if (isEnemyKing(nextRow, col - 1, color))
        {
            AddChecker(row, col);
        }
    }
}

void MoveGenerator::UpdateRookDangers(short row, short col, PieceColor color)
{
    bool attack = true;
    bool danger = true;
    bool pin = false;
    short rowPinned;
    short colPinned;

    for (short i = row + 1; i < 8; i++)
    {
        if (attack)
            SetAttackedSquare(i, col, true);

        if (danger)
            SetkingDangerSquare(i, col, true);

        if (!PosEmpty(i, col))
        {
            if (isEnemyKing(i, col, color))
            {
                if (attack)
                {
                    AddChecker(row, col);
                }
            }
            else
            {
                danger = false;
            }

            if (pin && isEnemyKing(i, col, color))
            {
                pin = false;
                SetPinnedPiece(rowPinned, colPinned, true);
            }
            else if (pin && !PosEmpty(i, col))
            {
                pin = false;
            }

            if (!pin && attack && isEnemyPiece(i, col, color))
            {
                pin = true;
                rowPinned = i;
                colPinned = col;
            }

            attack = false;
        }

        if (!attack && !danger && !pin)
            break;
    }

    attack = true;
    danger = true;
    pin = false;

    for (short i = row - 1; i >= 0; i--)
    {
        if (attack)
            SetAttackedSquare(i, col, true);

        if (danger)
            SetkingDangerSquare(i, col, true);

        if (!PosEmpty(i, col))
        {
            if (isEnemyKing(i, col, color))
            {
                if (attack)
                {
                    AddChecker(row, col);
                }
            }
            else
            {
                danger = false;
            }

            if (pin && isEnemyKing(i, col, color))
            {
                pin = false;
                SetPinnedPiece(rowPinned, colPinned, true);
            }
            else if (pin && !PosEmpty(i, col))
            {
                pin = false;
            }

            if (!pin && attack && isEnemyPiece(i, col, color))
            {
                pin = true;
                rowPinned = i;
                colPinned = col;
            }

            attack = false;
        }

        if (!attack && !danger && !pin)
            break;
    }

    attack = true;
    danger = true;
    pin = false;

    for (short i = col + 1; i < 8; i++)
    {
        if (attack)
            SetAttackedSquare(row, i, true);

        if (danger)
            SetkingDangerSquare(row, i, true);

        if (!PosEmpty(row, i))
        {

            if (isEnemyKing(row, i, color))
            {
                if (attack)
                {
                    AddChecker(row, col);
                }
            }
            else
            {
                danger = false;
            }

            if (pin && isEnemyKing(row, i, color))
            {
                pin = false;
                SetPinnedPiece(rowPinned, colPinned, true);
            }
            else if (pin && !PosEmpty(row, i))
            {
                pin = false;
            }

            if (!pin && attack && isEnemyPiece(row, i, color))
            {
                pin = true;
                rowPinned = row;
                colPinned = i;
            }

            attack = false;
        }

        if (!attack && !danger && !pin)
            break;
    }

    attack = true;
    danger = true;
    pin = false;

    for (short i = col - 1; i >= 0; i--)
    {
        if (attack)
            SetAttackedSquare(row, i, true);

        if (danger)
            SetkingDangerSquare(row, i, true);

        if (!PosEmpty(row, i))
        {
            if (isEnemyKing(row, i, color))
            {
                if (attack)
                {
                    AddChecker(row, col);
                }
            }
            else
            {
                danger = false;
            }

            if (pin && isEnemyKing(row, i, color))
            {
                pin = false;
                SetPinnedPiece(rowPinned, colPinned, true);
            }
            else if (pin && !PosEmpty(row, i))
            {
                pin = false;
            }

            if (!pin && attack && isEnemyPiece(row, i, color))
            {
                pin = true;
                rowPinned = row;
                colPinned = i;
            }

            attack = false;
        }

        if (!attack && !danger && !pin)
            break;
    }
}

void MoveGenerator::UpdateKnightDangers(short row, short col, PieceColor color)
{
    if (ValidPos(row + 1, col + 2))
    {
        SetAttackedSquare(row + 1, col + 2, true);
        SetkingDangerSquare(row + 1, col + 2, true);

        if (isEnemyKing(row + 1, col + 2, color))
        {
            AddChecker(row, col);
        }
    }
    if (ValidPos(row + 1, col - 2))
    {
        SetAttackedSquare(row + 1, col - 2, true);
        SetkingDangerSquare(row + 1, col - 2, true);

        if (isEnemyKing(row + 1, col - 2, color))
        {
            AddChecker(row, col);
        }
    }
    if (ValidPos(row - 1, col + 2))
    {
        SetAttackedSquare(row - 1, col + 2, true);
        SetkingDangerSquare(row - 1, col + 2, true);

        if (isEnemyKing(row - 1, col + 2, color))
        {
            AddChecker(row, col);
        }
    }
    if (ValidPos(row - 1, col - 2))
    {
        SetAttackedSquare(row - 1, col - 2, true);
        SetkingDangerSquare(row - 1, col - 2, true);

        if (isEnemyKing(row - 1, col - 2, color))
        {
            AddChecker(row, col);
        }
    }

    if (ValidPos(row + 2, col + 1))
    {
        SetAttackedSquare(row + 2, col + 1, true);
        SetkingDangerSquare(row + 2, col + 1, true);

        if (isEnemyKing(row + 2, col + 1, color))
        {
            AddChecker(row, col);
        }
    }
    if (ValidPos(row + 2, col - 1))
    {
        SetAttackedSquare(row + 2, col - 1, true);
        SetkingDangerSquare(row + 2, col - 1, true);

        if (isEnemyKing(row + 2, col - 1, color))
        {
            AddChecker(row, col);
        }
    }
    if (ValidPos(row - 2, col + 1))
    {
        SetAttackedSquare(row - 2, col + 1, true);
        SetkingDangerSquare(row - 2, col + 1, true);

        if (isEnemyKing(row - 2, col + 1, color))
        {
            AddChecker(row, col);
        }
    }
    if (ValidPos(row - 2, col - 1))
    {
        SetAttackedSquare(row - 2, col - 1, true);
        SetkingDangerSquare(row - 2, col - 1, true);

        if (isEnemyKing(row - 2, col - 1, color))
        {
            AddChecker(row, col);
        }
    }
}

void MoveGenerator::UpdateKingDangers(short row, short col, PieceColor color)
{
    short destRow = 0, destCol = 0;

    for (short i = -1; i <= 1; i++)
    {
        for (short j = -1; j <= 1; j++)
        {
            destRow = row + i;
            destCol = col + j;
            if ((i != 0 || j != 0) && ValidPos(destRow, destCol))
            {
                SetAttackedSquare(destRow, destCol, true);
                SetkingDangerSquare(destRow, destCol, true);
            }
        }
    }
}

void MoveGenerator::UpdateBishopDangers(short row, short col, PieceColor color)
{

    bool danger = true;
    bool attack = true;
    bool pin = false;
    short rowPinned;
    short colPinned;

    short i = row + 1, j = col + 1;
    while (i < 8 && j < 8)
    {
        if (attack)
            SetAttackedSquare(i, j, true);

        if (danger)
            SetkingDangerSquare(i, j, true);

        if (!PosEmpty(i, j))
        {
            if (isEnemyKing(i, j, color))
            {
                if (attack)
                    AddChecker(row, col);
            }
            else
            {
                danger = false;
            }

            if (pin && isEnemyKing(i, j, color))
            {
                pin = false;
                SetPinnedPiece(rowPinned, colPinned, true);
            }
            else if (pin && !PosEmpty(i, j))
            {
                pin = false;
            }

            if (!pin && attack && isEnemyPiece(i, j, color))
            {
                pin = true;
                rowPinned = i;
                colPinned = j;
            }

            attack = false;
        }
        if (!attack && !danger && !pin)
            break;
        i++;
        j++;
    }

    danger = true;
    attack = true;
    pin = false;
    i = row - 1, j = col - 1;
    while (i >= 0 && j >= 0)
    {
        if (attack)
            SetAttackedSquare(i, j, true);

        if (danger)
            SetkingDangerSquare(i, j, true);

        if (!PosEmpty(i, j))
        {

            if (isEnemyKing(i, j, color))
            {
                if (attack)
                    AddChecker(row, col);
            }
            else
            {
                danger = false;
            }

            if (pin && isEnemyKing(i, j, color))
            {
                pin = false;
                SetPinnedPiece(rowPinned, colPinned, true);
            }
            else if (pin && !PosEmpty(i, j))
            {
                pin = false;
            }

            if (!pin && attack && isEnemyPiece(i, j, color))
            {
                pin = true;
                rowPinned = i;
                colPinned = j;
            }

            attack = false;
        }
        if (!attack && !danger && !pin)
            break;
        i--;
        j--;
    }

    danger = true;
    attack = true;
    pin = false;
    i = row + 1, j = col - 1;
    while (i < 8 && j >= 0)
    {
        if (attack)
            SetAttackedSquare(i, j, true);

        if (danger)
            SetkingDangerSquare(i, j, true);

        if (!PosEmpty(i, j))
        {

            if (isEnemyKing(i, j, color))
            {
                if (attack)
                    AddChecker(row, col);
            }
            else
            {
                danger = false;
            }

            if (pin && isEnemyKing(i, j, color))
            {
                pin = false;
                SetPinnedPiece(rowPinned, colPinned, true);
            }
            else if (pin && !PosEmpty(i, j))
            {
                pin = false;
            }

            if (!pin && attack && isEnemyPiece(i, j, color))
            {
                pin = true;
                rowPinned = i;
                colPinned = j;
            }

            attack = false;
        }
        if (!attack && !danger && !pin)
            break;
        i++;
        j--;
    }

    attack = true;
    danger = true;
    pin = false;
    i = row - 1, j = col + 1;
    while (i >= 0 && j < 8)
    {
        if (attack)
            SetAttackedSquare(i, j, true);

        if (danger)
            SetkingDangerSquare(i, j, true);

        if (!PosEmpty(i, j))
        {
            if (isEnemyKing(i, j, color))
            {
                if (attack)
                    AddChecker(row, col);
            }
            else
            {
                danger = false;
            }

            if (pin && isEnemyKing(i, j, color))
            {
                pin = false;
                SetPinnedPiece(rowPinned, colPinned, true);
            }
            else if (pin && !PosEmpty(i, j))
            {
                pin = false;
            }

            if (!pin && attack && isEnemyPiece(i, j, color))
            {
                pin = true;
                rowPinned = i;
                colPinned = j;
            }

            attack = false;
        }
        if (!attack && !danger && !pin)
            break;
        i--;
        j++;
    }
}

void MoveGenerator::UpdateQueenDangers(short row, short col, PieceColor color)
{
    UpdateRookDangers(row, col, color);
    UpdateBishopDangers(row, col, color);
}

void MoveGenerator::GetLegalMoves(MoveArray &moves, int &numMoves)
{
    numLegalMoves = 0;
    legalMoves = &moves;

    attackedSquares.reset();
    kingDangerSquares.reset();
    captureMask.set();
    pushMask.set();
    pinnedPieces.reset();
    checkersNum = 0;

    updateDangers();

    UpdateCheck();

    if (checkersNum >= 2) // when double check only king moves allowed
        return;

    PieceColor color;

    for (short row = 0; row < 8; row++)
    {
        for (short col = 0; col < 8; col++)
        {
            color = pieces[row][col].color;
            if (PosEmpty(row, col) || color != stateInfo.turn)
                continue;

            switch (pieces[row][col].type)
            {
            case PieceType::PAWN:
                UpdatePawnMoves(row, col, color);
                break;
            case PieceType::KNIGHT:
                UpdateKnightMoves(row, col, color);
                break;
            case PieceType::KING:
                UpdateKingMoves(row, col, color);
                UpdateCastleMoves(color);
                break;
            case PieceType::QUEEN:
                UpdateQueenMoves(row, col, color);
                break;
            case PieceType::ROOK:
                UpdateRookMoves(row, col, color);
                break;
            case PieceType::BISHOP:
                UpdateBishopMoves(row, col, color);
                break;
            default:
                break;
            }
        }
    }

    numMoves = numLegalMoves;
}

void MoveGenerator::UpdatePawnMoves(short row, short col, PieceColor color)
{
    short dir = color == PieceColor::WHITE ? +1 : -1;
    short startRow = color == PieceColor::WHITE ? 1 : 6;
    short prePromotionRow = color == PieceColor::WHITE ? 6 : 1;
    short enPassantRow = color == PieceColor::WHITE ? 4 : 3;

    short nextRow = row + dir;
    // pawn push
    if (ValidPos(nextRow, col) && PosEmpty(nextRow, col) && row != prePromotionRow)
    {
        if (GetPushMask(nextRow, col) && IsPinnedPieceLegalMove(row, col, nextRow, col))
            AddLegalMove(Move(row, col, nextRow, col, MoveType::QUIET));
    }

    // diagonal captures
    if (ValidPos(nextRow, col + 1) && CapturablePos(nextRow, col + 1, color) && row != prePromotionRow)
    {
        if (GetCaptureMask(nextRow, col + 1) && IsPinnedPieceLegalMove(row, col, nextRow, col + 1))
            AddLegalMove(Move(row, col, nextRow, col + 1, MoveType::CAPTURE));
    }

    if (ValidPos(nextRow, col - 1) && CapturablePos(nextRow, col - 1, color) && row != prePromotionRow)
    {
        if (GetCaptureMask(nextRow, col - 1) && IsPinnedPieceLegalMove(row, col, nextRow, col - 1))
            AddLegalMove(Move(row, col, nextRow, col - 1, MoveType::CAPTURE));
    }

    if (row == startRow)
    {
        if (PosEmpty(nextRow, col) && PosEmpty(nextRow + dir, col))
        {
            if (GetPushMask(nextRow + dir, col) && IsPinnedPieceLegalMove(row, col, nextRow + dir, col))
                AddLegalMove(Move(row, col, nextRow + dir, col, MoveType::DOUBLEPAWNPUSH));
        }
    }
    else if (row == prePromotionRow)
    {
        if (PosEmpty(nextRow, col) && GetPushMask(nextRow, col))
        {
            if (IsPinnedPieceLegalMove(row, col, nextRow, col))
            {
                AddLegalMove(Move(row, col, nextRow, col, MoveType::QUEENPROMOTION));
                AddLegalMove(Move(row, col, nextRow, col, MoveType::ROOKPROMOTION));
                AddLegalMove(Move(row, col, nextRow, col, MoveType::BISHOPPROMOTION));
                AddLegalMove(Move(row, col, nextRow, col, MoveType::KNIGHTPROMOTION));
            }
        }

        // diagonal captures while promotion
        if (ValidPos(nextRow, col + 1) && CapturablePos(nextRow, col + 1, color) && GetCaptureMask(nextRow, col + 1))
        {
            if (IsPinnedPieceLegalMove(row, col, nextRow, col + 1))
            {
                AddLegalMove(Move(row, col, nextRow, col + 1, MoveType::QUEENPROMOCAPTURE));
                AddLegalMove(Move(row, col, nextRow, col + 1, MoveType::ROOKPROMOCAPTURE));
                AddLegalMove(Move(row, col, nextRow, col + 1, MoveType::BISHOPPROMOCAPTURE));
                AddLegalMove(Move(row, col, nextRow, col + 1, MoveType::KNIGHTPROMOCAPTURE));
            }
        }

        if (ValidPos(nextRow, col - 1) && CapturablePos(nextRow, col - 1, color) && GetCaptureMask(nextRow, col - 1))
        {
            if (IsPinnedPieceLegalMove(row, col, nextRow, col - 1))
            {
                AddLegalMove(Move(row, col, nextRow, col - 1, MoveType::QUEENPROMOCAPTURE));
                AddLegalMove(Move(row, col, nextRow, col - 1, MoveType::ROOKPROMOCAPTURE));
                AddLegalMove(Move(row, col, nextRow, col - 1, MoveType::BISHOPPROMOCAPTURE));
                AddLegalMove(Move(row, col, nextRow, col - 1, MoveType::KNIGHTPROMOCAPTURE));
            }
        }
    }
    else if (row == enPassantRow)
    {
        if (stateInfo.lastMove.type == MoveType::DOUBLEPAWNPUSH && stateInfo.lastMove.endRow == enPassantRow)
        {
            if (stateInfo.lastMove.endCol == col + 1)
            {
                if (GetCaptureMask(row, col + 1) && isValidEnPassant(row, col, nextRow, col + 1))
                    AddLegalMove(Move(row, col, nextRow, col + 1, MoveType::ENPASSANT));
            }
            else if (stateInfo.lastMove.endCol == col - 1)
            {
                if (GetCaptureMask(row, col - 1) && isValidEnPassant(row, col, nextRow, col - 1))
                    AddLegalMove(Move(row, col, nextRow, col - 1, MoveType::ENPASSANT));
            }
        }
    }
}

void MoveGenerator::UpdateKnightMoves(short row, short col, PieceColor color)
{
    short destRow = row + 1, destCol = col + 2;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row + 1, destCol = col - 2;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row - 1, destCol = col + 2;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row - 1, destCol = col - 2;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row + 2, destCol = col + 1;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row + 2, destCol = col - 1;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row - 2, destCol = col + 1;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row - 2, destCol = col - 1;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            AddLegalMove(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }
}

void MoveGenerator::UpdateRookMoves(short row, short col, PieceColor color)
{

    for (short i = row + 1; i < 8; i++)
    {
        if (!IsPinnedPieceLegalMove(row, col, i, col))
            continue;

        if (PosEmpty(i, col))
        {
            if (GetPushMask(i, col))
                AddLegalMove(Move(row, col, i, col, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, col, color) && GetCaptureMask(i, col))
            {
                AddLegalMove(Move(row, col, i, col, MoveType::CAPTURE));
            }
            break;
        }
    }

    for (short i = row - 1; i >= 0; i--)
    {
        if (!IsPinnedPieceLegalMove(row, col, i, col))
            continue;

        if (PosEmpty(i, col))
        {
            if (GetPushMask(i, col))
                AddLegalMove(Move(row, col, i, col, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, col, color) && GetCaptureMask(i, col))
            {
                AddLegalMove(Move(row, col, i, col, MoveType::CAPTURE));
            }
            break;
        }
    }

    for (short i = col + 1; i < 8; i++)
    {
        if (!IsPinnedPieceLegalMove(row, col, row, i))
            continue;

        if (PosEmpty(row, i))
        {
            if (GetPushMask(row, i))
                AddLegalMove(Move(row, col, row, i, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(row, i, color) && GetCaptureMask(row, i))
            {
                AddLegalMove(Move(row, col, row, i, MoveType::CAPTURE));
            }
            break;
        }
    }

    for (short i = col - 1; i >= 0; i--)
    {
        if (!IsPinnedPieceLegalMove(row, col, row, i))
            continue;

        if (PosEmpty(row, i))
        {
            if (GetPushMask(row, i))
                AddLegalMove(Move(row, col, row, i, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(row, i, color) && GetCaptureMask(row, i))
            {
                AddLegalMove(Move(row, col, row, i, MoveType::CAPTURE));
            }
            break;
        }
    }
}

void MoveGenerator::UpdateKingMoves(short row, short col, PieceColor color)
{
    short destRow = 0, destCol = 0;
    for (short i = -1; i <= 1; i++)
    {
        for (short j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            destRow = row + i;
            destCol = col + j;

            if (ValidPos(destRow, destCol) && !GetkingDangerSquare(destRow, destCol))
            {

                if (PosEmpty(destRow, destCol))
                {
                    AddLegalMove(Move(row, col, destRow, destCol, MoveType::QUIET));
                }
                else if (CapturablePos(destRow, destCol, color))
                {
                    AddLegalMove(Move(row, col, destRow, destCol, MoveType::CAPTURE));
                }
            }
        }
    }
}

void MoveGenerator::UpdateBishopMoves(short row, short col, PieceColor color)
{

    short i = row + 1, j = col + 1;
    while (i < 8 && j < 8)
    {
        if (!ValidPos(i, j) || !IsPinnedPieceLegalMove(row, col, i, j))
            break;

        if (PosEmpty(i, j))
        {
            if (GetPushMask(i, j))
                AddLegalMove(Move(row, col, i, j, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, j, color) && GetCaptureMask(i, j))
            {
                AddLegalMove(Move(row, col, i, j, MoveType::CAPTURE));
            }
            break;
        }
        i++;
        j++;
    }

    i = row - 1, j = col - 1;
    while (i >= 0 && j >= 0)
    {
        if (!ValidPos(i, j) || !IsPinnedPieceLegalMove(row, col, i, j))
            break;

        if (PosEmpty(i, j))
        {
            if (GetPushMask(i, j))
                AddLegalMove(Move(row, col, i, j, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, j, color) && GetCaptureMask(i, j))
            {
                AddLegalMove(Move(row, col, i, j, MoveType::CAPTURE));
            }
            break;
        }
        i--;
        j--;
    }

    i = row + 1, j = col - 1;
    while (i < 8 && j >= 0)
    {
        if (!ValidPos(i, j) || !IsPinnedPieceLegalMove(row, col, i, j))
            break;

        if (PosEmpty(i, j))
        {
            if (GetPushMask(i, j))
                AddLegalMove(Move(row, col, i, j, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, j, color) && GetCaptureMask(i, j))
            {
                AddLegalMove(Move(row, col, i, j, MoveType::CAPTURE));
            }
            break;
        }
        i++;
        j--;
    }

    i = row - 1, j = col + 1;
    while (i >= 0 && j < 8)
    {
        if (!ValidPos(i, j) || !IsPinnedPieceLegalMove(row, col, i, j))
            break;

        if (PosEmpty(i, j))
        {
            if (GetPushMask(i, j))
                AddLegalMove(Move(row, col, i, j, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, j, color) && GetCaptureMask(i, j))
            {
                AddLegalMove(Move(row, col, i, j, MoveType::CAPTURE));
            }
            break;
        }
        i--;
        j++;
    }
}

void MoveGenerator::UpdateQueenMoves(short row, short col, PieceColor color)
{
    UpdateRookMoves(row, col, color);
    UpdateBishopMoves(row, col, color);
}

void MoveGenerator::UpdateCastleMoves(PieceColor color)
{
    if (checkersNum > 0) // check
    {
        return;
    }

    if (color == PieceColor::WHITE && (stateInfo.kingMovedW || pieces[0][4] != PieceInfo{PieceType::KING, color}))
    {
        return;
    }

    if (color == PieceColor::BLACK && (stateInfo.kingMovedB || pieces[7][4] != PieceInfo{PieceType::KING, color}))
    {
        return;
    }

    if (color == PieceColor::WHITE)
    {
        if (!stateInfo.rookKMovedW && pieces[0][7] == PieceInfo{PieceType::ROOK, color})
        {
            if (PosEmpty(0, 5) && PosEmpty(0, 6))
            {
                if (!GetkingDangerSquare(0, 5) && !GetkingDangerSquare(0, 6))
                {
                    AddLegalMove(Move(stateInfo.kingRowW, stateInfo.kingColW, 0, 6, MoveType::KINGCASTLE));
                }
            }
        }

        if (!stateInfo.rookQMovedW && pieces[0][0] == PieceInfo{PieceType::ROOK, color})
        {
            if (PosEmpty(0, 1) && PosEmpty(0, 2) && PosEmpty(0, 3))
            {
                if (!GetkingDangerSquare(0, 2) && !GetkingDangerSquare(0, 3))
                {
                    AddLegalMove(Move(stateInfo.kingRowW, stateInfo.kingColW, 0, 2, MoveType::QUEENCASTLE));
                }
            }
        }
    }
    else
    {
        if (!stateInfo.rookKMovedB && pieces[7][7] == PieceInfo{PieceType::ROOK, color})
        {
            if (PosEmpty(7, 5) && PosEmpty(7, 6))
            {
                if (!GetkingDangerSquare(7, 5) && !GetkingDangerSquare(7, 6))
                {
                    AddLegalMove(Move(stateInfo.kingRowB, stateInfo.kingColB, 7, 6, MoveType::KINGCASTLE));
                }
            }
        }

        if (!stateInfo.rookQMovedB && pieces[7][0] == PieceInfo{PieceType::ROOK, color})
        {
            if (PosEmpty(7, 1) && PosEmpty(7, 2) && PosEmpty(7, 3))
            {
                if (!GetkingDangerSquare(7, 2) && !GetkingDangerSquare(7, 3))
                {
                    AddLegalMove(Move(stateInfo.kingRowB, stateInfo.kingColB, 7, 2, MoveType::QUEENCASTLE));
                }
            }
        }
    }
}

bool MoveGenerator::IsSlider(short row, short col) const
{
    switch (pieces[row][col].type)
    {
    case PieceType::BISHOP:
        return true;
        break;
    case PieceType::ROOK:
        return true;
        break;
    case PieceType::QUEEN:
        return true;
        break;
    case PieceType::KNIGHT:
        return false;
        break;
    case PieceType::PAWN:
        return false;
        break;
    case PieceType::KING:
        return false;
        break;
    default:
        return false;
        break;
    }
}

void MoveGenerator::UpdateCheck()
{

    short kingRow = stateInfo.turn == PieceColor::WHITE ? stateInfo.kingRowW : stateInfo.kingRowB;
    short kingCol = stateInfo.turn == PieceColor::WHITE ? stateInfo.kingColW : stateInfo.kingColB;

    if (checkersNum == 1)
    {
        // single check

        captureMask.reset();
        pushMask.reset();

        SetCaptureMask(checkerRow, checkerCol, true);

        if (IsSlider(checkerRow, checkerCol))
        {
            short diri = kingRow < checkerRow ? +1 : -1;
            short dirj = kingCol < checkerCol ? +1 : -1;

            short i = kingRow, j = kingCol;

            while (i != checkerRow || j != checkerCol)
            {
                if ((i != kingRow || j != kingCol) && (i != checkerRow || j != checkerCol))
                    SetPushMask(i, j, true);

                if (i != checkerRow)
                    i += diri;

                if (j != checkerCol)
                    j += dirj;
            }
        }
    }
    else if (checkersNum >= 2)
    {
        // double check, only king moves allowed

        UpdateKingMoves(kingRow, kingCol, stateInfo.turn);
    }
}

bool MoveGenerator::IsPinnedPieceLegalMove(short pieceRow, short pieceCol, short destRow, short destCol) const
{

    if (!GetPinnedPiece(pieceRow, pieceCol))
        return true; // piece is not pinned

    bool isLegal = false;

    PieceColor pieceColor = pieces[pieceRow][pieceCol].color;
    short kingRow = pieceColor == PieceColor::WHITE ? stateInfo.kingRowW : stateInfo.kingRowB;
    short kingCol = pieceColor == PieceColor::WHITE ? stateInfo.kingColW : stateInfo.kingColB;

    if (pieceRow == kingRow) // horizontal pin
    {
        if (destRow == pieceRow)
        {
            isLegal = true;
        }
    }
    else if (pieceCol == kingCol) // vertical pin
    {
        if (destCol == pieceCol)
        {
            isLegal = true;
        }
    }
    else // diagonal pin
    {

        // Calculate the slope of the line
        int numerator = (kingCol - pieceCol) * (destRow - pieceRow);
        int denominator = (kingRow - pieceRow);

        // Check if the y-coordinate of the destPos on the line matches the y-coordinate of the destPos
        isLegal = (numerator % denominator == 0) && (numerator == (destCol - pieceCol) * denominator);
    }

    return isLegal;
}

bool MoveGenerator::isValidEnPassant(short pieceRow, short pieceCol, short destRow, short destCol) const
{
    if (!IsPinnedPieceLegalMove(pieceRow, pieceCol, destRow, destCol))
    {
        return false;
    }

    bool validMove = true;

    PieceColor pieceColor = pieces[pieceRow][pieceCol].color;

    short kingRow = pieceColor == PieceColor::WHITE ? stateInfo.kingRowW : stateInfo.kingRowB;
    short kingCol = pieceColor == PieceColor::WHITE ? stateInfo.kingColW : stateInfo.kingColB;
    short enPassantRow = pieceColor == PieceColor::WHITE ? 4 : 3;

    if (kingRow == enPassantRow)
    {
        short dir = kingCol < pieceCol ? +1 : -1;
        int i = kingCol + dir;
        bool stop = false;
        while (!stop && ValidPos(kingRow, i))
        {
            if (i != destCol && i != pieceCol)
            {
                if (!PosEmpty(kingRow, i))
                {

                    validMove = !(isEnemyPiece(kingRow, i, pieceColor) && (pieces[kingRow][i].type == PieceType::QUEEN ||
                                                                           pieces[kingRow][i].type == PieceType::ROOK));

                    stop = true;
                }
            }
            i += dir;
        }
    }

    return validMove;
}
