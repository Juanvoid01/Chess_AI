#include "Board.hpp"

Board::Board(float posX, float posY, float width, float height, const Renderer& r)
    :Object(posX,posY,width,height, TextureName::BOARD, r)
{

}

Board::~Board()
{
}

void Board::Render(const Renderer& renderer)
{
    Object::Render(renderer);

    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            squares[row][col].Render();
        }
    }
}
