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
    kingRowW = 0;
    kingColW = 4;
    pieces[0][5] = {PieceType::BISHOP, PieceColor::WHITE};
    pieces[0][6] = {PieceType::KNIGHT, PieceColor::WHITE};
    pieces[0][7] = {PieceType::ROOK, PieceColor::WHITE};

    pieces[7][0] = {PieceType::ROOK, PieceColor::BLACK};
    pieces[7][1] = {PieceType::KNIGHT, PieceColor::BLACK};
    pieces[7][2] = {PieceType::BISHOP, PieceColor::BLACK};
    pieces[7][3] = {PieceType::QUEEN, PieceColor::BLACK};
    pieces[7][4] = {PieceType::KING, PieceColor::BLACK};
    kingRowW = 7;
    kingColW = 4;

    pieces[7][5] = {PieceType::BISHOP, PieceColor::BLACK};
    pieces[7][6] = {PieceType::KNIGHT, PieceColor::BLACK};
    pieces[7][7] = {PieceType::ROOK, PieceColor::BLACK};
};

void ChessEngine::ClearBoard()
{
    for (short row = 0; row < 8; row++)
    {
        for (short col = 0; col < 8; col++)
        {
            ClearPos(row, col);
        }
    }
}

void ChessEngine::MakeMove(Move move)
{
    pieces[move.endRow][move.endCol] = pieces[move.iniRow][move.iniCol];
    ClearPos(move.iniRow, move.iniCol);

    if (turn == PieceColor::WHITE)
    {
        if (pieces[move.endRow][move.endCol].type == PieceType::KING)
        {
            kingRowW = move.endRow;
            kingColW = move.endCol;
        }
        turn = PieceColor::BLACK;
    }
    else
    {
        if (pieces[move.endRow][move.endCol].type == PieceType::KING)
        {
            kingRowB = move.endRow;
            kingColB = move.endCol;
        }
        turn = PieceColor::WHITE;
    }

    updateLegalMoves();
    lastMove = move;
}

bool ChessEngine::IsCheckMate() const
{
    return legalMoves.size() == 0 && checkersNum > 0;
}

bool ChessEngine::IsStaleMate() const
{
    return legalMoves.size() == 0 && checkersNum == 0;
}

void ChessEngine::updateDangers()
{

    for (short row = 0; row < 8; row++)
    {
        for (short col = 0; col < 8; col++)
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

void ChessEngine::UpdatePawnDangers(short row, short col, PieceColor color)
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

void ChessEngine::UpdateRookDangers(short row, short col, PieceColor color)
{
    bool attack = true;
    bool danger = true;

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
            attack = false;
        }

        if (!attack && !danger)
            break;
    }

    attack = true;
    danger = true;

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
            attack = false;
        }

        if (!attack && !danger)
            break;
    }

    attack = true;
    danger = true;

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
            attack = false;
        }

        if (!attack && !danger)
            break;
    }

    attack = true;
    danger = true;

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
            attack = false;
        }

        if (!attack && !danger)
            break;
    }
}

void ChessEngine::UpdateKnightDangers(short row, short col, PieceColor color)
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

void ChessEngine::UpdateKingDangers(short row, short col, PieceColor color)
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

void ChessEngine::UpdateBishopDangers(short row, short col, PieceColor color)
{

    bool danger = true;
    bool attack = true;
    short i = row + 1, j = col + 1;
    while (i < 8 && j < 8)
    {
        if (attack)
            SetAttackedSquare(i, j, true);

        if (danger)
            SetkingDangerSquare(i, j, true);

        if (!PosEmpty(i, j))
        {
            attack = false;
            if (isEnemyKing(i, j, color))
            {
                AddChecker(row, col);
            }
            else
            {
                danger = false;
            }
        }
        if (!attack && !danger)
            break;
        i++;
        j++;
    }

    danger = true;
    attack = true;
    i = row - 1, j = col - 1;
    while (i >= 0 && j >= 0)
    {
        if (attack)
            SetAttackedSquare(i, j, true);

        if (danger)
            SetkingDangerSquare(i, j, true);

        if (!PosEmpty(i, j))
        {
            attack = false;
            if (isEnemyKing(i, j, color))
            {
                AddChecker(row, col);
            }
            else
            {
                danger = false;
            }
        }
        if (!attack && !danger)
            break;
        i--;
        j--;
    }

    danger = true;
    attack = true;
    i = row + 1, j = col - 1;
    while (i < 8 && j >= 0)
    {
        if (attack)
            SetAttackedSquare(i, j, true);

        if (danger)
            SetkingDangerSquare(i, j, true);

        if (!PosEmpty(i, j))
        {
            attack = false;
            if (isEnemyKing(i, j, color))
            {
                AddChecker(row, col);
            }
            else
            {
                danger = false;
            }
        }
        if (!attack && !danger)
            break;
        i++;
        j--;
    }

    attack = true;
    danger = true;
    i = row - 1, j = col + 1;
    while (i >= 0 && j < 8)
    {
        if (attack)
            SetAttackedSquare(i, j, true);

        if (danger)
            SetkingDangerSquare(i, j, true);

        if (!PosEmpty(i, j))
        {
            attack = false;
            if (isEnemyKing(i, j, color))
            {
                AddChecker(row, col);
            }
            else
            {
                danger = false;
            }
        }
        if (!attack && !danger)
            break;
        i--;
        j++;
    }
}

void ChessEngine::UpdateQueenDangers(short row, short col, PieceColor color)
{
    UpdateRookDangers(row, col, color);
    UpdateBishopDangers(row, col, color);
}

void ChessEngine::updateLegalMoves()
{
    legalMoves.clear();
    attackedSquares.reset();
    kingDangerSquares.reset();
    captureMask.set();
    pushMask.set();
    pinnedPieces.reset();
    checkersNum = 0;

    updateDangers();

    UpdateCheck();

    UpdatePins();

    if (checkersNum >= 2) // when double check only king moves allowed
        return;

    for (short row = 0; row < 8; row++)
    {
        for (short col = 0; col < 8; col++)
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

void ChessEngine::UpdatePawnMoves(short row, short col, PieceColor color)
{
    short dir = color == PieceColor::WHITE ? +1 : -1;
    short startRow = color == PieceColor::WHITE ? 1 : 6;
    short prePromotionRow = color == PieceColor::WHITE ? 6 : 1;
    short enPassantRow = color == PieceColor::WHITE ? 5 : 4;

    short nextRow = row + dir;
    // pawn push
    if (ValidPos(nextRow, col) && PosEmpty(nextRow, col) && row != prePromotionRow)
    {
        if (GetPushMask(nextRow, col) && IsPinnedPieceLegalMove(row, col, nextRow, col))
            legalMoves.emplace_back(Move(row, col, nextRow, col, MoveType::QUIET));
    }

    // diagonal captures
    if (ValidPos(nextRow, col + 1) && CapturablePos(nextRow, col + 1, color))
    {
        if (GetCaptureMask(nextRow, col + 1) && IsPinnedPieceLegalMove(row, col, nextRow, col + 1))
            legalMoves.emplace_back(Move(row, col, nextRow, col + 1, MoveType::CAPTURE));
    }

    if (ValidPos(nextRow, col - 1) && CapturablePos(nextRow, col - 1, color))
    {
        if (GetCaptureMask(nextRow, col - 1) && IsPinnedPieceLegalMove(row, col, nextRow, col - 1))
            legalMoves.emplace_back(Move(row, col, nextRow, col - 1, MoveType::CAPTURE));
    }

    if (row == startRow)
    {
        if (PosEmpty(nextRow, col) && PosEmpty(nextRow + dir, col))
        {
            if (GetPushMask(nextRow + dir, col) && IsPinnedPieceLegalMove(row, col, nextRow + dir, col))
                legalMoves.emplace_back(Move(row, col, nextRow + dir, col, MoveType::DOUBLEPAWNPUSH));
        }
    }
    else if (row == prePromotionRow)
    {
        if (PosEmpty(nextRow, col) && GetPushMask(nextRow, col))
        {
            if (IsPinnedPieceLegalMove(row, col, nextRow, col))
            {
                legalMoves.emplace_back(Move(row, col, nextRow, col, MoveType::QUEENPROMOTION));
                legalMoves.emplace_back(Move(row, col, nextRow, col, MoveType::ROOKPROMOTION));
                legalMoves.emplace_back(Move(row, col, nextRow, col, MoveType::BISHOPPROMOTION));
                legalMoves.emplace_back(Move(row, col, nextRow, col, MoveType::KNIGHTPROMOTION));
            }
        }

        // diagonal captures while promotion
        if (ValidPos(nextRow, col + 1) && CapturablePos(nextRow, col + 1, color) && GetCaptureMask(nextRow, col + 1))
        {
            if (IsPinnedPieceLegalMove(row, col, nextRow, col + 1))
            {
                legalMoves.emplace_back(Move(row, col, nextRow, col + 1, MoveType::QUEENPROMOTION));
                legalMoves.emplace_back(Move(row, col, nextRow, col + 1, MoveType::ROOKPROMOTION));
                legalMoves.emplace_back(Move(row, col, nextRow, col + 1, MoveType::BISHOPPROMOTION));
                legalMoves.emplace_back(Move(row, col, nextRow, col + 1, MoveType::KNIGHTPROMOTION));
            }
        }

        if (ValidPos(nextRow, col - 1) && CapturablePos(nextRow, col - 1, color) && GetCaptureMask(nextRow, col - 1))
        {
            if (IsPinnedPieceLegalMove(row, col, nextRow, col - 1))
            {
                legalMoves.emplace_back(Move(row, col, nextRow, col - 1, MoveType::QUEENPROMOCAPTURE));
                legalMoves.emplace_back(Move(row, col, nextRow, col - 1, MoveType::ROOKPROMOCAPTURE));
                legalMoves.emplace_back(Move(row, col, nextRow, col - 1, MoveType::BISHOPPROMOCAPTURE));
                legalMoves.emplace_back(Move(row, col, nextRow, col - 1, MoveType::KNIGHTPROMOCAPTURE));
            }
        }
    }
    else if (row == enPassantRow)
    {
    }
}

void ChessEngine::UpdateKnightMoves(short row, short col, PieceColor color)
{
    short destRow = row + 1, destCol = col + 2;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row + 1, destCol = col - 2;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row - 1, destCol = col + 2;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row - 1, destCol = col - 2;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row + 2, destCol = col + 1;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row + 2, destCol = col - 1;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row - 2, destCol = col + 1;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }

    destRow = row - 2, destCol = col - 1;

    if (ValidPos(destRow, destCol) && IsPinnedPieceLegalMove(row, col, destRow, destCol))
    {
        if (PosEmpty(destRow, destCol) && GetPushMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::QUIET));
        }
        else if (CapturablePos(destRow, destCol, color) && GetCaptureMask(destRow, destCol))
        {
            legalMoves.emplace_back(Move(row, col, destRow, destCol, MoveType::CAPTURE));
        }
    }
}

void ChessEngine::UpdateRookMoves(short row, short col, PieceColor color)
{

    for (short i = row + 1; i < 8; i++)
    {
        if (!IsPinnedPieceLegalMove(row, col, i, col))
            continue;

        if (PosEmpty(i, col))
        {
            if (GetPushMask(i, col))
                legalMoves.emplace_back(Move(row, col, i, col, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, col, color) && GetCaptureMask(i, col))
            {
                legalMoves.emplace_back(Move(row, col, i, col, MoveType::CAPTURE));
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
                legalMoves.emplace_back(Move(row, col, i, col, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, col, color) && GetCaptureMask(i, col))
            {
                legalMoves.emplace_back(Move(row, col, i, col, MoveType::CAPTURE));
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
                legalMoves.emplace_back(Move(row, col, row, i, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(row, i, color) && GetCaptureMask(row, i))
            {
                legalMoves.emplace_back(Move(row, col, row, i, MoveType::CAPTURE));
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
                legalMoves.emplace_back(Move(row, col, row, i, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(row, i, color) && GetCaptureMask(row, i))
            {
                legalMoves.emplace_back(Move(row, col, row, i, MoveType::CAPTURE));
            }
            break;
        }
    }
}

void ChessEngine::UpdateKingMoves(short row, short col, PieceColor color)
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

void ChessEngine::UpdateBishopMoves(short row, short col, PieceColor color)
{

    short i = row + 1, j = col + 1;
    while (i < 8 && j < 8)
    {
        if (!ValidPos(i, j) || !IsPinnedPieceLegalMove(row, col, i, j))
            break;

        if (PosEmpty(i, j))
        {
            if (GetPushMask(i, j))
                legalMoves.emplace_back(Move(row, col, i, j, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, j, color) && GetCaptureMask(i, j))
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
        if (!ValidPos(i, j) || !IsPinnedPieceLegalMove(row, col, i, j))
            break;

        if (PosEmpty(i, j))
        {
            if (GetPushMask(i, j))
                legalMoves.emplace_back(Move(row, col, i, j, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, j, color) && GetCaptureMask(i, j))
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
        if (!ValidPos(i, j) || !IsPinnedPieceLegalMove(row, col, i, j))
            break;

        if (PosEmpty(i, j))
        {
            if (GetPushMask(i, j))
                legalMoves.emplace_back(Move(row, col, i, j, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, j, color) && GetCaptureMask(i, j))
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
        if (!ValidPos(i, j) || !IsPinnedPieceLegalMove(row, col, i, j))
            break;

        if (PosEmpty(i, j))
        {
            if (GetPushMask(i, j))
                legalMoves.emplace_back(Move(row, col, i, j, MoveType::QUIET));
        }
        else
        {
            if (CapturablePos(i, j, color) && GetCaptureMask(i, j))
            {
                legalMoves.emplace_back(Move(row, col, i, j, MoveType::CAPTURE));
            }
            break;
        }
        i--;
        j++;
    }
}

void ChessEngine::UpdateQueenMoves(short row, short col, PieceColor color)
{
    UpdateRookMoves(row, col, color);
    UpdateBishopMoves(row, col, color);
}

bool ChessEngine::IsSlider(short row, short col) const
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

void ChessEngine::UpdateCheck()
{

    short kingRow = turn == PieceColor::WHITE ? kingRowW : kingRowB;
    short kingCol = turn == PieceColor::WHITE ? kingColW : kingColB;

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

        UpdateKingMoves(kingRow, kingCol, turn);
    }
}

void ChessEngine::UpdatePins()
{
    /*short kingRow = turn == PieceColor::WHITE ? kingRowW : kingRowB;
    short kingCol = turn == PieceColor::WHITE ? kingColW : kingColB;

    // horizontal moves

    for (short i = kingRow + 1; i < 8; i++)
    {
        if (!PosEmpty(i, kingCol) && pieces[i][kingCol].color == turn)
        {
            if (GetAttackedSquare(i, kingCol))
                SetPinnedPiece(i, kingCol, true);
            break;
        }
    }

    for (short i = kingRow - 1; i >= 0; i--)
    {
        if (!PosEmpty(i, kingCol) && pieces[i][kingCol].color == turn)
        {
            if (GetAttackedSquare(i, kingCol))
                SetPinnedPiece(i, kingCol, true);
            break;
        }
    }

    for (short i = kingCol + 1; i < 8; i++)
    {
        if (!PosEmpty(kingRow, i) && pieces[kingRow][i].color == turn)
        {
            if (GetAttackedSquare(kingRow, i))
                SetPinnedPiece(kingRow, i, true);
            break;
        }
    }

    for (short i = kingCol - 1; i >= 0; i--)
    {
        if (!PosEmpty(kingRow, i) && pieces[kingRow][i].color == turn)
        {
            if (GetAttackedSquare(kingRow, i))
                SetPinnedPiece(kingRow, i, true);
            break;
        }
    }

    // diagonal moves

    short i = kingRow + 1, j = kingCol + 1;
    while (i < 8 && j < 8)
    {
        if (!ValidPos(i, j))
            break;

        if (!PosEmpty(i, j) && pieces[i][j].color == turn)
        {
            if (GetAttackedSquare(i, j))
                SetPinnedPiece(i, j, true);
            break;
        }
        i++;
        j++;
    }

    i = kingRow - 1, j = kingCol - 1;
    while (i >= 0 && j >= 0)
    {
        if (!ValidPos(i, j))
            break;

        if (!PosEmpty(i, j) && pieces[i][j].color == turn)
        {
            if (GetAttackedSquare(i, j))
                SetPinnedPiece(i, j, true);
            break;
        }
        i--;
        j--;
    }

    i = kingRow + 1, j = kingCol - 1;
    while (i < 8 && j >= 0)
    {
        if (!ValidPos(i, j))
            break;

        if (!PosEmpty(i, j) && pieces[i][j].color == turn)
        {
            if (GetAttackedSquare(i, j))
                SetPinnedPiece(i, j, true);
            break;
        }
        i++;
        j--;
    }

    i = kingRow - 1, j = kingCol + 1;
    while (i >= 0 && j < 8)
    {
        if (!ValidPos(i, j))
            break;

        if (!PosEmpty(i, j) && pieces[i][j].color == turn)
        {
            if (GetAttackedSquare(i, j))
                SetPinnedPiece(i, j, true);
            break;
        }
        i--;
        j++;
    }*/
}

bool ChessEngine::IsPinnedPieceLegalMove(short pieceRow, short pieceCol, short destRow, short destCol) const
{
    if (!GetPinnedPiece(pieceRow, pieceCol))
        return true; // piece is not pinned

    bool isLegal = false;

    PieceColor pieceColor = pieces[pieceRow][pieceCol].color;
    short kingRow = pieceColor == PieceColor::WHITE ? kingRowW : kingRowB;
    short kingCol = pieceColor == PieceColor::WHITE ? kingColW : kingColB;

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
    else // diagonak pin
    {
        /*if (kingCol - kingRow == destCol - destRow)
        {
            isLegal = true;
        }*/
    }

    return false;
}
