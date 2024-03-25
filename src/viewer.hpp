#pragma once

class Viewer
{
public:
    Viewer(){};
    void Run();

private:
    void Render();
    void Update();
    void ShowBoard();
};
