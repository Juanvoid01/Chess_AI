#include "Object.hpp"

Object::Object(const Renderer &r, TextureName texture, float posX, float posY, float width, float height)
    : originalWidth(width), originalHeight(height), textureName(texture),
      renderer(r), color(1.f), shaderName(ShaderName::TEXTURE)
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

Object::Object(const Renderer &r, glm::vec4 color, float posX, float posY, float width, float height)
    : originalWidth(width), originalHeight(height), textureName(TextureName::EMPTY),
      renderer(r), color(color), shaderName(ShaderName::COLOR)
{
  float positions[16] = {
      0.0f, 0.0f, 0.0f, 0.0f,
      width, 0.0f, 1.0f, 0.0f,
      width, height, 1.0f, 1.0f,
      0.0f, height, 0.0f, 1.0f};

  velocity = glm::vec3(0, 0, 0);
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
  renderer.Draw(va, ib, mvp, shaderName, textureName, color);
}

void Object::Update()
{
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

void Object::SetVelocity(float x, float y)
{
  velocity.x = x;
  velocity.y = y;
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

// Calculate direction vector of the object to the point Dest, then normalize the vector
void Object::DirectionTo(float destX, float destY, float &dirX, float &dirY) const
{

  dirX = destX - GetX();
  dirY = destY - GetY();


  float length = std::sqrt(dirX * dirX + dirY * dirY);

  // Normalize the direction vector
  if (length != 0)
  {
    dirX /= length;
    dirY /= length;
  }
}

// Calculate distance to the given point
float Object::DistanceTo(float pointX, float pointY) const
{
  float dx = pointX - GetX();
  float dy = pointY - GetY();

  return std::sqrt(dx * dx + dy * dy);
}
