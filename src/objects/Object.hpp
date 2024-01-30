#pragma once
#include <vector>

class Object
{
public:
    Object(std::vector<float> v, std::vector<float> i)
        :vertices(v),indices(i)
    {
    
    }

    ~Object();

    void Render();

    std::vector<float> vertices;
    std::vector<float> indices;
};