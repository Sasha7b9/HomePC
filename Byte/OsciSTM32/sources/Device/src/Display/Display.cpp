// 2022/5/7 18:05:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/Touch/Touch.h"
#include "Hardware/Timer.h"
#include "Display/Primitives.h"
#include "Display/Font/Font.h"
#include "Display/Colors.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>


namespace Display
{
    TS_StateTypeDef  TS_State = { 0 };
    uint8_t TouchPoit;
    uint8_t value = 0;
    uint16_t i;

    void BeginScene(const Color &);

    void EndScene();

    void Update1();
}


void Display::Init()
{
    Touch::Init();

//    BeginScene(Color::BLACK);

//    Color::BLUE.SetAsCurrent();
    Font::Set(&Font24);
//    LCD::DisplayStringAt(0, 0, (uint8_t *)"Capacitive touch screen test", CENTER_MODE);
//  HLine(1024).Draw(0, 30, Color::WHITE);
}


void Display::BeginScene(const Color &color)
{
    Region(WIDTH, HEIGHT).Fill(0, 0, color);
}


void Display::EndScene()
{
    HAL_LTDC::CopyBackBuffer();
}


void Display::Update1()
{
    static const Color PointColor[] =
    {
        Color::BLUE,
        Color::GREEN,
        Color::RED,
        Color::MAGENTA,
        Color::YELLOW
    };

    static TimeMeter meter;

    if (Touch::GetState(&TS_State))
    {
        TouchPoit = TS_State.touchDetected;

        for (i = 0; i < 5; i++)
        {
            if (TouchPoit & 0x01)
            {
                if (TS_State.touchY[i] < 75)TS_State.touchY[i] = 75;
                if (TS_State.touchY[i] > 580)TS_State.touchY[i] = 580;
                if (TS_State.touchX[i] < 20)TS_State.touchX[i] = 20;
                if (TS_State.touchX[i] > 1004)TS_State.touchX[i] = 1004;

                Circle(20).Fill(TS_State.touchX[i], TS_State.touchY[i], PointColor[i]);
                VLine(580).Draw(TS_State.touchX[i], 55);
                HLine(1004).Draw(5, TS_State.touchY[i]);
            }

            TouchPoit >>= 1;
        }

        meter.Reset();
    }
    else
    {
        if (meter.ElapsedTime() > 5)
        {
            Region(1024, 550).Fill(0, 50, Color::BLACK);
        }
    }
}


static void DrawLines()
{
//    static TimeMeter meter;

    static const int x0 = 10;
    static const int y0 = 10;

    static int delta = 0;

//    if (meter.ElapsedTime() > 100)
//    {
//        meter.Reset();
//
//        delta++;
//
//        if (delta > 100)
//        {
//            delta = 0;
//        }
//    }

    int k = 1;

    for (int i = 0; i < 250; i++)
    {
//        VLine(i).Draw(x0 + delta + k * i, y0, Color::WHITE);

        HLine(i).Draw(x0, y0 + delta + k * i, Color::WHITE);
    }
}


/*
static void DrawPoints()
{
    static TimeMeter meter;


}
*/


void Display::Update()
{
    BeginScene(Color::BLACK);

    DrawLines();

    EndScene();
}
