#pragma once

#include "Square.hpp"
#include "Move.hpp"
#include <vector>

class Board : public Object
{
public:
    Board(float posX, float posY, float width, float height, const Renderer &r);
    ~Board();
    void Render();
    const std::vector<Move> &GetMoves(int col, int row);

    void Clear();
    void PutInitialPosition();

    void Translate(float x, float y);
    void SetScale(float x, float y);
    void SetPosition(float x, float y);
    void SetCenter(float x, float y);

    void ClickEvent(float mouseX, float mouseY);

private:
    std::array<std::array<std::unique_ptr<Square>, 8>, 8> squares;
    float squareWidth;
    float squareHeight;
    bool pieceSelected = false;
    int rowSelected = -1;
    int colSelected = -1;

    std::vector<BoardPos> legalMoves;

    void MovePiece(int originRow, int originCol, int finalRow, int finalCol);

    void SelectPiece(int row, int col);
    void UnSelectPiece();

    void GetLegalMoves(int row, int col);

    void GetPawnMoves(int row, int col, PieceColor color);
    void GetKnightMoves(int row, int col, PieceColor color);
    void GetRookMoves(int row, int col, PieceColor color);
    void GetQueenMoves(int row, int col, PieceColor color);
    void GetKingMoves(int row, int col, PieceColor color);
    void GetBishopMoves(int row, int col, PieceColor color);

    inline PieceColor GetPColor(int row, int col) const { return squares[row][col]->GetPieceColor(); }

    inline bool ValidPos(int row, int col) const { return row >= 0 && row < 8 && col >= 0 && col < 8; }

    inline bool PosEmpty(int row, int col) const { return squares[row][col]->IsEmpty(); }

    inline void SelectPos(int row, int col) { return squares[row][col]->Select(); }

    inline void UnSelectPos(int row, int col) { return squares[row][col]->UnSelect(); }

    inline void AddLegalMove(int row, int col) { legalMoves.push_back({row, col}); }

    inline bool CapturablePos(int row, int col, PieceColor color) const { return !PosEmpty(row, col) && GetPColor(row, col) != color; }
};
