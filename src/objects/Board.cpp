#include "Board.hpp"

Board::Board(float posX, float posY, float width, float height, const Renderer &r)
    : Object(posX, posY, width, height, TextureName::BOARD, r)
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

        if (row != rowSelected || col != colSelected)
            MovePiece(rowSelected, colSelected, row, col);
    }
    else
    {
        if (!squares[row][col]->IsEmpty())
        {
            pieceSelected = true;
            rowSelected = row;
            colSelected = col;
        }
    }
}

void Board::MovePiece(int originRow, int originCol, int finalRow, int finalCol)
{
    squares[finalRow][finalCol]->PutPiece(squares[originRow][originCol]->GetPiece(),
                                          squares[originRow][originCol]->GetPieceColor());

    squares[originRow][originCol]->Clear();
}
