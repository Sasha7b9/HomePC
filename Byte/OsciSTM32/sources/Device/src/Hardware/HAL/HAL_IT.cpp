#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Touch/Touch.h"
#include <stm32f7xx_hal.h>


void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}


void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
}

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    Touch::CallbackOnInterrupt(pin);
}
