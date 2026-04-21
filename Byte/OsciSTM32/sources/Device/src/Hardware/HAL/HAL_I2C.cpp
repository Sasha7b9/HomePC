// 2022/5/8 19:25:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f7xx_hal.h>

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c4;

/* I2C4 init function */
void MX_I2C4_Init(void)
{

    hi2c4.Instance = I2C4;
    hi2c4.Init.Timing = 0xF0C13545;
    hi2c4.Init.OwnAddress1 = 0;
    hi2c4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c4.Init.OwnAddress2 = 0;
    hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c4);

    /**Configure Analogue filter
    */
    HAL_I2CEx_AnalogFilter_Config(&hi2c4, I2C_ANALOGFILTER_ENABLE);

}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if (hi2c->Instance == I2C4)
    {
        /* USER CODE BEGIN I2C4_MspInit 0 */

        /* USER CODE END I2C4_MspInit 0 */

          /**I2C4 GPIO Configuration
          PD12     ------> I2C4_SCL
          PD13     ------> I2C4_SDA
          */
        GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C4;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C4_CLK_ENABLE();
        /* USER CODE BEGIN I2C4_MspInit 1 */

        /* USER CODE END I2C4_MspInit 1 */
    }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{

    if (hi2c->Instance == I2C4)
    {
        /* USER CODE BEGIN I2C4_MspDeInit 0 */

        /* USER CODE END I2C4_MspDeInit 0 */
          /* Peripheral clock disable */
        __HAL_RCC_I2C4_CLK_DISABLE();

        /**I2C4 GPIO Configuration
        PD12     ------> I2C4_SCL
        PD13     ------> I2C4_SDA
        */
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12 | GPIO_PIN_13);

    }
    /* USER CODE BEGIN I2C4_MspDeInit 1 */

    /* USER CODE END I2C4_MspDeInit 1 */
}
