// 2022/5/8 19:25:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f7xx_hal.h>


SDRAM_HandleTypeDef hsdram1;


void HAL_FMC::Init(void)
{
    FMC_SDRAM_TimingTypeDef SdramTiming;

    hsdram1.Instance = FMC_SDRAM_DEVICE;
    hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
    hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
    hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
    hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
    hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
    hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
    hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
    hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
    hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;

    SdramTiming.LoadToActiveDelay = 1;
    SdramTiming.ExitSelfRefreshDelay = 1;
    SdramTiming.SelfRefreshTime = 1;
    SdramTiming.RowCycleDelay = 5;
    SdramTiming.WriteRecoveryTime = 1;
    SdramTiming.RPDelay = 2;
    SdramTiming.RCDDelay = 2;

    HAL_SDRAM_Init(&hsdram1, &SdramTiming);
}


static int FMC_Initialized = 0;


static void HAL_FMC_MspInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (FMC_Initialized)
    {
        return;
    }

    FMC_Initialized = 1;

    __FMC_CLK_ENABLE();

    /** FMC GPIO Configuration
    PF0   ------> FMC_A0
    PF1   ------> FMC_A1
    PF2   ------> FMC_A2
    PF3   ------> FMC_A3
    PF4   ------> FMC_A4
    PF5   ------> FMC_A5
    PH5   ------> FMC_SDNWE
    PF11   ------> FMC_SDNRAS
    PF12   ------> FMC_A6
    PF13   ------> FMC_A7
    PF14   ------> FMC_A8
    PF15   ------> FMC_A9
    PG0   ------> FMC_A10
    PG1   ------> FMC_A11
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PE11   ------> FMC_D8
    PE12   ------> FMC_D9
    PE13   ------> FMC_D10
    PE14   ------> FMC_D11
    PE15   ------> FMC_D12
    PH6   ------> FMC_SDNE1
    PH7   ------> FMC_SDCKE1
    PD8   ------> FMC_D13
    PD9   ------> FMC_D14
    PD10   ------> FMC_D15
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PG4   ------> FMC_BA0
    PG5   ------> FMC_BA1
    PG8   ------> FMC_SDCLK
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PG15   ------> FMC_SDNCAS
    PE0   ------> FMC_NBL0
    PE1   ------> FMC_NBL1
    */
    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
        | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12
        | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5
        | GPIO_PIN_8 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
        | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
        | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14
        | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}


void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    HAL_FMC_MspInit();
}

