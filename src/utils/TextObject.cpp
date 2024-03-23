#include "TextObject.hpp"

TextObject::TextObject(const std::string &&text, float posX, float posY, float width, float height, const Renderer &r)
    : Object(r, glm::vec4(1.f), posX, posY, width, height)
{
    textString = text;
    gltText = gltCreateText();
    gltSetText(gltText, text.c_str());
}

// shows text on screen
void TextObject::Render()
{
}