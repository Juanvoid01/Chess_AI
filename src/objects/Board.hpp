#pragma once

#include "Square.hpp"
#include "ChessAI.hpp"
#include "PromotionSelector.hpp"

#include <vector>

class Board : public Object
{
public:
    enum class State
    {
        UNSELECTED,
        PIECE_SELECTED,
        SELECTING_PROMO,
        MOVING_PIECES
    };

    Board(float posX, float posY, float width, float height, const Renderer &r, ChessAI &chessAI, MoveGenerator &moveGen);
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
    void ReCalculateMVP();
    
    // returns true if the board has been clicked
    bool ClickEvent(float mouseX, float mouseY);

    void Rotate();

    // executes a move in the moveGenerator, then updates the legal moves and the board
    void MakeMove(Move move);

    // executes the move selected by the IA
    void MoveIA(Move move);

    // highlights the last move played
    void SelectLastMove();

    // unselect all squares in the board
    void UnSelectBoard();

    // get state of the board
    inline State GetState() const { return state; }

private:
    State state;
    std::unique_ptr<Object> resultText;
    bool renderResult = false;
    std::array<std::array<std::unique_ptr<Square>, 8>, 8> squares;
    float squareWidth;
    float squareHeight;
    //bool pieceSelected = false;

    bool rotated = false;

    int numLegalMoves = 0;
    MoveArray legalMoves;

    PieceType promoPiece = PieceType::EMPTY;

    MoveGenerator &moveGenerator;

    ChessAI &chessAI;

    std::unique_ptr<PromotionSelector> promotionSelector;

    bool clicked = false;
    short rowClicked = -1, colClicked = -1;
    float clickX = -1, clickY = -1;
    Move selectedMove = InvalidMove;

    std::unique_ptr<Piece> movingPiece;
    // brings all pieces in chessEngine to the board in screen, it considers rotation
    void CopyBoardFromEngine();

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

    // returns the move from initial square to final square, it considers the promotion selection
    Move FindMoveSelected(short iniRow, short iniCol, short finalRow, short finalCol);

    // check if there is a legal move from initial square to final square
    bool IsValidMove(short iniRow, short iniCol, short finalRow, short finalCol);

    // returns the coords of the square clicked, it considers rotation
    inline void GetSquareClicked(float mouseX, float mouseY, short &row, short &col);

    // calls the moveGenerator and stores the legal moves in an array.
    inline void UpdateLegalMoves();

    // Add the pieces involved in the move to the movingPiecesVector
    void AddMoveToMovingPieces(const Move &move);

    // Get the coords of the center of the square row, col
    void GetCoordsOfSquare(short row, short col, float &posX, float &posY) const;

    // set All pieces visibility to a value
    void SetPiecesVisibility(bool value);
};
