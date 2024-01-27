#pragma once

#include <vector>
#include <GL/glew.h>

enum class VBuffLayoutType
{
    UINT,
    FLOAT,
    UCHAR
};

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(VBuffLayoutType type)
    {
        switch (type)
        {
        case VBuffLayoutType::UINT:
            return sizeof(GLuint);
            break;
        case VBuffLayoutType::UCHAR:
            return sizeof(GLbyte);
            break;
        case VBuffLayoutType::FLOAT:
            return sizeof(GLfloat);
            break;
        default:
            return 0;
            break;
        }
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout()
        : m_Stride(0){};

    void Push(VBuffLayoutType type, unsigned int count)
    {
        switch (type)
        {
        case VBuffLayoutType::UINT:
            m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
            break;
        case VBuffLayoutType::UCHAR:
            m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
            break;
        case VBuffLayoutType::FLOAT:
            m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
            break;
        default:
            break;
        }
        m_Stride += count * VertexBufferElement::GetSizeOfType(type);
    }

    inline const std::vector<VertexBufferElement> &GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }

    VBuffLayoutType GLtypeToVBuffLayoutType(unsigned int Gltype) const
    {
        switch (Gltype)
        {
        case GL_FLOAT:
            return VBuffLayoutType::FLOAT;
            break;
        case GL_UNSIGNED_INT:
            return VBuffLayoutType::UINT;
            break;
        case GL_UNSIGNED_BYTE:
            return VBuffLayoutType::UCHAR;
            break;
        default:
            return VBuffLayoutType::UCHAR;
            break;
        }
    }

private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
};