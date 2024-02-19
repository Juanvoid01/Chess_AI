#pragma once
#include "Texture.hpp"
#include <memory>

enum class TextureName
{
    EMPTY = -1,
    BOARD = 0,
    WPAWN = 1,
    BPAWN = 2,
    WKING = 3,
    BKING = 4,
    WQUEEN = 5,
    BQUEEN = 6,
    WKNIGHT = 7,
    BKNIGHT = 8,
    WROOK = 9,
    BROOK = 10,
    WBISHOP = 11,
    BBISHOP = 12,
    CHECKMATE = 13,
    STALEMATE = 14
};

class TextureManager
{
public:
    TextureManager()
    {
        InitializeTextures();
    };
    ~TextureManager(){};

    inline unsigned int GetNumTextures() const { return numTextures; }

    inline const Texture &GetTexture(TextureName name) const { return *textureArray[IndexOf(name)]; }

private:
    unsigned int numTextures = 15;
    std::unique_ptr<Texture> textureArray[15];
    inline int IndexOf(TextureName name) const { return static_cast<int>(name); }

    void InitializeTextures()
    {
        textureArray[IndexOf(TextureName::BOARD)] = std::make_unique<Texture>("res/textures/board.png");
        textureArray[IndexOf(TextureName::WPAWN)] = std::make_unique<Texture>("res/textures/wP.png");
        textureArray[IndexOf(TextureName::BPAWN)] = std::make_unique<Texture>("res/textures/bP.png");
        textureArray[IndexOf(TextureName::WKING)] = std::make_unique<Texture>("res/textures/wK.png");
        textureArray[IndexOf(TextureName::BKING)] = std::make_unique<Texture>("res/textures/bK.png");
        textureArray[IndexOf(TextureName::WQUEEN)] = std::make_unique<Texture>("res/textures/wQ.png");
        textureArray[IndexOf(TextureName::BQUEEN)] = std::make_unique<Texture>("res/textures/bQ.png");
        textureArray[IndexOf(TextureName::WKNIGHT)] = std::make_unique<Texture>("res/textures/wN.png");
        textureArray[IndexOf(TextureName::BKNIGHT)] = std::make_unique<Texture>("res/textures/bN.png");
        textureArray[IndexOf(TextureName::WROOK)] = std::make_unique<Texture>("res/textures/wR.png");
        textureArray[IndexOf(TextureName::BROOK)] = std::make_unique<Texture>("res/textures/bR.png");
        textureArray[IndexOf(TextureName::WBISHOP)] = std::make_unique<Texture>("res/textures/wB.png");
        textureArray[IndexOf(TextureName::BBISHOP)] = std::make_unique<Texture>("res/textures/bB.png");
        textureArray[IndexOf(TextureName::CHECKMATE)] = std::make_unique<Texture>("res/textures/checkmate.png");
        textureArray[IndexOf(TextureName::STALEMATE)] = std::make_unique<Texture>("res/textures/stalemate.png");
    }
};