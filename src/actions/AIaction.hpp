#pragma once
#include "Action.hpp"

class AIPieceAction : public Action
{
public:
    AIPieceAction()
        : Action(Type::IA)
    {
    }
    void execute(ActionController& controller) override
    {
        controller.MakeIAmove();
    }

};