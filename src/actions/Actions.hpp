#pragma once

#include <vector>

#include "NoneAction.hpp"
#include "AIaction.hpp"
#include "RotateBoardAction.hpp"


class Actions
{
public:
    Actions(ActionController &controller) : controller(controller) {}

    // Release memory of Action objects
    ~Actions()
    {
        for (Action *action : actions)
        {
            if (action != nullptr)
                delete action;
        }
    }

    // Method to add actions to the vector
    inline void AddAction(Action *action)
    {
        actions.emplace_back(action);
    }

    // Execute all actions in the vector, then clear the actions vector
    inline void ExecuteActions()
    {
        for (Action *action : actions)
        {
            action->execute(controller);
        }

        actions.clear();
    }

    // Clear the actions in the vector
    inline void Clear()
    {
        actions.clear();
    }

private:
    std::vector<Action *> actions;
    ActionController &controller;
};
