#include "InformationText.hpp"

InformationText::InformationText(float posX, float posY, const Renderer &r)
    : Object(r, TextureName::BOARD, posX, posY, 1.f, 1.f),
      stateMessage("Search not started", posX, posY, r),
      depthSearched("Depth : 0", posX, posY - textHeight, r),
      nodesVisited("Nodes : 0", posX, posY - 2 * textHeight, r)
{
}

void InformationText::Render()
{
  stateMessage.Render();
  depthSearched.Render();
  nodesVisited.Render();
}

void InformationText::Translate(float x, float y)
{
  Object::Translate(x, y);
  stateMessage.Translate(x, y);
  depthSearched.Translate(x, y);
  nodesVisited.Translate(x, y);
}
void InformationText::SetScale(float x, float y)
{
  Object::SetScale(x, y);
  stateMessage.SetScale(x, y);
  depthSearched.SetScale(x, y);
  nodesVisited.SetScale(x, y);
}

void InformationText::SetPosition(float x, float y)
{
  Object::SetPosition(x, y);

  stateMessage.SetPosition(x, y);
  depthSearched.SetPosition(x, y - textHeight);
  nodesVisited.SetPosition(x, y - 2 * textHeight);
}
void InformationText::SetCenter(float x, float y)
{
  Object::SetCenter(x, y);

  stateMessage.SetCenter(x, y);
  depthSearched.SetCenter(x, y - textHeight);
  nodesVisited.SetCenter(x, y - 2 * textHeight);
}
