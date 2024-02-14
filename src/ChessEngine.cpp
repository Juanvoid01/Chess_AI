#include "ChessEngine.hpp"
#include <iostream>

ChessEngine::ChessEngine()
{

    legalMoves.reserve(MAX_THEORETICAL_MOVES_PER_POSITION);
    SetInitialPosition();
    updateLegalMoves();
}

void ChessEngine::SetInitialPosition()
{
    for (int row = 2; row < 6; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            pieces[row][col] = {PieceType::EMPTY, PieceColor::WHITE};
        }
    }

    for (int col = 0; col < 8; col++)
    {
        pieces[1][col] = {PieceType::PAWN, PieceColor::WHITE};
        pieces[6][col] = {PieceType::PAWN, PieceColor::BLACK};
    }

    pieces[0][0] = {PieceType::ROOK, PieceColor::WHITE};
    pieces[0][1] = {PieceType::KNIGHT, PieceColor::WHITE};
    pieces[0][2] = {PieceType::BISHOP, PieceColor::WHITE};
    pieces[0][3] = {PieceType::QUEEN, PieceColor::WHITE};
    pieces[0][4] = {PieceType::KING, PieceColor::WHITE};
    pieces[0][5] = {PieceType::BISHOP, PieceColor::WHITE};
    pieces[0][6] = {PieceType::KNIGHT, PieceColor::WHITE};
    pieces[0][7] = {PieceType::ROOK, PieceColor::WHITE};

    pieces[7][0] = {PieceType::ROOK, PieceColor::BLACK};
    pieces[7][1] = {PieceType::KNIGHT, PieceColor::BLACK};
    pieces[7][2] = {PieceType::BISHOP, PieceColor::BLACK};
    pieces[7][3] = {PieceType::QUEEN, PieceColor::BLACK};
    pieces[7][4] = {PieceType::KING, PieceColor::BLACK};
    pieces[7][5] = {PieceType::BISHOP, PieceColor::BLACK};
    pieces[7][6] = {PieceType::KNIGHT, PieceColor::BLACK};
    pieces[7][7] = {PieceType::ROOK, PieceColor::BLACK};
};

void ChessEngine::ClearBoard()
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            ClearPos(row, col);
        }
    }
}

void ChessEngine::MakeMove(Move move)
{
    pieces[move.endRow][move.endCol] = pieces[move.iniRow][move.iniCol];
    ClearPos(move.iniRow, move.iniCol);

    turn = turn == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE;
    updateLegalMoves();
}

void ChessEngine::updateDangers()
{

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (PosEmpty(row, col) || pieces[row][col].color == turn)
                continue;

            switch (pieces[row][col].type)
            {
            case PieceType::PAWN:
                UpdatePawnDangers(row, col, pieces[row][col].color);
                break;
            case PieceType::KNIGHT:
                UpdateKnightDangers(row, col, pieces[row][col].color);
                break;
            case PieceType::KING:
                UpdateKingDangers(row, col, pieces[row][col].color);
                break;
            case PieceType::QUEEN:
                UpdateQueenDangers(row, col, pieces[row][col].color);
                break;
            case PieceType::ROOK:
                UpdateRookDangers(row, col, pieces[row][col].color);
                break;
            case PieceType::BISHOP:
                UpdateBishopDangers(row, col, pieces[row][col].color);
                break;
            default:
                break;
            }
        }
    }
};

void ChessEngine::UpdatePawnDangers(int row, int col, PieceColor color)
{
    short dir = color == PieceColor::WHITE ? 1 : -1;

    short nextRow = row + dir;

    if (ValidPos(nextRow, col + 1))
    {
        SetAttackedSquare(nextRow, col + 1, true);
        SetkingDangerSquare(nextRow, col + 1, true);
    }

    if (ValidPos(nextRow, col - 1))
    {
        SetAttackedSquare(nextRow, col - 1, true);
        SetkingDangerSquare(nextRow, col - 1, true);
    }
}

void ChessEngine::UpdateRookDangers(int row, int col, PieceColor color)
{
    bool attack = true;
    bool danger = true;

    for (int i = row + 1; i < 8; i++)
    {
        if (attack)
            SetAttackedSquare(i, col, true);

        if (danger)
            SetkingDangerSquare(i, col, true);

        if (!PosEmpty(i, col))
        {
            attack = false;
            if (!isEnemyKing(i, col, color))
            {
                danger = false;
            }
        }

        if (!attack && !danger)
            break;
    }

    attack = true;
    danger = true;

    for (int i = row - 1; i >= 0; i--)
    {
        if (attack)
            SetAttackedSquare(i, col, true);

        if (danger)
            SetkingDangerSquare(i, col, true);

        if (!PosEmpty(i, col))
        {
            attack = false;
            if (!isEnemyKing(i, col, color))
            {
                danger = false;
            }
        }

        if (!attack && !danger)
            break;
    }

    attack = true;
    danger = true;

    for (int i = col + 1; i < 8; i++)
    {
        if (attack)
            SetAttackedSquare(row, i, true);

        if (danger)
            SetkingDangerSquare(row, i, true);

        if (!PosEmpty(i, col))
        {
            attack = false;
            if (!isEnemyKing(row, i, color))
            {
                danger = false;
            }
        }

        if (!attack && !danger)
            break;
    }

    attack = true;
    danger = true;

    for (int i = col - 1; i >= 0; i--)
    {
        if (attack)
            SetAttackedSquare(row, i, true);

        if (danger)
            SetkingDangerSquare(row, i, true);

        if (!PosEmpty(i, col))
        {
            attack = false;
            if (!isEnemyKing(row, i, color))
            {
                danger = false;
            }
        }

        if (!attack && !danger)
            break;
    }
}

void ChessEngine::UpdateKnightDangers(int row, int col, PieceColor color)
{
    if (ValidPos(row + 1, col + 2))
    {
        SetAttackedSquare(row + 1, col + 2, true);
        SetkingDangerSquare(row + 1, col + 2, true);
    }
    if (ValidPos(row + 1, col - 2))
    {
        SetAttackedSquare(row + 1, col - 2, true);
        SetkingDangerSquare(row + 1, col - 2, true);
    }
    if (ValidPos(row - 1, col + 2))
    {
        SetAttackedSquare(row - 1, col + 2, true);
        SetkingDangerSquare(row - 1, col + 2, true);
    }
    if (ValidPos(row - 1, col - 2))
    {
        SetAttackedSquare(row - 1, col - 2, true);
        SetkingDangerSquare(row - 1, col - 2, true);
    }

    if (ValidPos(row + 2, col + 1))
    {
        SetAttackedSquare(row + 2, col + 1, true);
        SetkingDangerSquare(row + 2, col + 1, true);
    }
    if (ValidPos(row + 2, col - 1))
    {
        SetAttackedSquare(row + 2, col - 1, true);
        SetkingDangerSquare(row + 2, col - 1, true);
    }
    if (ValidPos(row - 2, col + 1))
    {
        SetAttackedSquare(row - 2, col + 1, true);
        SetkingDangerSquare(row - 2, col + 1, true);
    }
    if (ValidPos(row - 2, col - 1))
    {
        SetAttackedSquare(row - 2, col - 1, true);
        SetkingDangerSquare(row - 2, col - 1, true);
    }
}

void ChessEngine::UpdateKingDangers(int row, int col, PieceColor color)
{
    int destRow = 0, destCol = 0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
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

void ChessEngine::UpdateBishopDangers(int row, int col, PieceColor color)
{

    bool attack = true;
    int i = row + 1, j = col + 1;
    while (i < 8 && j < 8)
    {
        if (attack)
        {
            SetAttackedSquare(i, j, true);
        }

        SetkingDangerSquare(i, j, true);

        if (attack && !PosEmpty(i, j))
        {
            attack = false;
        }
        i++;
        j++;
    }

    attack = true;
    i = row - 1, j = col - 1;
    while (i >= 0 && j >= 0)
    {
        if (attack)
        {
            SetAttackedSquare(i, j, true);
        }

        SetkingDangerSquare(i, j, true);

        if (attack && !PosEmpty(i, j))
        {
            attack = false;
        }
        i--;
        j--;
    }

    attack = true;
    i = row + 1, j = col - 1;
    while (i < 8 && j >= 0)
    {
        if (attack)
        {
            SetAttackedSquare(i, j, true);
        }

        SetkingDangerSquare(i, j, true);

        if (attack && !PosEmpty(i, j))
        {
            attack = false;
        }
        i++;
        j--;
    }

    attack = true;
    i = row - 1, j = col + 1;
    while (i >= 0 && j < 8)
    {
        if (attack)
        {
            SetAttackedSquare(i, j, true);
        }

        SetkingDangerSquare(i, j, true);

        if (attack && !PosEmpty(i, j))
        {
            attack = false;
        }
        i--;
        j++;
    }
}

void ChessEngine::UpdateQueenDangers(int row, int col, PieceColor color)
{
    UpdateRookDangers(row, col, color);
    UpdateBishopDangers(row, col, color);
}

void ChessEngine::updateLegalMoves()
{
    legalMoves.clear();
    attackedSquares.reset();
    kingDangerSquares.reset();

    updateDangers();

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (PosEmpty(row, col) || pieces[row][col].color != turn)
                continue;

            switch (pieces[row][col].type)
            {
            case PieceType::PAWN:
                UpdatePawnMoves(row, col, pieces[row][col].color);
                break;
            case PieceType::KNIGHT:
                UpdateKnightMoves(row, col, pieces[row][col].color);
                break;
            case PieceType::KING:
                UpdateKingMoves(row, col, pieces[row][col].color);
                break;
            case PieceType::QUEEN:
                UpdateQueenMoves(row, col, pieces[row][col].color);
                break;
            case PieceType::ROOK:
                UpdateRookMoves(row, col, pieces[row][col].color);
                break;
            case PieceType::BISHOP:
                UpdateBishopMoves(row, col, pieces[row][col].color);
                break;
            default:
                break;
            }
        }
    }
}

void ChessEngine::UpdatePawnMoves(int row, int col, PieceColor color)
{
    short dir = color == PieceColor::WHITE ? +1 : -1;
    short startRow = color == PieceColor::WHITE ? 1 : 6;
    short prePromotionRow = color == PieceColor::WHITE ? 6 : 1;
    short enPassantRow = color == PieceColor::WHITE ? 5 : 4;

    short nextRow = row + dir;
    // pawn push
    if (ValidPos(nextRow, col) && PosEmpty(nextRow, col) && row != prePromotionRow)
    {
        legalMoves.emplace_back(Move(row, col, nextRow, col, MoveType::QUIET));
    }

    // diagonal captures
    if (ValidPos(nextRow, col + 1) && CapturablePos(nextRow, col + 1, color))
    {
        legalMoves.emplace_back(Move(row, col, nextRow, col + 1, MoveType::CAPTURE));
    }

    if (ValidPos(nextRow, col - 1) && CapturablePos(nextRow, col - 1, color))
    {
        legalMoves.emplace_back(Move(row, col, nextRow, col - 1, MoveType::CAPTURE));
    }

    if (row == startRow)
    {

        if (PosEmpty(nextRow, col) && PosEmpty(nextRow + dir, col))
        {
            legalMoves.emplace_back(Move(row, col, nextRow + dir, col, MoveType::DOUBLEPAWNPUSH));
        }
    }
    else if (row == prePromotionRow)
    {
        if (PosEmpty(nextRow, col))
        {
            legalMoves.emplace_back(Move(row, col, nextRow, col, MoveType::QUEENPROMOTION));
            legalMoves.emplace_back(Move(row, col, nextRow, col, MoveType::ROOKPROMOTION));
            legalMoves.emplace_back(Move(row, col, nextRow, col, MoveType::BISHOPPROMOTION));
            legalMoves.emplace_back(Move(row, col, nextRow, col, MoveType::KNIGHTPROMOTION));
        }

        // diagonal captures while promotion
        if (ValidPos(nextRow, col + 1) && CapturablePos(nextRow, col + 1, color))
        {
            legalMoves.emplace_back(Move(row, col, nextRow, col + 1, MoveType::QUEENPROMOTION));
            legalMoves.emplace_back(Move(row, col, nextRow, col + 1, MoveType::ROOKPROMOTION));
            legalMoves.emplace_back(Move(row, col, nextRow, col + 1, MoveType::BISHOPPROMOTION));
            legalMoves.emplace_back(Move(row, col, nextRow, col + 1, MoveType::KNIGHTPROMOTION));
        }

        if (ValidPos(nextRow, col - 1) && CapturablePos(nextRow, col - 1, color))
        {
            legalMoves.emplace_back(Move(row, col, nextRow, col - 1, MoveType::QUEENPROMOCAPTURE));
            legalMoves.emplace_back(Move(row, col, nextRow, col - 1, MoveType::ROOKPROMOCAPTURE));
            legalMoves.emplace_back(Move(row, col, nextRow, col - 1, MoveType::BISHOPPROMOCAPTURE));
            legalMoves.emplace_back(Move(row, col, nextRow, col - 1, MoveType::KNIGHTPROMOCAPTURE));
        }
    }
    else if (row == enPassantRow)
    {
    }
}

void ChessEngine::UpdateKnightMoves(int row, int col, PieceColor color)
{
    int destRow = row + 1, destCol = col + 2;

    if (ValidPos(destRow, destCol))
    {
        if (PosEmpty(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row + 1, destCol = col - 2;
    if (ValidPos(destRow, destCol))
    {
        if (PosEmpty(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row - 1, destCol = col + 2;
    if (ValidPos(destRow, destCol))
    {
        if (PosEmpty(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row - 1, destCol = col - 2;
    if (ValidPos(destRow, destCol))
    {
        if (PosEmpty(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row + 2, destCol = col + 1;
    if (ValidPos(destRow, destCol))
    {
        if (PosEmpty(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row + 2, destCol = col - 1;
    if (ValidPos(destRow, destCol))
    {
        if (PosEmpty(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row - 2, destCol = col + 1;
    if (ValidPos(destRow, destCol))
    {
        if (PosEmpty(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row - 2, destCol = col - 1;
    if (ValidPos(destRow, destCol))
    {
        if (PosEmpty(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }
}

void ChessEngine::UpdateRookMoves(int row, int col, PieceColor color)
{

    for (int i = row + 1; i < 8; i++)
    {
        if (PosEmpty(i, col))
        {
            legalMoves.emplace_back(Move(row, col, i, col, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, col, color))
            {
                legalMoves.emplace_back(Move(row, col, i, col, MoveType::CAPTURE));
            }
            break;
        }
    }

    for (int i = row - 1; i >= 0; i--)
    {
        if (PosEmpty(i, col))
        {
            legalMoves.emplace_back(Move(row, col, i, col, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, col, color))
            {
                legalMoves.emplace_back(Move(row, col, i, col, MoveType::CAPTURE));
            }
            break;
        }
    }

    for (int i = col + 1; i < 8; i++)
    {
        if (PosEmpty(row, i))
        {
            legalMoves.emplace_back(Move(row, col, row, i, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(row, i, color))
            {
                legalMoves.emplace_back(Move(row, col, row, i, MoveType::CAPTURE));
            }
            break;
        }
    }

    for (int i = col - 1; i >= 0; i--)
    {
        if (PosEmpty(row, i))
        {
            legalMoves.emplace_back(Move(row, col, row, i, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(row, i, color))
            {
                legalMoves.emplace_back(Move(row, col, row, i, MoveType::CAPTURE));
            }
            break;
        }
    }
}

void ChessEngine::UpdateKingMoves(int row, int col, PieceColor color)
{
    int destRow = 0, destCol = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            destRow = row + i;
            destCol = col + j;

            if (ValidPos(destRow, destCol) && !GetkingDangerSquare(destRow, destCol))
            {

                if (PosEmpty(destRow, destCol))
                {
                    legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
                }
                else if (CapturablePos(destRow, destCol, color))
                {
                    legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
                }
            }
        }
    }
}

void ChessEngine::UpdateBishopMoves(int row, int col, PieceColor color)
{

    int i = row + 1, j = col + 1;
    while (i < 8 && j < 8)
    {
        if (!ValidPos(i, j))
            break;

        if (PosEmpty(i, j))
        {
            legalMoves.emplace_back(Move(row, col, i, j, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, j, color))
            {
                legalMoves.emplace_back(Move(row, col, i, j, MoveType::CAPTURE));
            }
            break;
        }
        i++;
        j++;
    }

    i = row - 1, j = col - 1;
    while (i >= 0 && j >= 0)
    {
        if (!ValidPos(i, j))
            break;

        if (PosEmpty(i, j))
        {
            legalMoves.emplace_back(Move(row, col, i, j, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, j, color))
            {
                legalMoves.emplace_back(Move(row, col, i, j, MoveType::CAPTURE));
            }
            break;
        }
        i--;
        j--;
    }

    i = row + 1, j = col - 1;
    while (i < 8 && j >= 0)
    {
        if (!ValidPos(i, j))
            break;

        if (PosEmpty(i, j))
        {
            legalMoves.emplace_back(Move(row, col, i, j, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, j, color))
            {
                legalMoves.emplace_back(Move(row, col, i, j, MoveType::CAPTURE));
            }
            break;
        }
        i++;
        j--;
    }

    i = row - 1, j = col + 1;
    while (i >= 0 && j < 8)
    {
        if (!ValidPos(i, j))
            break;

        if (PosEmpty(i, j))
        {
            legalMoves.emplace_back(Move(row, col, i, j, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, j, color))
            {
                legalMoves.emplace_back(Move(row, col, i, j, MoveType::CAPTURE));
            }
            break;
        }
        i--;
        j++;
    }
}

void ChessEngine::UpdateQueenMoves(int row, int col, PieceColor color)
{
    UpdateRookMoves(row, col, color);
    UpdateBishopMoves(row, col, color);
}
