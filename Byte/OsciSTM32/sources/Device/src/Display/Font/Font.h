// 2022/5/7 17:53:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct sFONT
{
    const uint8 *table;
    uint16       Width;
    uint16       Height;
};


namespace Font
{
    void   Set(sFONT *fonts);
    sFONT *Get();
}


extern sFONT Font24;
extern sFONT Font20;
extern sFONT Font16;
extern sFONT Font12;
extern sFONT Font8;

#define LINE(x) ((x) * (((sFONT *)Font::Get())->Height))
