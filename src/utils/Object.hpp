#pragma once
#include <vector>
#include "Renderer.hpp"

class Object
{
public:
    Object(float posX, float posY, float width, float height, TextureName texture, const Renderer &r);

    ~Object();

    void Render(const Renderer &renderer);

    inline float GetWidth() const { return width; }
    inline float GetHeight() const { return height; }
    inline const glm::mat4 &GetMVP() const { return mvp; }

    inline void ReCalculateMVP() { mvp = renderer.GetProjection() * renderer.GetView() * model; }

    void Translate(float x, float y);
    void SetScale(float x, float y);
    void SetPosition(float x, float y);
    void SetCenter(float x, float y);

    inline void SetTexture(TextureName newTexture) { textureName = newTexture; }

protected:
    float width;
    float height;

private:
    const unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0};

    VertexArray va;
    IndexBuffer ib = IndexBuffer(indices, 6);
    glm::mat4 origin;
    glm::mat4 model;
    glm::mat4 mvp;

    TextureName textureName;

    const Renderer &renderer;
};