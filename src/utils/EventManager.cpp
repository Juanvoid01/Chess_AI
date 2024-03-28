#include "EventManager.hpp"

EventManager::EventManager(GLFWwindow *window, Renderer *r, Controller *ctrl)
{
    renderer = r;
    controller = ctrl;

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetMouseButtonCallback(window, Mouse_click_callback);

    glfwSetKeyCallback(window, Key_pressed_callback);
}

EventManager::~EventManager()
{
}

void EventManager::Mouse_click_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        ypos = renderer->GetWindowHeight() - ypos;

        Move move = controller->GetBoard().ClickEvent(xpos, ypos);

        if (move.IsValid())
        {
            controller->AddAction(new MovePieceAction(move));
        }
    }
}

void EventManager::Key_pressed_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    if (key == GLFW_KEY_R)
    {
        controller->AddAction(new RotateBoardAction());
    }
    else if (key == GLFW_KEY_P)
    {
        controller->AddAction(new AIPieceAction());
    }
}

void EventManager::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    renderer->SetWindowWidth(width);
    renderer->SetWindowWidth(height);
}
