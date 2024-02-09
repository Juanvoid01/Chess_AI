#pragma once
#include <vector>
#include "Renderer.hpp"

class Object
{
public:
    Object(float posX, float posY, float width, float height, TextureName texture, const Renderer &r);

    ~Object();

    void Render();

    inline const glm::mat4 &GetMVP() const { return mvp; }

    inline void ReCalculateMVP() { mvp = renderer.GetProjection() * renderer.GetView() * model; }

    void Translate(float x, float y);
    void SetScale(float x, float y);
    void SetPosition(float x, float y);
    void SetCenter(float x, float y);

    inline void SetTexture(TextureName newTexture) { textureName = newTexture; }

    inline float GetWidth() const { return width; }
    inline float GetHeight() const { return height; }
    inline float GetX() const { return x; }
    inline float GetY() const { return y; }

    inline bool PosInside(float px, float py) const
    {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }

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

    glm::mat4 model;
    glm::mat4 mvp;

    TextureName textureName;

    const Renderer &renderer;

    void RecalculateModel();
};