#include "Controller.hpp"

Controller::Controller(const Renderer &renderer)
    : renderer(renderer)
{

    informationText = std::make_unique<InformationText>(800.f, 150.f, renderer);
    board = std::make_unique<Board>(0.0f, 0.0f, 550.0f, 550.0f, renderer, chessAI, *informationText);
    actionController = std::make_unique<ActionController>(*this);
    actions = std::make_unique<Actions>(*actionController);
    informationText->SetScale(1.3f, 1.3f);
    // board.LoadFEN(FEN_QUEEN_VS_PAWN_ENDGAME);

    board->SetCenter(renderer.GetWindowWidth() / 2.f, renderer.GetWindowHeight() / 2.f);
}

void Controller::Update()
{
    actions->ExecuteActions();
}
void Controller::Render()
{
    board->Render();
    informationText->Render();
}
void Controller::AddAction(Action *action) { actions->AddAction(action); }

void Controller::MakeMove(const Move &move)
{
}
void Controller::MakeIAmove()
{
}