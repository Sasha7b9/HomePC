// 2022/5/7 18:04:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors.h"
#include <cstdlib>


Color Color::current = Color(Color::_WHITE);

Color Color::NONE(Color::_Count);

Color Color::BLUE(Color::_BLUE);
Color Color::GREEN(Color::_GREEN);
Color Color::RED(Color::_RED);
Color Color::MAGENTA(Color::_MAGENTA);
Color Color::YELLOW(Color::_YELLOW);
Color Color::WHITE(Color::_WHITE);
Color Color::BLACK(Color::_BLACK);


Color Color::GetRandom()
{
    return Color((Color::E)(std::rand() % 7));
}
