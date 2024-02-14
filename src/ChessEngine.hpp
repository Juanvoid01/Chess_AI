#pragma once

#include "Move.hpp"
#include <bitset>
#include <vector>
#include <array>

#define MAX_THEORETICAL_MOVES_PER_POSITION 218

class ChessEngine
{
public:
    ChessEngine();
    ~ChessEngine() {}

    inline const std::vector<Move> &GetLegalMoves() const { return legalMoves; }
    void SetInitialPosition();
    void ClearBoard();
    void MakeMove(Move move);

private:
    std::vector<Move> legalMoves;
    std::bitset<64> kingDangerSquares;
    std::bitset<64> kingAttackedSquares;
    std::array<std::array<PieceInfo, 8>, 8> pieces;
    PieceColor turn = PieceColor::WHITE;

    inline void SetkingAttackedSquare(int row, int col, bool value) { kingAttackedSquares.set(row * 8 + col, value); }
    inline void SetkingDangerSquare(int row, int col, bool value) { kingDangerSquares.set(row * 8 + col, value); }
    inline bool GetkingAttackedSquare(int row, int col) { return kingAttackedSquares[row * 8 + col]; }
    inline bool GetkingDangerSquare(int row, int col) { return kingDangerSquares[row * 8 + col]; }
    inline bool ValidPos(int row, int col) const { return row >= 0 && row < 8 && col >= 0 && col < 8; }
    inline bool PosEmpty(int row, int col) const { return pieces[row][col].type == PieceType::EMPTY; }
    inline bool CapturablePos(int row, int col, PieceColor color) const { return !PosEmpty(row, col) && pieces[row][col].color != color; }
    inline void ClearPos(int row, int col) { pieces[row][col] = {PieceType::EMPTY, PieceColor::WHITE}; }
    
    void updateDangers();
    void UpdatePawnDangers(int row, int col, PieceColor color);
    void UpdateKnightDangers(int row, int col, PieceColor color);
    void UpdateRookDangers(int row, int col, PieceColor color);
    void UpdateKingDangers(int row, int col, PieceColor color);
    void UpdateBishopDangers(int row, int col, PieceColor color);
    void UpdateQueenDangers(int row, int col, PieceColor color);

    void updateLegalMoves();
    void UpdatePawnMoves(int row, int col, PieceColor color);
    void UpdateKnightMoves(int row, int col, PieceColor color);
    void UpdateRookMoves(int row, int col, PieceColor color);
    void UpdateKingMoves(int row, int col, PieceColor color);
    void UpdateBishopMoves(int row, int col, PieceColor color);
    void UpdateQueenMoves(int row, int col, PieceColor color);
};