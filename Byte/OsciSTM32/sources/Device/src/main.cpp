// 2022/5/8 19:24:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"



int main(void)
{
    HAL::Init();

    Display::Init();

    while (1)
    {
        Display::Update();
    }
}
