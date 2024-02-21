#pragma once

#include "Square.hpp"
#include "ChessEngine.hpp"
#include <vector>

class Board : public Object
{
public:
    Board(float posX, float posY, float width, float height, const Renderer &r);
    ~Board();
    void Render();

    void Clear();
    void LoadFEN(const std::string& FEN);
    void Translate(float x, float y);
    void SetScale(float x, float y);
    void SetPosition(float x, float y);
    void SetCenter(float x, float y);

    void ClickEvent(float mouseX, float mouseY);

private:
    std::unique_ptr<Object> resultText;
    bool renderResult = false;
    std::array<std::array<std::unique_ptr<Square>, 8>, 8> squares;
    float squareWidth;
    float squareHeight;
    bool pieceSelected = false;
    int rowSelected = -1;
    int colSelected = -1;

    PieceType promoPiece = PieceType::QUEEN;
    
    ChessEngine chessEngine;

    void CopyBoardFromEngine();
    void MovePiece(Move move);

    void SelectPiece(int row, int col);
    void UnSelectPiece();

    inline PieceType GetPType(int row, int col) const { return squares[row][col]->GetPiece(); }

    inline PieceColor GetPColor(int row, int col) const { return squares[row][col]->GetPieceColor(); }

    inline bool ValidPos(int row, int col) const { return row >= 0 && row < 8 && col >= 0 && col < 8; }

    inline bool PosEmpty(int row, int col) const { return squares[row][col]->IsEmpty(); }

    inline void SelectPos(int row, int col) { return squares[row][col]->Select(); }

    inline void SelectAsLastMove(int row, int col) { return squares[row][col]->SelectAsLastMove(); }

    inline void UnSelectPos(int row, int col) { return squares[row][col]->UnSelect(); }

    void checkResult();
    
    void SelectLastMove();
    
    void UnSelectBoard();
};
