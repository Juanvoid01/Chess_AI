#pragma once
#include "Shader.hpp"
#include <memory>

enum class ShaderName
{
    COLOR = 0,
    TEXTURE = 1
};

class ShaderManager
{
public:
    ShaderManager()
    {
        InitializeShaders();
    };
    ~ShaderManager(){};

    inline unsigned int GetNumShaders() const { return numShaders; }

    inline Shader &GetShader(ShaderName name) { return *shaderArray[IndexOf(name)]; }

private:
    unsigned int numShaders = 2;
    std::unique_ptr<Shader> shaderArray[2];
    inline int IndexOf(ShaderName name) const { return static_cast<int>(name); }

    void InitializeShaders()
    {
        shaderArray[IndexOf(ShaderName::COLOR)] = std::make_unique<Shader>("res/shaders/Color.shader");
        shaderArray[IndexOf(ShaderName::TEXTURE)] = std::make_unique<Shader>("res/shaders/Texture.shader");
    }
};