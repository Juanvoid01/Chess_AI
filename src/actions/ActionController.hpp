#pragma once

// forward declaration
class Controller;
class Move;

class ActionController
{
public:
    ActionController(Controller &controller);
    ~ActionController() {}
    void RotateBoard();
    void MakeMove(const Move& move);
    void MakeIAmove();

private:
    Controller &controller;
};
