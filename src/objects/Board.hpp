#pragma once

#include "Square.hpp"
#include "ChessAI.hpp"
#include "PromotionSelector.hpp"
#include "InformationText.hpp"

#include <vector>

class Board : public Object
{
public:
    Board(float posX, float posY, float width, float height, const Renderer &r, ChessAI& chessAI, InformationText& infoText);
    ~Board();

    // shows board on screen
    void Render();

    // update board state
    void Update();

    // remove all pieces from the board
    void Clear();

    // set up the chess position on the FEN, then updates the board view
    void LoadFEN(const std::string &FEN);

    void Translate(float x, float y);
    void SetScale(float x, float y);
    void SetPosition(float x, float y);
    void SetCenter(float x, float y);

    // handle mouse inputs on the board, move pieces, promotion.
    void ClickEvent(float mouseX, float mouseY);

    // handle keyboard inputs on the board, rotation 'r'.
    void KeyEvent(char key);

    void Rotate();

private:

    InformationText& infoText;
    std::unique_ptr<Object> resultText;
    bool renderResult = false;
    std::array<std::array<std::unique_ptr<Square>, 8>, 8> squares;
    float squareWidth;
    float squareHeight;
    bool pieceSelected = false;

    bool rotated = false;

    int numLegalMoves = 0;
    MoveArray legalMoves;

    PieceType promoPiece = PieceType::EMPTY;

    MoveGenerator moveGenerator;

    ChessAI& chessAI;

    std::unique_ptr<PromotionSelector> promotionSelector;

    // brings all pieces in chessEngine to the board in screen, it considers rotation
    void CopyBoardFromEngine();

    // executes a move in the moveGenerator, then updates the legal moves and the board
    void MakeMove(Move move);

    void SelectPiece(int row, int col);
    void UnSelectPiece();

    // returns the type of the piece in the square row, col, it considers rotation
    inline PieceType GetPType(int rowPiece, int colPiece) const;

    // returns the color of the piece in the square row, col, it considers rotation
    inline PieceColor GetPColor(int rowPiece, int colPiece) const;

    // checks if the coords are inside the board ( 0 to 7)
    inline bool ValidPos(int row, int col) const;

    // returns true if in the square there is no piece, it considers rotation
    inline bool PosEmpty(int row, int col) const;

    // selects the square with coords row, col, it considers rotation
    inline void SelectSquare(int row, int col);

    // highlights the square with a last move color, it considers rotation
    inline void SelectAsLastMove(int row, int col);

    // unselect square with coords row, col, it considers rotation
    inline void UnSelectSquare(int row, int col);

    // highlights all squares which a piece could move from row, col coords
    int SelectLegalMovesFrom(short pieceRow, short pieceCol);

    // check if game finishes and shows game ending messages
    void checkResult();

    // highlights the last move played
    void SelectLastMove();

    // unselect all squares in the board
    void UnSelectBoard();

    // returns the move from initial square to final square, it considers the promotion selection
    Move FindMoveSelected(short iniRow, short iniCol, short finalRow, short finalCol);

    // check if there is a legal move from initial square to final square
    bool IsValidMove(short iniRow, short iniCol, short finalRow, short finalCol);

    // returns the coords of the square clicked, it considers rotation
    inline void GetSquareClicked(float mouseX, float mouseY, short &row, short &col);

    // calls the moveGenerator and stores the legal moves in an array.
    inline void UpdateLegalMoves();
};
