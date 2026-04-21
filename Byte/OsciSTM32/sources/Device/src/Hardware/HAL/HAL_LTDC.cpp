// 2022/5/7 20:54:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/stm32746g_sdram.h"
#include "Display/Display.h"
#include "Display/Colors.h"
#include "Display/Primitives.h"
#include <stm32f7xx_hal.h>


uint colors[Color::Count] =
{
    0x000000FF,  // BLUE,
    0x07E0,      // GREEN,
    0xF800,      // RED,
    0xF81F,      // MAGENTA,
    0xFFE0,      // YELLOW,
    0xFFFFFFFF,  // WHITE,
    0x00000000   // BLACK, 
};


uint Color::Value() const
{
    return colors[index];
}


namespace HAL_LTDC
{
    LTDC_HandleTypeDef handle;

    static uint frontBuffer = SDRAM_DEVICE_ADDR;
    static uint backBuffer = frontBuffer + 1024 * 600 + 6;
}


namespace HAL_DMA2D
{
    DMA2D_HandleTypeDef handle;
}


void HAL_DMA2D::Init(void)
{
    handle.Instance = DMA2D;
    handle.Init.Mode = DMA2D_M2M;
    handle.Init.ColorMode = LTDC_PIXEL_FORMAT_L8;
    handle.Init.OutputOffset = 0;
    handle.XferCpltCallback = nullptr;
    handle.LayerCfg[1].InputOffset = 0;
    handle.LayerCfg[1].InputColorMode = LTDC_PIXEL_FORMAT_L8;
    handle.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    handle.LayerCfg[1].InputAlpha = 0xFF;

    HAL_DMA2D_Init(&handle);

    HAL_DMA2D_ConfigLayer(&handle, 1);
}


void HAL_LTDC::CopyBackBuffer()
{
    HAL_DMA2D::handle.Instance = DMA2D;
    HAL_DMA2D::handle.Init.Mode = DMA2D_M2M;
    HAL_DMA2D::handle.Init.ColorMode = LTDC_PIXEL_FORMAT_L8;    // DMA2D_OUTPUT_ARGB8888;
    HAL_DMA2D::handle.Init.OutputOffset = 0;
    HAL_DMA2D::handle.XferCpltCallback = nullptr;
    HAL_DMA2D::handle.LayerCfg[0].InputOffset = 0;
    HAL_DMA2D::handle.LayerCfg[0].InputColorMode = DMA2D_INPUT_L8;
    HAL_DMA2D::handle.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    HAL_DMA2D::handle.LayerCfg[0].InputAlpha = 0xFF;

    HAL_DMA2D_Init(&HAL_DMA2D::handle);

    HAL_DMA2D_ConfigLayer(&HAL_DMA2D::handle, 0);

    HAL_DMA2D_Start(&HAL_DMA2D::handle, backBuffer, frontBuffer, Display::WIDTH, Display::HEIGHT);

    HAL_DMA2D_PollForTransfer(&HAL_DMA2D::handle, 100);
}


void Region::Fill(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    HLine line(width);

    if ((x % 2) == 0 && (width % 2) == 0)
    {
        for (int i = 0; i < height; i++)
        {
            line.Draw(x, y + i);
        }
    }
    else
    {
        if (x % 2)
        {
//            VLine(height).Draw(x, y);

            if (width % 2)
            {
                Region(width - 1, height).Fill(x + 1, y);
            }
            else
            {
                Region(width - 2, height).Fill(x + 1, y);

//                VLine(height).Draw(x + width - 2, y);
            }
        }
        else
        {
            Region(width - 1, height).Fill(x, y);

//            VLine(height).Draw(x + width - 1, y);
        }
    }
}


void VLine::Draw(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    uint8 *address = (uint8 *)(HAL_LTDC::backBuffer + y * Display::WIDTH + x);

    uint8 value = Color::GetCurrent().index;

    for (int i = 0; i < height; i++)
    {
        *address = value;
        address += Display::WIDTH;
    }
}


void HLine::Draw(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    if ((x % 2) == 0)
    {
        if ((width % 2) == 0)
        {
            uint16 *address = (uint16 *)(HAL_LTDC::backBuffer + y * Display::WIDTH + x);

            uint16 value = (Color::GetCurrent().index << 8) + Color::GetCurrent().index;

            for (int i = 0; i < width / 2; i++)
            {
                *address++ = value;
            }
        }
    }

    /*
    uint8 *address = (uint8 *)(HAL_LTDC::backBuffer + y * Display::WIDTH + x);

    uint8 value = Color::GetCurrent().index;

    for (int i = 0; i < width; i++)
    {
        *address++ = value;
    }
    */
}


void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d)
{
    if (hdma2d->Instance == DMA2D)
    {
        __DMA2D_CLK_ENABLE();
    }
}


void HAL_LTDC::Init(void)
{
    LTDC_LayerCfgTypeDef layer_front;
    LTDC_LayerCfgTypeDef layer_back;

    handle.Instance = LTDC;
    handle.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    handle.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    handle.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    handle.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

    handle.Init.HorizontalSync = 0;
    handle.Init.AccumulatedActiveW = 1183;
    handle.Init.TotalWidth = 1366;

    handle.Init.VerticalSync = 0;
    handle.Init.AccumulatedActiveH = 700;
    handle.Init.TotalHeigh = 800;

    handle.Init.Backcolor.Blue = 0;
    handle.Init.Backcolor.Green = 0;
    handle.Init.Backcolor.Red = 0;

    HAL_LTDC_Init(&handle);

    layer_front.WindowX0 = 0;
    layer_front.WindowX1 = 1024;
    layer_front.WindowY0 = 0;
    layer_front.WindowY1 = 600;
    layer_front.PixelFormat = LTDC_PIXEL_FORMAT_L8;
    layer_front.Alpha = 0xFF;
    layer_front.Alpha0 = 0xFF;
    layer_front.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    layer_front.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    layer_front.FBStartAdress = frontBuffer;
    layer_front.ImageWidth = 1024;
    layer_front.ImageHeight = 600;
    layer_front.Backcolor.Blue = 0;
    layer_front.Backcolor.Green = 0;
    layer_front.Backcolor.Red = 0;

    HAL_LTDC_ConfigLayer(&handle, &layer_front, 0);

    HAL_LTDC_ConfigCLUT(&handle, &COLOR(0), Color::Count, 0);

    HAL_LTDC_EnableCLUT(&handle, 0);

    layer_back.WindowX0 = 0;
    layer_back.WindowX1 = 1024;
    layer_back.WindowY0 = 0;
    layer_back.WindowY1 = 600;
    layer_back.PixelFormat = LTDC_PIXEL_FORMAT_L8;
    layer_back.Alpha = 0xFF;
    layer_back.Alpha0 = 0;
    layer_back.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    layer_back.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    layer_back.FBStartAdress = frontBuffer;
    layer_back.ImageWidth = 1024;
    layer_back.ImageHeight = 600;
    layer_back.Backcolor.Blue = 0;
    layer_back.Backcolor.Green = 0;
    layer_back.Backcolor.Red = 0;

    HAL_LTDC_ConfigLayer(&handle, &layer_back, 1);

    HAL_LTDC_ConfigCLUT(&handle, &COLOR(0), Color::Count, 1);

    HAL_LTDC_EnableCLUT(&handle, 1);
}


void Circle::Fill(int, int, const Color &)
{

}


void Color::SetAsCurrent() const
{
    if (index != Color::NONE.index)
    {
        current = *this;
    }
}


void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (hltdc->Instance == LTDC)
    {
        __LTDC_CLK_ENABLE();

        /**LTDC GPIO Configuration
        PE4     ------> LTDC_B0
        PE5     ------> LTDC_G0
        PE6     ------> LTDC_G1
        PI9     ------> LTDC_VSYNC
        PI10     ------> LTDC_HSYNC
        PF10     ------> LTDC_DE
        PC0     ------> LTDC_R5
        PH2     ------> LTDC_R0
        PH3     ------> LTDC_R1
        PB1     ------> LTDC_R6
        PH8     ------> LTDC_R2
        PH9     ------> LTDC_R3
        PH10     ------> LTDC_R4
        PG6     ------> LTDC_R7
        PG7     ------> LTDC_CLK
        PH13     ------> LTDC_G2
        PH15     ------> LTDC_G4
        PI0     ------> LTDC_G5
        PI1     ------> LTDC_G6
        PI2     ------> LTDC_G7
        PD6     ------> LTDC_B2
        PG10     ------> LTDC_G3
        PG11     ------> LTDC_B3
        PG12     ------> LTDC_B1
        PI4     ------> LTDC_B4
        PI5     ------> LTDC_B5
        PI6     ------> LTDC_B6
        PI7     ------> LTDC_B7
        */
        GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_0 | GPIO_PIN_1          // Работает в режиме DE, поэтому VSYNC и HSYNC не нужны
            | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6
            | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9
            | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    }
}
