#include "Object.hpp"

Object::Object(float posX, float posY, float width, float height, TextureName texture, const Renderer &r)
    : textureName(texture), renderer(r)
{
  float positions[16] = {
      posX, posY, 0.0f, 0.0f,
      posX + width, posY, 1.0f, 0.0f,
      posX + width, posY + height, 1.0f, 1.0f,
      posX, posY + height, 0.0f, 1.0f};

  model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
  ReCalculateMVP();

  VertexBuffer vb(positions, 4 * 4 * sizeof(float));

  VertexBufferLayout layout;
  layout.PushFloat(2);
  layout.PushFloat(2);
  va.AddBuffer(vb, layout);

  va.Unbind();
  vb.Unbind();
  ib.Unbind();
}

Object::~Object()
{
}

void Object::Render(const Renderer &renderer)
{
  renderer.Draw(va, ib, mvp, textureName);
}

void Object::Translate(float x, float y)
{
  model = glm::translate(model, glm::vec3(x, y, 0));
  ReCalculateMVP();
}

void Object::Scale(float x, float y)
{
  model = glm::scale(model, glm::vec3(x, y, 0));
  ReCalculateMVP();
}