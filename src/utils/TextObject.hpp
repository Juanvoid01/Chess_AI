#pragma once

#include "Object.hpp"

#define GLT_IMPLEMENTATION
#include "gltext.h"

class TextObject : public Object
{
public:
    TextObject(const std::string &&text, float posX, float posY, float width, float height, const Renderer &r);
    ~TextObject() { gltDeleteText(gltText); }

    inline void SetText(const std::string &&text)
    {
        gltDeleteText(gltText);
        textString = text;
        gltText = gltCreateText();
        gltSetText(gltText, text.c_str());
    }
    inline const std::string &GetTextString() const { return textString; }
    inline GLTtext *GetgltString() const { return gltText; }
    // shows text on screen
    void Render();

private:
    std::string textString;
    GLTtext *gltText;
};