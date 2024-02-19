#include "Board.hpp"

Board::Board(float posX, float posY, float width, float height, const Renderer &r)
    : Object(r, TextureName::BOARD, posX, posY, width, height)
{
    squareWidth = width / 8.f;
    squareHeight = height / 8.f;

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            squares[row][col] =
                std::make_unique<Square>(row, col, posX + col * squareWidth, posY + row * squareHeight, squareWidth, squareHeight, r);
        }
    }
}

Board::~Board()
{
}

void Board::Render()
{
    Object::Render();

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            squares[row][col]->Render();
        }
    }
}

void Board::Clear()
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            squares[row][col]->Clear();
        }
    }
}

void Board::PutInitialPosition()
{
    for (int row = 2; row < 6; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            squares[row][col]->Clear();
        }
    }

    for (int col = 0; col < 8; col++)
    {
        squares[1][col]->PutPiece(PieceType::PAWN, PieceColor::WHITE);
        squares[6][col]->PutPiece(PieceType::PAWN, PieceColor::BLACK);
    }

    squares[0][0]->PutPiece(PieceType::ROOK, PieceColor::WHITE);
    squares[0][1]->PutPiece(PieceType::KNIGHT, PieceColor::WHITE);
    squares[0][2]->PutPiece(PieceType::BISHOP, PieceColor::WHITE);
    squares[0][3]->PutPiece(PieceType::QUEEN, PieceColor::WHITE);
    squares[0][4]->PutPiece(PieceType::KING, PieceColor::WHITE);
    squares[0][5]->PutPiece(PieceType::BISHOP, PieceColor::WHITE);
    squares[0][6]->PutPiece(PieceType::KNIGHT, PieceColor::WHITE);
    squares[0][7]->PutPiece(PieceType::ROOK, PieceColor::WHITE);

    squares[7][0]->PutPiece(PieceType::ROOK, PieceColor::BLACK);
    squares[7][1]->PutPiece(PieceType::KNIGHT, PieceColor::BLACK);
    squares[7][2]->PutPiece(PieceType::BISHOP, PieceColor::BLACK);
    squares[7][3]->PutPiece(PieceType::QUEEN, PieceColor::BLACK);
    squares[7][4]->PutPiece(PieceType::KING, PieceColor::BLACK);
    squares[7][5]->PutPiece(PieceType::BISHOP, PieceColor::BLACK);
    squares[7][6]->PutPiece(PieceType::KNIGHT, PieceColor::BLACK);
    squares[7][7]->PutPiece(PieceType::ROOK, PieceColor::BLACK);
}

void Board::Translate(float x, float y)
{
    Object::Translate(x, y);

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            squares[row][col]->Translate(x, y);
        }
    }
}
void Board::SetScale(float x, float y)
{
    Object::SetScale(x, y);

    squareWidth = GetWidth() / 8.f;
    squareHeight = GetHeight() / 8.f;

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            squares[row][col]->SetPosition(x + col * squareWidth, y + row * squareHeight);
            squares[row][col]->SetScale(x, y);
        }
    }
}
void Board::SetPosition(float x, float y)
{
    Object::SetPosition(x, y);

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            squares[row][col]->SetPosition(x + col * squareWidth, y + row * squareHeight);
        }
    }
}
void Board::SetCenter(float x, float y)
{
    Object::SetCenter(x, y);

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            squares[row][col]->SetPosition(GetX() + col * squareWidth, GetY() + row * squareHeight);
        }
    }
}

void Board::ClickEvent(float mouseX, float mouseY)
{
    if (!PosInside(mouseX, mouseY))
    {
        pieceSelected = false;
        return;
    }

    int col = (mouseX - GetX()) / squareWidth;
    int row = (mouseY - GetY()) / squareHeight;

    if (row < 0 || row >= 8 || col < 0 || col >= 8)
    {
        return;
    }

    if (pieceSelected)
    {
        pieceSelected = false;

        if (squares[row][col]->IsSelected())
        {

            for (Move move : chessEngine.GetLegalMoves())
            {
                if (move.endRow == row && move.endCol == col &&
                    move.iniRow == rowSelected && move.iniCol == colSelected)
                {
                    MovePiece(move);
                    break;
                }
            }
        }

        UnSelectBoard();
    }
    else
    {
        if (!PosEmpty(row, col))
        {
            pieceSelected = true;
            SelectPos(row, col);
            rowSelected = row;
            colSelected = col;

            for (Move move : chessEngine.GetLegalMoves())
            {
                if (move.iniRow == rowSelected && move.iniCol == colSelected)
                {
                    SelectPos(move.endRow, move.endCol);
                }
            }
        }
    }
}

void Board::MovePiece(Move move)
{
    if (move.iniRow == move.endRow && move.iniCol == move.endCol)
        return;

    if (move.type == MoveType::KINGCASTLE)
    {

        squares[move.endRow][move.endCol - 1]->PutPiece(PieceType::ROOK,
                                                        GetPColor(move.iniRow, move.iniCol));
        squares[move.endRow][7]->Clear();
    }
    else if (move.type == MoveType::QUEENCASTLE)
    {

        squares[move.endRow][move.endCol + 1]->PutPiece(PieceType::ROOK,
                                                        GetPColor(move.iniRow, move.iniCol));
        squares[move.endRow][0]->Clear();
    }
    else
    {
    }
    squares[move.endRow][move.endCol]->PutPiece(GetPType(move.iniRow, move.iniCol),
                                                GetPColor(move.iniRow, move.iniCol));
    squares[move.iniRow][move.iniCol]->Clear();

    chessEngine.MakeMove(move);
}

void Board::SelectLastMove()
{
    Move move = chessEngine.GetLastMove();

    if (move.iniCol == -1)
        return;

    if (move.type == MoveType::KINGCASTLE)
    {
        SelectAsLastMove(move.iniRow, move.endCol - 1);
        SelectAsLastMove(move.endRow, move.endCol);
    }
    else if (move.type == MoveType::QUEENCASTLE)
    {
        SelectAsLastMove(move.iniRow, move.endCol + 1);
        SelectAsLastMove(move.endRow, move.endCol);
    }
    else
    {
        SelectAsLastMove(move.iniRow, move.iniCol);
        SelectAsLastMove(move.endRow, move.endCol);
    }
}

void Board::UnSelectBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            UnSelectPos(i, j);
        }
    }
    SelectLastMove();
}