#include "defines.h"
#include "Hardware/Touch/Touch.h"
#include "TS_I2C.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"
#include <stdio.h>


namespace Touch
{
	static uint8 flag = 0;
}


uint8_t Touch::Init(void)
{
	GPIO_InitTypeDef is =
	{
		GPIO_PIN_7,
		GPIO_MODE_IT_RISING,
		GPIO_NOPULL
	};

	HAL_GPIO_Init(GPIOD, &is);
	
	I2C::Init();

	/* reset GT811 */
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(200);

	/* if Version is correct, send the configuration parameters */
	if (Touch::ReadID() == GT811_VERSION_VALUE)
	{
		/* touch screen configuration parameter (touch screen manufacturers provide) */
		uint8_t GTP_CFG_DATA[] =
		{
			0x12,0x10,0x0E,0x0C,0x0A,0x08,0x06,0x04,0x02,0x00,0x05,0x55,0x15,0x55,0x25,0x55,0x35,0x55,0x45,0x55,
			0x55,0x55,0x65,0x55,0x75,0x55,0x85,0x55,0x95,0x55,0xA5,0x55,0xB5,0x55,0xC5,0x55,0xD5,0x55,0xE5,0x55,
			0xF5,0x55,0x1B,0x03,0x00,0x00,0x00,0x13,0x13,0x13,0x0F,0x0F,0x0A,0x50,0x30,0x05,0x03,0x64,0x05,0xe0,
			0x01,0x20,0x03,0x00,0x00,0x32,0x2C,0x34,0x2E,0x00,0x00,0x04,0x14,0x22,0x04,0x00,0x00,0x00,0x00,0x00,
			0x20,0x14,0xEC,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x30,0x25,0x28,0x14,0x00,
			0x00,0x00,0x00,0x00,0x00,0x01,
		};

		/* config  */
		GTP_CFG_DATA[62] = Display::WIDTH >> 8;
		GTP_CFG_DATA[61] = Display::WIDTH & 0xff;
		GTP_CFG_DATA[60] = Display::HEIGHT >> 8;
		GTP_CFG_DATA[59] = Display::HEIGHT & 0xff;

		I2C::WriteReg(GT811_CMD_WR, GT811_CONFIG_REG, (uint8_t *)GTP_CFG_DATA, sizeof(GTP_CFG_DATA));
	}
	return HAL_OK;
}

/**
  * @brief  Read the GT811 device ID, pre initialize I2C in case of need to be
  *         able to read the GT811 device ID, and verify this is a GT811.
  * @param  DeviceAddr: I2C GT811 Slave address.
  * @retval The Device ID (two bytes).
  */
uint16_t Touch::ReadID()
{
	uint8_t value[2];
	I2C::ReadReg(GT811_CMD_WR, GT811_VERSION, value, 2);
	printf("ID    %x\r\n", (((uint16_t)value[0] << 8) + value[1]));
	return value[0] * 0x100 + value[1];
}


bool Touch::GetState(TS_StateTypeDef *TS_State)
{
	if (flag)
	{
		flag = 0;

		uint8_t RegBuf[34];

		/* Read touch message */
		I2C::ReadReg(GT811_CMD_WR, GT811_READ_XY_REG, RegBuf, sizeof(RegBuf));

		/* get touch massage */
		TS_State->SensorId = RegBuf[0x00] >> 6;
		TS_State->touchDetected = RegBuf[0x00] & 0x1F;

		if (TS_State->touchDetected != 0)
		{
			//Touch point 1 coordinates
			TS_State->touchY[0] = Display::HEIGHT - (((uint16_t)RegBuf[0x02] << 8) + RegBuf[0x03]);
			TS_State->touchX[0] = (((uint16_t)RegBuf[0x04] << 8) + RegBuf[0x05]);
			TS_State->touchWeight[0] = RegBuf[0x06];

			//Touch point 2 coordinates
			TS_State->touchY[1] = Display::HEIGHT - (((uint16_t)RegBuf[0x07] << 8) + RegBuf[0x08]);
			TS_State->touchX[1] = (((uint16_t)RegBuf[0x09] << 8) + RegBuf[0x0A]);
			TS_State->touchWeight[1] = RegBuf[0x0B];

			//Touch point 3 coordinates
			TS_State->touchY[2] = Display::HEIGHT - (((uint16_t)RegBuf[0x0C] << 8) + RegBuf[0x0D]);
			TS_State->touchX[2] = (((uint16_t)RegBuf[0x0E] << 8) + RegBuf[0x0F]);
			TS_State->touchWeight[2] = RegBuf[0x10];

			//Touch point 4 coordinates
			TS_State->touchY[3] = Display::HEIGHT - (((uint16_t)RegBuf[0x11] << 8) + RegBuf[0x18]);
			TS_State->touchX[3] = (((uint16_t)RegBuf[0x19] << 8) + RegBuf[0x1A]);
			TS_State->touchWeight[3] = RegBuf[0x1B];

			//Touch point 5 coordinates
			TS_State->touchY[4] = Display::HEIGHT - (((uint16_t)RegBuf[0x1C] << 8) + RegBuf[0x1D]);
			TS_State->touchX[4] = (((uint16_t)RegBuf[0x1E] << 8) + RegBuf[0x1F]);
			TS_State->touchWeight[4] = RegBuf[0x20];

			return true;
		}
	}

	return false;
}


void Touch::CallbackOnInterrupt(uint16 pin)
{
	if (pin == GPIO_PIN_7)
	{
		flag = 1;
	}
}
