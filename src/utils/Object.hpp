#pragma once
#include <vector>
#include "Renderer.hpp"

class Object
{
public:
    Object(const Renderer &r, TextureName texture,
           float posX = 0.f, float posY = 0.f, float width = 10.f, float height = 10.f);

    Object(const Renderer &r, glm::vec4 color = glm::vec4(1.f),
           float posX = 0.f, float posY = 0.f, float width = 10.f, float height = 10.f);

    ~Object();

    void Render();

    inline const glm::mat4 &GetMVP() const { return mvp; }

    inline void ReCalculateMVP() { mvp = renderer.GetProjection() * renderer.GetView() * model; }

    void Translate(float x, float y);
    void SetScale(float x, float y);
    void SetPosition(float x, float y);
    void SetCenter(float x, float y);

    inline void SetTexture(TextureName newTexture) { textureName = newTexture; }
    inline void SetShader(ShaderName newShader) { shaderName = newShader; }
    inline void SetColor(float r, float g, float b, float a) { color = glm::vec4(r, g, b, a); }
    inline void SetColor(const glm::vec4 &newColor) { color = newColor; }

    inline float GetWidth() const { return width; }
    inline float GetHeight() const { return height; }
    inline float GetX() const { return x; }
    inline float GetY() const { return y; }
    inline const glm::vec4 &GetColor() const { return color; }

    inline bool PosInside(float px, float py) const
    {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }

protected:
    const Renderer &renderer;
    glm::mat4 mvp;

private:
    float originalWidth;
    float originalHeight;
    float originX = 0.0f;
    float originY = 0.0f;

    float width;
    float height;
    float x;
    float y;

    const unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0};

    VertexArray va;
    IndexBuffer ib = IndexBuffer(indices, 6);

    glm::vec3 positionVector;
    glm::vec3 translationVector;
    glm::vec3 scaleVector;
    glm::vec3 rotationVector;

    glm::vec4 color;

    glm::mat4 model;

    TextureName textureName;
    ShaderName shaderName;

    void RecalculateModel();
};