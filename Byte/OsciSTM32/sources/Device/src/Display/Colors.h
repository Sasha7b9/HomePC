// 2022/5/7 18:04:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Color
{
private:

    enum E
    {
        _BLUE,
        _GREEN,
        _RED,
        _MAGENTA,
        _YELLOW,
        _WHITE,
        _BLACK,
        _Count
    };

    static Color current;

public:

    Color(E i) : index(i) {}

    static Color NONE;

    static Color BLUE;
    static Color GREEN;
    static Color RED;
    static Color MAGENTA;
    static Color YELLOW;
    static Color WHITE;
    static Color BLACK;

    uint16 index;

    void SetAsCurrent() const;

    uint Value() const;

    static Color GetCurrent() { return current; };

    static const int Count = 256;

    static Color GetRandom();
};


extern uint colors[Color::Count];

#define COLOR(x) colors[x]
