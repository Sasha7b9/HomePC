// 2022/5/7 16:50:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


struct HLine
{
    int width;

    HLine(int _width) : width(_width) {}

    void Draw(int x, int y, const Color &color = Color::NONE);
};


struct VLine
{
    int height;

    VLine(int _height) : height(_height) {}

    void Draw(int x, int y, const Color &color = Color::NONE);
};


struct Circle
{
    int radius;

    Circle(int _radius) : radius(_radius) {}

    void Fill(int x, int y, const Color &color = Color::NONE);
};


struct Region
{
    int width;
    int height;

    Region(int w, int h) : width(w), height(h) { }

    void Fill(int x, int y, const Color &color = Color::NONE);
};
