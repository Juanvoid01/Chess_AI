#include <iostream>

#include "viewer.hpp"
#include "utils/VertexArray.hpp"
#include "utils/IndexBuffer.hpp"
#include "utils/Shader.hpp"
#include "utils/Renderer.hpp"
#include "utils/Texture.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
    window = glfwCreateWindow(640, 480, "Chess AI", NULL, NULL);
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

        float positions[] = {
            -0.5f,
            -0.5f,
            0.5f,
            -0.5f,
            0.5f,
            0.5f,
            -0.5f,
            0.5f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0};

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.PushFloat(2);
        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/board.png");

        texture.Bind(); // Pass the same argument in Bind(slot) as in SetUniform1i("",slot)
        shader.SetUniform1i("u_Texture", 0);

        
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;

        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
            {
                increment = -0.05f;
            }
            else if (r < 0.0f)
            {
                increment = 0.05f;
            }

            r += increment;

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
