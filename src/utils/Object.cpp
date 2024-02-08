#include "Object.hpp"

Object::Object(float posX, float posY, float width, float height, TextureName texture, const Renderer &r)
    : originalWidth(width), originalHeight(height), textureName(texture), renderer(r)
{
  float positions[16] = {
      0.0f, 0.0f, 0.0f, 0.0f,
      width, 0.0f, 1.0f, 0.0f,
      width, height, 1.0f, 1.0f,
      0.0f, height, 0.0f, 1.0f};

  positionVector = glm::vec3(posX, posY, 0);
  translationVector = glm::vec3(0, 0, 0);
  scaleVector = glm::vec3(1, 1, 1);

  RecalculateModel();

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

void Object::Render()
{
  renderer.Draw(va, ib, mvp, textureName);
}

void Object::Translate(float x, float y)
{
  translationVector.x += x;
  translationVector.y += y;
  RecalculateModel();
  ReCalculateMVP();
}

void Object::SetScale(float x, float y)
{
  scaleVector.x = x;
  scaleVector.y = y;
  RecalculateModel();
  ReCalculateMVP();
}

void Object::SetPosition(float x, float y)
{
  positionVector = glm::vec3(x, y, 0);
  translationVector = glm::vec3(0, 0, 0);
  RecalculateModel();
  ReCalculateMVP();
}

void Object::SetCenter(float x, float y)
{
  positionVector = glm::vec3(x - GetWidth() / 2.f, y - GetHeight() / 2.f, 0);
  translationVector = glm::vec3(0, 0, 0);

  RecalculateModel();
  ReCalculateMVP();
}

void Object::RecalculateModel()
{
  model = glm::translate(glm::mat4(1.0f), positionVector);
  model = glm::translate(model, translationVector);
  model = glm::scale(model, scaleVector);

  width = originalWidth * scaleVector.x;
  height = originalHeight * scaleVector.y;
  x = positionVector.x + translationVector.x;
  y = positionVector.y + translationVector.y;
}
