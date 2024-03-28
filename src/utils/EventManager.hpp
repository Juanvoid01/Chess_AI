#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Controller.hpp"

static Renderer *renderer;
static Controller* controller;

class EventManager
{
public:
    EventManager(GLFWwindow *windowd, Renderer *r, Controller* ctrl);
    ~EventManager();

private:
    static void Mouse_click_callback(GLFWwindow *window, int button, int action, int mods);
    static void Key_pressed_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};
