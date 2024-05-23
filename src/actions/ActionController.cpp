#include "ActionController.hpp"
#include "Controller.hpp"

ActionController::ActionController(Controller &controller)
    : controller(controller)
{
}

void ActionController::RotateBoard()
{
    controller.RotateBoard();
}

void ActionController::MakeMove(const Move &move)
{
    controller.MakeMove(move);
}

void ActionController::MakeIAmove()
{
    controller.MakeIAmove();
}

void ActionController::StartGame(const std::string &FEN)
{
    controller.StartGame(FEN);
}