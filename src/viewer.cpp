#include <iostream>

#include "EventManager.hpp"
#include "viewer.hpp"
#include "Board.hpp"
#include "Perft.hpp"

void Viewer::run()
{
    int result = 0;
    GLFWwindow *window;

    if (!glfwInit())
    {
        result = -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(1024, 720, "Chess AI", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        result = -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "Error! glewInit" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    {

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(windowWidth, windowHeight);
        std::shared_ptr<Board> board = std::make_shared<Board>(0.0f, 0.0f, 550.0f, 550.0f, *renderer);
        //board->LoadFEN(FEN_QUEEN_VS_PAWN_ENDGAME);
        EventManager eventManager(window, renderer, board);

        board->SetCenter(windowWidth / 2, windowHeight / 2);

        // Loop until the user closes the window

        while (!glfwWindowShouldClose(window))
        {
            renderer->Clear();

            board->Render();

            // Swap front and back buffers
            glfwSwapBuffers(window);

            // Poll for and process events
            glfwPollEvents();
        }
    }
    // Cleanup
    glfwTerminate();
}

void Viewer::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Your drawing functions go here

    glfwSwapBuffers(glfwGetCurrentContext());
}
