#include "Zobrist.hpp"

// generate random keys to calculate hashValue
Zobrist::Zobrist()
{
    // we generate random values for each square in the board
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0ULL, std::numeric_limits<uint64_t>::max());

    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            for (int pieceType = 0; pieceType < NUM_PIECES; pieceType++)
            {
                keys[row][col][pieceType] = dis(gen);
            }
        }
    }
    hashValue = 0ULL;
}

// modifies the HashValue of the position, it performs XOR with the key value of the movement
inline void Zobrist::AddPiece(short row, short col, PieceInfo piece)
{
    hashValue ^= keys[row][col][PieceToNumber(piece)];
}

// modifies the HashValue of the position, it performs XOR with the key value of the movement
inline void Zobrist::DeletePiece(short row, short col, PieceInfo piece)
{
    hashValue ^= keys[row][col][PieceToNumber(piece)];
}

// put hashValue doint XOR of all pieces on board
void Zobrist::InitializeHashWithPos(const PieceArray &pieces)
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (pieces[row][col].type != PieceType::EMPTY)
                hashValue ^= keys[row][col][PieceToNumber(pieces[row][col])];
        }
    }
}

// CALL BEFORE MAKING THE MOVE
//  modifies the HashValue, it calls Add and Delete pieces that the move produces
void Zobrist::AddMove(Move move, const PieceArray &pieces)
{
    switch (move.type)
    {
    case MoveType::QUIET:
        AddPiece(move.endRow, move.endCol, pieces[move.iniRow][move.iniCol]);
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        break;

    case MoveType::CAPTURE:
        DeletePiece(move.endRow, move.endCol, pieces[move.endRow][move.endCol]);
        AddPiece(move.endRow, move.endCol, pieces[move.iniRow][move.iniCol]);
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        break;

    case MoveType::ENPASSANT:
        DeletePiece(move.iniRow, move.endCol, pieces[move.iniRow][move.endCol]);
        AddPiece(move.endRow, move.endCol, pieces[move.iniRow][move.iniCol]);
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        break;

    case MoveType::KINGCASTLE:
        AddPiece(move.endRow, move.endCol, pieces[move.iniRow][move.iniCol]);
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        AddPiece(move.endRow, move.endCol - 1, pieces[move.iniRow][7]);
        DeletePiece(move.iniRow, 7, pieces[move.iniRow][7]);
        break;

    case MoveType::QUEENCASTLE:
        AddPiece(move.endRow, move.endCol, pieces[move.iniRow][move.iniCol]);
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        AddPiece(move.endRow, move.endCol + 1, pieces[move.iniRow][0]);
        DeletePiece(move.iniRow, 0, pieces[move.iniRow][0]);
        break;

    case MoveType::QUEENPROMOTION:
        AddPiece(move.endRow, move.endCol, {PieceType::QUEEN, pieces[move.iniRow][move.iniCol].color});
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        break;

    case MoveType::KNIGHTPROMOTION:
        AddPiece(move.endRow, move.endCol, {PieceType::KNIGHT, pieces[move.iniRow][move.iniCol].color});
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        break;

    case MoveType::ROOKPROMOTION:
        AddPiece(move.endRow, move.endCol, {PieceType::ROOK, pieces[move.iniRow][move.iniCol].color});
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        break;

    case MoveType::BISHOPPROMOTION:
        AddPiece(move.endRow, move.endCol, {PieceType::BISHOP, pieces[move.iniRow][move.iniCol].color});
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        break;

    case MoveType::QUEENPROMOCAPTURE:
        DeletePiece(move.endRow, move.endCol, pieces[move.endRow][move.endCol]);
        AddPiece(move.endRow, move.endCol, {PieceType::QUEEN, pieces[move.iniRow][move.iniCol].color});
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        break;

    case MoveType::KNIGHTPROMOCAPTURE:
        DeletePiece(move.endRow, move.endCol, pieces[move.endRow][move.endCol]);
        AddPiece(move.endRow, move.endCol, {PieceType::KNIGHT, pieces[move.iniRow][move.iniCol].color});
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        break;

    case MoveType::ROOKPROMOCAPTURE:
        DeletePiece(move.endRow, move.endCol, pieces[move.endRow][move.endCol]);
        AddPiece(move.endRow, move.endCol, {PieceType::ROOK, pieces[move.iniRow][move.iniCol].color});
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        break;

    case MoveType::BISHOPPROMOCAPTURE:
        DeletePiece(move.endRow, move.endCol, pieces[move.endRow][move.endCol]);
        AddPiece(move.endRow, move.endCol, {PieceType::BISHOP, pieces[move.iniRow][move.iniCol].color});
        DeletePiece(move.iniRow, move.iniCol, pieces[move.iniRow][move.iniCol]);
        break;

    default:
        break;
    }
}

// returns the index in the array of the piece
int Zobrist::PieceToNumber(PieceInfo piece) const
{
    int value = 0;
    switch (piece.type)
    {
    case PieceType::PAWN:
        value = 0;
        break;
    case PieceType::KNIGHT:
        value = 1;
        break;
    case PieceType::BISHOP:
        value = 2;
        break;
    case PieceType::ROOK:
        value = 3;
        break;
    case PieceType::QUEEN:
        value = 4;
        break;
    case PieceType::KING:
        value = 5;
        break;

    default:
        // program should crash with this value
        value = -7;
        break;
    }

    if (piece.color == PieceColor::BLACK)
        value += 6;

    return value;
}