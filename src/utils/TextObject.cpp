#include "TextObject.hpp"

TextObject::TextObject(const std::string &&text, float posX, float posY, const Renderer &r)
    : Object(r, glm::vec4(1.f), posX, posY, 1.f, 1.f)
{
    textString = text;
    gltText = gltCreateText();
    gltSetText(gltText, text.c_str());
    Object::SetScale(1.f, -1.f);
}

// shows text on screen
void TextObject::Render()
{
    renderer.DrawText(GetgltString(), GetMVP());
}

void TextObject::SetScale(float x, float y)
{
    Object::SetScale(x, -y); // text is rendered upside down by default
}