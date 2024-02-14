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

    if (ValidPos(row + dir, col + 1))
    {
        SetkingAttackedSquare(row + dir, col + 1, true);
        SetkingDangerSquare(row + dir, col + 1, true);
    }

    if (ValidPos(row + dir, col - 1))
    {
        SetkingAttackedSquare(row + dir, col - 1, true);
        SetkingDangerSquare(row + dir, col - 1, true);
    }
}

void ChessEngine::UpdateRookDangers(int row, int col, PieceColor color)
{
    bool attack = true;

    for (int i = row + 1; i < 8; i++)
    {
        if (attack)
        {
            SetkingAttackedSquare(i, col, true);
        }

        SetkingDangerSquare(i, col, true);

        if (attack && !PosEmpty(i, col))
        {
            attack = false;
        }
    }

    attack = true;

    for (int i = row - 1; i >= 0; i--)
    {
        if (attack)
        {
            SetkingAttackedSquare(i, col, true);
        }

        SetkingDangerSquare(i, col, true);

        if (attack && !PosEmpty(i, col))
        {
            attack = false;
        }
    }

    attack = true;

    for (int i = col + 1; i < 8; i++)
    {
        if (attack)
        {
            SetkingAttackedSquare(row, i, true);
        }

        SetkingDangerSquare(row, i, true);

        if (attack && !PosEmpty(row, i))
        {
            attack = false;
        }
    }

    attack = true;

    for (int i = col - 1; i >= 0; i--)
    {
        if (attack)
        {
            SetkingAttackedSquare(row, i, true);
        }

        SetkingDangerSquare(row, i, true);

        if (attack && !PosEmpty(row, i))
        {
            attack = false;
        }
    }
}

void ChessEngine::UpdateKnightDangers(int row, int col, PieceColor color)
{
    if (ValidPos(row + 1, col + 2))
    {
        SetkingAttackedSquare(row + 1, col + 2, true);
        SetkingDangerSquare(row + 1, col + 2, true);
    }
    if (ValidPos(row + 1, col - 2))
    {
        SetkingAttackedSquare(row + 1, col - 2, true);
        SetkingDangerSquare(row + 1, col - 2, true);
    }
    if (ValidPos(row - 1, col + 2))
    {
        SetkingAttackedSquare(row - 1, col + 2, true);
        SetkingDangerSquare(row - 1, col + 2, true);
    }
    if (ValidPos(row - 1, col - 2))
    {
        SetkingAttackedSquare(row - 1, col - 2, true);
        SetkingDangerSquare(row - 1, col - 2, true);
    }

    if (ValidPos(row + 2, col + 1))
    {
        SetkingAttackedSquare(row + 2, col + 1, true);
        SetkingDangerSquare(row + 2, col + 1, true);
    }
    if (ValidPos(row + 2, col - 1))
    {
        SetkingAttackedSquare(row + 2, col - 1, true);
        SetkingDangerSquare(row + 2, col - 1, true);
    }
    if (ValidPos(row - 2, col + 1))
    {
        SetkingAttackedSquare(row - 2, col + 1, true);
        SetkingDangerSquare(row - 2, col + 1, true);
    }
    if (ValidPos(row - 2, col - 1))
    {
        SetkingAttackedSquare(row - 2, col - 1, true);
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
                SetkingAttackedSquare(destRow, destCol, true);
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
            SetkingAttackedSquare(i, j, true);
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
            SetkingAttackedSquare(i, j, true);
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
            SetkingAttackedSquare(i, j, true);
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
            SetkingAttackedSquare(i, j, true);
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
    kingAttackedSquares.reset();
    kingDangerSquares.reset();

    updateDangers();

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (PosEmpty(row, col) || pieces[row][col].color != turn)
                continue;

            UpdateKingMoves(row, col, pieces[row][col].color);
            /*switch (pieces[row][col].type)
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
            }*/
        }
    }
}

void ChessEngine::UpdatePawnMoves(int row, int col, PieceColor color)
{
}
void ChessEngine::UpdateKnightMoves(int row, int col, PieceColor color)
{
}
void ChessEngine::UpdateRookMoves(int row, int col, PieceColor color)
{
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

            if (ValidPos(destRow, destCol) && !GetkingAttackedSquare(destRow, destCol))
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
}
void ChessEngine::UpdateQueenMoves(int row, int col, PieceColor color)
{
}
