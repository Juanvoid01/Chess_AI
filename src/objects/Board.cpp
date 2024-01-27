#include "Board.hpp"

Board::Board()
{

}

Board::~Board()
{
}

void Board::render()
{
    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            squares[row][col].Render();
        }
    }
}
