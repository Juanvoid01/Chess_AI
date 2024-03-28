#pragma once
#include "Action.hpp"

class RotateBoardAction : public Action
{
public:
    RotateBoardAction()
        : Action(Type::ROTATEBOARD)
    {
    }
    void execute(ActionController& controller) override
    {
        controller.RotateBoard();
    }
};