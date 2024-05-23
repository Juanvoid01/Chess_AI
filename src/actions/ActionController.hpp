#pragma once

#include <string>

// forward declaration
class Controller;
class Move;

class ActionController
{
public:
    ActionController(Controller &controller);
    ~ActionController() {}
    void RotateBoard();
    void MakeMove(const Move &move);
    void MakeIAmove();
    void StartGame(const std::string &FEN);

private:
    Controller &controller;
};
