#include "Board.hpp"

#include <thread>

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

    resultText = std::make_unique<Object>(r, TextureName::CHECKMATE, posX + squareWidth * 3.f, posY + height, squareWidth * 2, squareWidth);
    CopyBoardFromEngine();
    UpdateLegalMoves();
    promotionSelector = std::make_unique<PromotionSelector>(r, posX, posY, squareWidth, squareHeight);
}

Board::~Board()
{
}

// shows board on screen
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

    if (renderResult)
        resultText->Render();

    if (promotionSelector->IsInSelection())
        promotionSelector->Render();
}

// remove all pieces from the board
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

// set up the chess position on the FEN, then updates the board view
void Board::LoadFEN(const std::string &FEN)
{
    moveGenerator.LoadFEN(FEN);
    CopyBoardFromEngine();
    UpdateLegalMoves();
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
    resultText->Translate(x, y);

    promotionSelector->Translate(x, y);
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
    resultText->SetScale(x, y);

    promotionSelector->SetScale(x, y, squareWidth, squareHeight);
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
    resultText->SetPosition(GetX() + squareWidth * 3.f, GetY() + GetHeight());

    promotionSelector->SetPosition(GetX(), GetY());
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
    resultText->SetPosition(GetX() + squareWidth * 3.f, GetY() + GetHeight());

    promotionSelector->SetCenter(GetX(), GetY());
}

// handle mouse inputs on the board, move pieces, promotion.
void Board::ClickEvent(float mouseX, float mouseY)
{
    static int rowPieceSelected;
    static int colPieceSelected;

    if (promotionSelector->IsInSelection())
    {
        promoPiece = promotionSelector->GetSelectedPiece(mouseX, mouseY);

        if (promoPiece == PieceType::EMPTY)
            return;

        Move selectedMove = FindMoveSelected(rowPieceSelected, colPieceSelected, promotionSelector->GetPromoRow(), promotionSelector->GetPromoCol());

        MakeMove(selectedMove);

        UnSelectBoard();
        SelectLastMove();

        pieceSelected = false;

        return;
    }

    if (!PosInside(mouseX, mouseY))
    {
        pieceSelected = false;
        return;
    }

    short row, col;
    GetSquareClicked(mouseX, mouseY, row, col);

    if (row < 0 || row >= 8 || col < 0 || col >= 8)
    {
        return;
    }

    if (pieceSelected)
    {
        if (GetPType(rowPieceSelected, colPieceSelected) == PieceType::PAWN)
        {
            if ((row == 7 || row == 0) && IsValidMove(rowPieceSelected, colPieceSelected, row, col))
            {
                promotionSelector->StartSelection(row, col, GetPColor(rowPieceSelected, colPieceSelected), rotated);
                return;
            }
        }

        Move selectedMove = FindMoveSelected(rowPieceSelected, colPieceSelected, row, col);

        pieceSelected = false;

        if (selectedMove.iniRow != -1)
            MakeMove(selectedMove);

        UnSelectBoard();
        SelectLastMove();
    }
    else
    {
        if (!PosEmpty(row, col))
        {
            rowPieceSelected = row;
            colPieceSelected = col;

            if (SelectLegalMovesFrom(rowPieceSelected, colPieceSelected) > 0)
            {
                pieceSelected = true;
                SelectSquare(row, col);
            }
        }
    }
}

// handle keyboard inputs on the board, rotation 'r'.
void Board::KeyEvent(char key)
{
    if (key == 'x' || key == 'X')
    {
        moveGenerator.UnMakeMove(moveGenerator.GetLastMove(), moveGenerator.GetStateInfo());
        CopyBoardFromEngine();
    }
    else if (key == 'r' || key == 'R')
    {
        if (promotionSelector->IsInSelection())
            return;
        rotated = !rotated;
        CopyBoardFromEngine();
        UnSelectBoard();
        SelectLastMove();
    }
    else if (key == 'p' || key == 'P')
    {

        MoveGenerator *movegen = &moveGenerator;
        ChessAI *ai = &chessAI;

        auto startSearchFunc = [ai, movegen]()
        {
            ai->StartSearch(*movegen);
        };

        std::thread searchWorker(startSearchFunc);

        std::this_thread::sleep_for(std::chrono::milliseconds(3000));

        chessAI.AbortSearch();

        searchWorker.join();

        Move moveAI = chessAI.GetBestMove();

        MakeMove(moveAI);
        UnSelectBoard();
        SelectLastMove();
    }
}

// executes a move in the moveGenerator, then updates the legal moves and the board
void Board::MakeMove(Move move)
{
    if (move.type == MoveType::INVALID)
        return;
    if (move.iniRow == move.endRow && move.iniCol == move.endCol)
        return;

    moveGenerator.MakeMove(move);
    CopyBoardFromEngine();
    UpdateLegalMoves();
    checkResult();
}

// highlights the last move played
void Board::SelectLastMove()
{
    Move move = moveGenerator.GetLastMove();

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

// unselect all squares in the board
void Board::UnSelectBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            UnSelectSquare(i, j);
        }
    }
}

// returns the move from initial square to final square, it considers the promotion selection
Move Board::FindMoveSelected(short iniRow, short iniCol, short finalRow, short finalCol)
{
    Move selectedMove = Move(-1, -1, -1, -1, MoveType::QUIET);

    for (int i = 0; i < numLegalMoves; i++)
    {
        if (legalMoves[i].endRow == finalRow && legalMoves[i].endCol == finalCol &&
            legalMoves[i].iniRow == iniRow && legalMoves[i].iniCol == iniCol)
        {
            if (legalMoves[i].type == MoveType::QUEENPROMOTION || legalMoves[i].type == MoveType::QUEENPROMOCAPTURE)
            {
                if (promoPiece == PieceType::QUEEN)
                {
                    selectedMove = legalMoves[i];
                    break;
                }
            }
            else if (legalMoves[i].type == MoveType::KNIGHTPROMOTION || legalMoves[i].type == MoveType::KNIGHTPROMOCAPTURE)
            {
                if (promoPiece == PieceType::KNIGHT)
                {
                    selectedMove = legalMoves[i];
                    break;
                }
            }
            else if (legalMoves[i].type == MoveType::BISHOPPROMOTION || legalMoves[i].type == MoveType::BISHOPPROMOCAPTURE)
            {
                if (promoPiece == PieceType::BISHOP)
                {
                    selectedMove = legalMoves[i];
                    break;
                }
            }
            else if (legalMoves[i].type == MoveType::ROOKPROMOTION || legalMoves[i].type == MoveType::ROOKPROMOCAPTURE)
            {
                if (promoPiece == PieceType::ROOK)
                {
                    selectedMove = legalMoves[i];
                    break;
                }
            }
            else
            {
                selectedMove = legalMoves[i];
                break;
            }
        }
    }

    return selectedMove;
}

// check if there is a legal move from initial square to final square
bool Board::IsValidMove(short iniRow, short iniCol, short finalRow, short finalCol)
{
    UpdateLegalMoves();
    for (int i = 0; i < numLegalMoves; i++)
    {
        if (legalMoves[i].endRow == finalRow && legalMoves[i].endCol == finalCol &&
            legalMoves[i].iniRow == iniRow && legalMoves[i].iniCol == iniCol)
        {
            return true;
        }
    }
    return false;
}

// returns the coords of the square clicked, it considers rotation
inline void Board::GetSquareClicked(float mouseX, float mouseY, short &row, short &col)
{
    col = (mouseX - GetX()) / squareWidth;
    row = (mouseY - GetY()) / squareHeight;

    if (rotated)
    {
        row = 7 - row;
        col = 7 - col;
    }
}

// calls the moveGenerator and stores the legal moves in an array.
inline void Board::UpdateLegalMoves()
{
    moveGenerator.GetLegalMoves(legalMoves, numLegalMoves);
}

// highlights all squares which a piece could move from row, col coords
int Board::SelectLegalMovesFrom(short pieceRow, short pieceCol)
{
    int numMovesSelected = 0;
    for (int i = 0; i < numLegalMoves; i++)
    {
        if (legalMoves[i].iniRow == pieceRow && legalMoves[i].iniCol == pieceCol)
        {
            SelectSquare(legalMoves[i].endRow, legalMoves[i].endCol);
            numMovesSelected++;
        }
    }

    return numMovesSelected;
}

// check if game finishes and shows game ending messages
void Board::checkResult()
{
    if (moveGenerator.IsCheckMate())
    {
        renderResult = true;
        resultText->SetTexture(TextureName::CHECKMATE);
    }
    else if (moveGenerator.IsStaleMate())
    {
        renderResult = true;
        resultText->SetTexture(TextureName::STALEMATE);
    }
    else
    {
        renderResult = false;
    }
}

// brings all pieces in moveGenerator to the board in screen, it considers rotation
void Board::CopyBoardFromEngine()
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (!rotated)
            {
                squares[row][col]->PutPiece(moveGenerator.GetPiece(row, col).type, moveGenerator.GetPiece(row, col).color);
            }
            else
            {
                squares[7 - row][7 - col]->PutPiece(moveGenerator.GetPiece(row, col).type, moveGenerator.GetPiece(row, col).color);
            }
        }
    }
}

// returns the type of the piece in the square row, col, it considers rotation
inline PieceType Board::GetPType(int rowPiece, int colPiece) const
{
    if (rotated)
    {
        rowPiece = 7 - rowPiece;
        colPiece = 7 - colPiece;
    }
    return squares[rowPiece][colPiece]->GetPiece();
}

// returns the color of the piece in the square row, col, it considers rotation
inline PieceColor Board::GetPColor(int rowPiece, int colPiece) const
{
    if (rotated)
    {
        rowPiece = 7 - rowPiece;
        colPiece = 7 - colPiece;
    }
    return squares[rowPiece][colPiece]->GetPieceColor();
}

// checks if the coords are inside the board ( 0 to 7)
inline bool Board::ValidPos(int row, int col) const
{
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

// returns true if in the square there is no piece, it considers rotation
inline bool Board::PosEmpty(int row, int col) const
{
    if (rotated)
    {
        row = 7 - row;
        col = 7 - col;
    }

    return squares[row][col]->IsEmpty();
}

// selects the square with coords row, col, it considers rotation
inline void Board::SelectSquare(int row, int col)
{
    if (rotated)
    {
        row = 7 - row;
        col = 7 - col;
    }

    if (!ValidPos(row, col))
        return;

    return squares[row][col]->Select();
}

// highlights the square with a last move color, it considers rotation
inline void Board::SelectAsLastMove(int row, int col)
{
    if (rotated)
    {
        row = 7 - row;
        col = 7 - col;
    }

    if (!ValidPos(row, col))
        return;
    return squares[row][col]->SelectAsLastMove();
}

// unselect square with coords row, col, it considers rotation
inline void Board::UnSelectSquare(int row, int col)
{
    if (rotated)
    {
        row = 7 - row;
        col = 7 - col;
    }

    if (!ValidPos(row, col))
        return;
    return squares[row][col]->UnSelect();
}