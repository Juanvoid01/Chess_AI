#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:
    Shader(const std::string &filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set uniforms
    void SetUniform1i(const std::string &name, int value);
    void SetUniform1f(const std::string &name, float value);
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);

private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;

    unsigned int CompileShader(unsigned int type, const std::string &source);
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
    ShaderProgramSource ParseShader(const std::string &filePath);

    int GetUniformLocation(const std::string &name) const;
};
