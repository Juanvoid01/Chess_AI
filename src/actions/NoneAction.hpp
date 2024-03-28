#pragma once
#include "Action.hpp"

class NoneAction : public Action
{
public:
    NoneAction()
            : Action(Type::NONE)
    {
    }
    void execute(ActionController& controller) override
    {
    }
};