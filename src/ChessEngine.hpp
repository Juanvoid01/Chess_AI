#pragma once

#include "Move.hpp"
#include <bitset>
#include <vector>
#include <array>

#define MAX_THEORETICAL_MOVES_PER_POSITION 218
#define FEN_START_POS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

class ChessEngine
{
public:
    ChessEngine(const std::string &FEN = FEN_START_POS);
    ~ChessEngine() {}

    inline const std::vector<Move> &GetLegalMoves() const { return legalMoves; }
    void SetInitialPosition();
    void ClearBoard();
    void MakeMove(Move move);
    inline Move GetLastMove() const { return lastMove; }

    bool IsCheckMate() const;
    bool IsStaleMate() const;
    bool IsDrawBy50move() const { return halfMoveCounter >= 100; }
    void LoadFEN(const std::string &fen);
    inline PieceInfo GetPiece(short row, short col) const { return pieces[row][col]; }
    inline int GetMoveCounter() const { return moveCounter; }

private:
    std::vector<Move> legalMoves;
    std::bitset<64> kingDangerSquares;
    std::bitset<64> attackedSquares;
    std::bitset<64> captureMask;
    std::bitset<64> pushMask;
    std::bitset<64> pinnedPieces;

    std::array<std::array<PieceInfo, 8>, 8> pieces;
    PieceColor turn = PieceColor::WHITE;
    Move lastMove = Move(-1, -1, -1, -1, MoveType::QUIET);

    short checkerRow;
    short checkerCol;
    short checkersNum;
    short kingRowW, kingColW;
    short kingRowB, kingColB;

    bool kingMovedW;
    bool kingMovedB;
    bool rookKMovedW;
    bool rookQMovedW;
    bool rookKMovedB;
    bool rookQMovedB;

    int halfMoveCounter;
    int moveCounter;

    inline void SetAttackedSquare(short row, short col, bool value) { attackedSquares.set(row * 8 + col, value); }
    inline void SetkingDangerSquare(short row, short col, bool value) { kingDangerSquares.set(row * 8 + col, value); }
    inline void SetCaptureMask(short row, short col, bool value) { captureMask.set(row * 8 + col, value); }
    inline void SetPushMask(short row, short col, bool value) { pushMask.set(row * 8 + col, value); }
    inline void SetPinnedPiece(short row, short col, bool value) { pinnedPieces.set(row * 8 + col, value); }

    inline bool GetAttackedSquare(short row, short col) const { return attackedSquares[row * 8 + col]; }
    inline bool GetkingDangerSquare(short row, short col) const { return kingDangerSquares[row * 8 + col]; }
    inline bool GetCaptureMask(short row, short col) const { return captureMask[row * 8 + col]; }
    inline bool GetPushMask(short row, short col) const { return pushMask[row * 8 + col]; }
    inline bool GetPinnedPiece(short row, short col) const { return pinnedPieces[row * 8 + col]; }

    inline bool ValidPos(short row, short col) const { return row >= 0 && row < 8 && col >= 0 && col < 8; }
    inline bool PosEmpty(short row, short col) const { return pieces[row][col].type == PieceType::EMPTY; }
    inline bool CapturablePos(short row, short col, PieceColor color) const { return !PosEmpty(row, col) && pieces[row][col].color != color; }
    inline void ClearPos(short row, short col) { pieces[row][col] = {PieceType::EMPTY, PieceColor::WHITE}; }
    inline bool isEnemyKing(short row, short col, PieceColor color) const { return pieces[row][col].type == PieceType::KING && pieces[row][col].color != color; }
    inline bool isEnemyPiece(short row, short col, PieceColor color) const { return !PosEmpty(row, col) && pieces[row][col].color != color; }
    inline bool isFriendlyPiece(short row, short col, PieceColor color) const { return !PosEmpty(row, col) && pieces[row][col].color == color; }

    void updateDangers();
    void UpdatePawnDangers(short row, short col, PieceColor color);
    void UpdateKnightDangers(short row, short col, PieceColor color);
    void UpdateRookDangers(short row, short col, PieceColor color);
    void UpdateKingDangers(short row, short col, PieceColor color);
    void UpdateBishopDangers(short row, short col, PieceColor color);
    void UpdateQueenDangers(short row, short col, PieceColor color);

    void updateLegalMoves();
    void UpdatePawnMoves(short row, short col, PieceColor color);
    void UpdateKnightMoves(short row, short col, PieceColor color);
    void UpdateRookMoves(short row, short col, PieceColor color);
    void UpdateKingMoves(short row, short col, PieceColor color);
    void UpdateBishopMoves(short row, short col, PieceColor color);
    void UpdateQueenMoves(short row, short col, PieceColor color);
    void UpdateCastleMoves(PieceColor color);

    bool IsSlider(short row, short col) const;

    void UpdateCheck();
    bool IsPinnedPieceLegalMove(short pieceRow, short pieceCol, short destRow, short destCol) const;
    bool isValidEnPassant(short pieceRow, short pieceCol, short destRow, short destCol) const;

    inline void AddChecker(short row, short col)
    {
        checkersNum++;
        checkerRow = row;
        checkerCol = col;
    }
};