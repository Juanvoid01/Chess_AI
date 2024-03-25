#pragma once

#include "Object.hpp"

#define GLT_IMPLEMENTATION
#include "gltext.h"


class TextObject : public Object
{
public:

    typedef void (*DrawFunction)(const TextObject&);

    //using DrawFunction = void (*)(const TextObject&);

    TextObject(const std::string &&text, float posX, float posY, const Renderer &r, DrawFunction drawfunction);
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
    void SetScale(float x, float y);
private:
    std::string textString;
    GLTtext *gltText;

    //Need the function pointer to the draw Text Method
    DrawFunction drawFunction;
};