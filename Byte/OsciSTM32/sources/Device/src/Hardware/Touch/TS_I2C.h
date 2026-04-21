// 2022/5/7 13:01:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "stm32f7xx_hal.h"

namespace I2C
{
    void Init();
    void Start();
    void Stop();
    uint8_t Wait_Ack();
    void Send_Byte(uint8_t txd);
    uint8_t Read_Byte(uint8_t ack);
    uint8_t Send(uint8_t I2c_Addr, uint8_t *buf, uint8_t len);
    void ReadReg(uint8_t I2c_Addr, uint16_t reg, uint8_t *buf, uint8_t len);
    uint8_t WriteReg(uint8_t I2c_Addr, uint16_t reg, uint8_t *buf, uint8_t len);
    void NAck();
}
