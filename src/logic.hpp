#pragma once

#include "controller.hpp"

class Logic
{
public:
    Logic(const Controller &c) : controller(c){};
    void run();

private:
    const Controller &controller;
};