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

    resultText = std::make_unique<Object>(r, TextureName::CHECKMATE, posX + squareWidth * 3.f, posY + height, squareWidth * 2, squareWidth);
    CopyBoardFromEngine();

    promotionSelector = std::make_unique<PromotionSelector>(r, posX, posY, squareWidth, squareHeight);
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

    if (renderResult)
        resultText->Render();

    if (promotionSelector->IsInSelection())
        promotionSelector->Render();
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

void Board::LoadFEN(const std::string &FEN)
{
    chessEngine.LoadFEN(FEN);
    CopyBoardFromEngine();
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

void Board::ClickEvent(float mouseX, float mouseY)
{
    if (promotionSelector->IsInSelection())
    {
        promoPiece = promotionSelector->GetSelectedPiece(mouseX, mouseY);

        Move selectedMove = FindMoveSelected(rowSelected, colSelected, promotionSelector->endRow, promotionSelector->endCol);

        MovePiece(selectedMove);

        pieceSelected = false;
        return;
    }

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
        if (squares[rowSelected][colSelected]->GetPiece() == PieceType::PAWN)
        {
            if (row == 7 || row == 0)
            {
                promotionSelector->StartSelection(row, col);
                return;
            }
        }

        pieceSelected = false;

        Move selectedMove = FindMoveSelected(rowSelected, colSelected, row, col);

        if (selectedMove.iniRow != -1)
            MovePiece(selectedMove);

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

            chessEngine.GetLegalMoves(legalMoves, numLegalMoves);
            for (int i = 0; i < numLegalMoves; i++)
            {
                if (legalMoves[i].iniRow == rowSelected && legalMoves[i].iniCol == colSelected)
                {
                    SelectPos(legalMoves[i].endRow, legalMoves[i].endCol);
                }
            }
        }
    }
}

void Board::KeyEvent(char key)
{
    if (key == 'x' || key == 'X')
    {
        chessEngine.UnMakeMove(chessEngine.GetLastMove(), chessEngine.GetStateInfo());
        CopyBoardFromEngine();
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

        squares[move.endRow][move.endCol]->PutPiece(GetPType(move.iniRow, move.iniCol),
                                                    GetPColor(move.iniRow, move.iniCol));
    }
    else if (move.type == MoveType::QUEENCASTLE)
    {

        squares[move.endRow][move.endCol + 1]->PutPiece(PieceType::ROOK,
                                                        GetPColor(move.iniRow, move.iniCol));
        squares[move.endRow][0]->Clear();

        squares[move.endRow][move.endCol]->PutPiece(GetPType(move.iniRow, move.iniCol),
                                                    GetPColor(move.iniRow, move.iniCol));
    }
    else if (move.type == MoveType::ENPASSANT)
    {

        squares[move.iniRow][move.endCol]->Clear();

        squares[move.endRow][move.endCol]->PutPiece(GetPType(move.iniRow, move.iniCol),
                                                    GetPColor(move.iniRow, move.iniCol));
    }
    else if (move.type == MoveType::QUEENPROMOTION || move.type == MoveType::QUEENPROMOCAPTURE)
    {
        squares[move.endRow][move.endCol]->PutPiece(PieceType::QUEEN, GetPColor(move.iniRow, move.iniCol));
    }
    else if (move.type == MoveType::KNIGHTPROMOTION || move.type == MoveType::KNIGHTPROMOCAPTURE)
    {
        squares[move.endRow][move.endCol]->PutPiece(PieceType::KNIGHT, GetPColor(move.iniRow, move.iniCol));
    }
    else if (move.type == MoveType::BISHOPPROMOTION || move.type == MoveType::BISHOPPROMOCAPTURE)
    {
        squares[move.endRow][move.endCol]->PutPiece(PieceType::BISHOP, GetPColor(move.iniRow, move.iniCol));
    }
    else if (move.type == MoveType::ROOKPROMOTION || move.type == MoveType::ROOKPROMOCAPTURE)
    {
        squares[move.endRow][move.endCol]->PutPiece(PieceType::ROOK, GetPColor(move.iniRow, move.iniCol));
    }
    else
    {
        squares[move.endRow][move.endCol]->PutPiece(GetPType(move.iniRow, move.iniCol),
                                                    GetPColor(move.iniRow, move.iniCol));
    }

    squares[move.iniRow][move.iniCol]->Clear();

    chessEngine.MakeMove(move);

    checkResult();
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

Move Board::FindMoveSelected(short iniRow, short iniCol, short finalRow, short finalCol)
{
    Move selectedMove = Move(-1, -1, -1, -1, MoveType::QUIET);

    if (squares[finalRow][finalCol]->IsSelected())
    {
        chessEngine.GetLegalMoves(legalMoves, numLegalMoves);
        for (int i = 0; i < numLegalMoves; i++)
        {
            if (legalMoves[i].endRow == finalRow && legalMoves[i].endCol == finalCol &&
                legalMoves[i].iniRow == rowSelected && legalMoves[i].iniCol == colSelected)
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
    }

    return selectedMove;
}

void Board::checkResult()
{
    if (chessEngine.IsCheckMate())
    {
        renderResult = true;
        resultText->SetTexture(TextureName::CHECKMATE);
    }
    else if (chessEngine.IsStaleMate())
    {
        renderResult = true;
        resultText->SetTexture(TextureName::STALEMATE);
    }
    else
    {
        renderResult = false;
    }
}

void Board::CopyBoardFromEngine()
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            squares[row][col]->PutPiece(chessEngine.GetPiece(row, col).type, chessEngine.GetPiece(row, col).color);
        }
    }
}