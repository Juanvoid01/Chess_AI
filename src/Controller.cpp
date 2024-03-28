#include "Controller.hpp"

Controller::Controller(const Renderer &renderer)
    : renderer(renderer),
      informationText(800.f, 150.f, renderer),
      board(0.0f, 0.0f, 550.0f, 550.0f, renderer, chessAI, informationText)
{

    informationText.SetScale(1.3f, 1.3f);
    //board.LoadFEN(FEN_QUEEN_VS_PAWN_ENDGAME);

    board.SetCenter(renderer.GetWindowWidth() / 2.f, renderer.GetWindowHeight() / 2.f);
}

void Controller::Update()
{
}
void Controller::Render()
{
    board.Render();
    informationText.Render();
}