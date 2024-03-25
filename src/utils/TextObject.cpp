#include "TextObject.hpp"

TextObject::TextObject(const std::string &&text, float posX, float posY, const Renderer &r, DrawFunction drawfunction)
    : Object(r, glm::vec4(1.f), posX, posY, 1.f, 1.f), drawFunction(drawfunction)
{
    textString = text;
    gltText = gltCreateText();
    gltSetText(gltText, text.c_str());
    Object::SetScale(1.f, -1.f);
}

// shows text on screen
void TextObject::Render()
{
    (*drawFunction)(*this);
}

void TextObject::SetScale(float x, float y)
{
    Object::SetScale(x, -y); // text is rendered upside down by default
}