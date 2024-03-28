#include "Renderer.hpp"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLCheckError()
{
    while (GLenum error = glGetError())
    {

        std::cout << "[OpenGL Error] ";
        switch (error)
        {
        case GL_INVALID_ENUM:
            std::cout << "GL_INVALID_ENUM : An unacceptable value is specified for an enumerated argument.";
            break;
        case GL_INVALID_VALUE:
            std::cout << "GL_INVALID_OPERATION : A numeric argument is out of range.";
            break;
        case GL_INVALID_OPERATION:
            std::cout << "GL_INVALID_OPERATION : The specified operation is not allowed in the current state.";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION : The framebuffer object is not complete.";
            break;
        case GL_OUT_OF_MEMORY:
            std::cout << "GL_OUT_OF_MEMORY : There is not enough memory left to execute the command.";
            break;
        case GL_STACK_UNDERFLOW:
            std::cout << "GL_STACK_UNDERFLOW : An attempt has been made to perform an operation that would cause an internal stack to underflow.";
            break;
        case GL_STACK_OVERFLOW:
            std::cout << "GL_STACK_OVERFLOW : An attempt has been made to perform an operation that would cause an internal stack to overflow.";
            break;
        default:
            std::cout << "Unrecognized error" << error;
        }
        std::cout << std::endl;
        return false;
    }
    return true;
}

Renderer::Renderer(int windowWidth, int windowHeight, DrawFunction drawMethod)
    : windowWidth(windowWidth), windowHeight(windowHeight),drawFunction(drawMethod)
{
}

Renderer::~Renderer()
{
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const glm::mat4 &mvp,
                    ShaderName shader, TextureName texture, const glm::vec4 &color) const
{
    if (shader == ShaderName::TEXTURE)
    {
        if (texture == TextureName::EMPTY)
        {
            return;
        }
    }

    shaderManager.GetShader(shader).Bind();
    shaderManager.GetShader(shader).SetUniformMat4f("u_MVP", mvp);

    if (shader == ShaderName::TEXTURE)
    {
        textureManager.GetTexture(texture).Bind();
        shaderManager.GetShader(shader).SetUniform1i("u_Texture", 0);
    }
    else if (shader == ShaderName::COLOR)
    {
        shaderManager.GetShader(shader).SetUniform4f("u_Color", color[0], color[1], color[2], color[3]);
    }

    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

// shows text on screen
void Renderer::DrawText(GLTtext *gltText, const glm::mat4 &mvp) const
{
    (*drawFunction)(gltText, mvp);
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::SetWindowWidth(int width)
{
    windowWidth = width;
}

void Renderer::SetWindowHeight(int height)
{
    windowHeight = height;
}
