// 2022/5/8 19:25:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f7xx_hal.h>


#define LCD_BL_Pin GPIO_PIN_3
#define LCD_BL_GPIO_Port GPIOA



void HAL_GPIO::Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __GPIOE_CLK_ENABLE();
    __GPIOI_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    __GPIOH_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = LCD_BL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(LCD_BL_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = LCD_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(LCD_RST_GPIO_Port, &GPIO_InitStruct);


    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}
