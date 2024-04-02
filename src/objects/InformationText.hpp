#pragma once

#include "TextObject.hpp"

class InformationText : public Object
{
public:
    InformationText(float posX, float posY, const Renderer &r);
    ~InformationText() {}

    void Render();

    void Translate(float x, float y);
    void SetScale(float x, float y);
    void SetPosition(float x, float y);
    void SetCenter(float x, float y);

    inline void SetState(const std::string &&state) { stateMessage.SetText(std::move(state)); }
    inline void SetDepth(const std::string &&depth) { depthSearched.SetText(std::move(depth)); }
    inline void SetNodes(const std::string &&nodes) { nodesVisited.SetText(std::move(nodes)); }
    inline void SetInfo(const std::string &&info) { infoSearch.SetText(std::move(info)); }

private:
    float textHeight = 20.f;

    TextObject stateMessage;
    TextObject depthSearched;
    TextObject nodesVisited;
    TextObject infoSearch;
};