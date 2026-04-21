// 2022/4/24 22:31:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define LCD_RST_Pin GPIO_PIN_11
#define LCD_RST_GPIO_Port GPIOD


namespace HAL
{
    void Init();
}


namespace HAL_DMA2D
{
    void Init(void);
}


namespace HAL_FMC
{
    void Init(void);
}


namespace HAL_GPIO
{
    void Init(void);
}


namespace HAL_I2C
{
    void Init(void);

    extern void *handle;          // I2C_HandleTypeDef
}


namespace HAL_LTDC
{
    void Init(void);

    void CopyBackBuffer();
}


namespace HAL_SDRAM
{
    static const uint ADDRESS = 0xD0000000;
    static const uint SIZE = 0x800000;
    bool Test();
}


namespace HAL_UART1
{
    void Init(void);

    extern void *handle;       // UART_HandleTypeDef
}



#ifdef __cplusplus
extern "C" {
#endif

    void SysTick_Handler(void);

    void EXTI9_5_IRQHandler(void);


#ifdef __cplusplus
}
#endif

