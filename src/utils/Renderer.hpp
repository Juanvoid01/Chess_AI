#pragma once

#include <GL/glew.h>
#include <assert.h>

#include "TextureManager.hpp"
#include "ShaderManager.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#define GLT_IMPLEMENTATION
#include "gltext.h"

#define ASSERT(x) \
    if (!(x))     \
    assert(false)

#define GLCall(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLCheckError())

void GLClearError();
bool GLCheckError();


class Renderer
{
public:
    typedef void (*DrawFunction)(GLTtext *, const glm::mat4 &);

    Renderer(float windowWidth, float windowHeight, DrawFunction drawMethod);
    ~Renderer();
    void Draw(const VertexArray &va, const IndexBuffer &ib, const glm::mat4 &mvp,
              ShaderName shader, TextureName texture = TextureName::EMPTY, const glm::vec4 &color = glm::vec4(1.f)) const;

    // shows text on screen
    void DrawText(GLTtext *gltText, const glm::mat4 &mvp) const;

    void Clear() const;

    inline const glm::mat4 &GetProjection() const { return proj; }
    inline const glm::mat4 &GetView() const { return view; }

    inline float GetWindowWidth() const { return windowWidth; }
    inline float GetWindowHeight() const { return windowHeight; }

    inline float GetOriginalWindowWidth() const { return originalWidth; }
    inline float GetOriginalWindowHeight() const { return originalHeight; }

    void SetWindowWidth(int width);
    void SetWindowHeight(int height);

    // Need the function pointer to the draw Text Method
    DrawFunction drawFunction;
private:
    float windowWidth;
    float windowHeight;

    float originalWidth;
    float originalHeight;

    glm::mat4 proj;
    glm::mat4 view;

    mutable ShaderManager shaderManager;
    TextureManager textureManager;
};