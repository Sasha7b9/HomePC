// 2022/5/7 13:01:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "TS_I2C.h"


#define SDA_GPIO_PORT   GPIOD
#define SDA_GPIO_PIN    GPIO_PIN_13

#define SCL_CLK()		__GPIOD_CLK_ENABLE();
#define SCL_GPIO_PORT   GPIOD
#define SCL_GPIO_PIN    GPIO_PIN_12

#define SET_SDA()		HAL_GPIO_WritePin(SDA_GPIO_PORT,SDA_GPIO_PIN,GPIO_PIN_SET)    
#define CLR_SDA()		HAL_GPIO_WritePin(SDA_GPIO_PORT,SDA_GPIO_PIN,GPIO_PIN_RESET)  

#define SET_CLK()		HAL_GPIO_WritePin(SCL_GPIO_PORT,SCL_GPIO_PIN,GPIO_PIN_SET)    
#define CLR_CLK()		HAL_GPIO_WritePin(SCL_GPIO_PORT,SCL_GPIO_PIN,GPIO_PIN_RESET)

#define READ_SDA()		HAL_GPIO_ReadPin(SDA_GPIO_PORT,SDA_GPIO_PIN)

#define I2C_MEMADD_16BIT



/**
  * @brief  I2C delay.
  * @param  None
  * @retval None
  */
static void delay_us(uint32_t value)
{
	uint32_t i;
	i = value * 250;
	while (i--);
}

/**
  * @brief  I2C Init.
  * @param  None
  * @retval None
  */
void I2C::Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__GPIOD_CLK_ENABLE();
	SCL_CLK()

		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

	GPIO_InitStruct.Pin = SDA_GPIO_PIN;
	HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SDA_GPIO_PORT, SDA_GPIO_PIN, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = SCL_GPIO_PIN;
	HAL_GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SCL_GPIO_PORT, SCL_GPIO_PIN, GPIO_PIN_SET);

}

/**
  * @brief  Set SDA In.
  * @param  None
  * @retval None
  */
static void SDA_IN()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = SDA_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);

}

/**
  * @brief  Set SDA Out.
  * @param  None
  * @retval None
  */
static void SDA_OUT()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = SDA_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  I2C Start.
  * @param  None
  * @retval None
  */
void I2C::Start(void)
{
	SDA_OUT();

	SET_SDA();
	delay_us(1);

	SET_CLK();
	delay_us(1);

	CLR_SDA();
	delay_us(1);

	CLR_CLK();
	delay_us(1);
}

/**
  * @brief  I2C Stop.
  * @param  None
  * @retval None
  */
void I2C::Stop(void)
{
	SDA_OUT();

	CLR_SDA();
	delay_us(1);

	SET_CLK();
	delay_us(1);

	SET_SDA();
	delay_us(1);
}

/**
  * @brief  I2C Ack.
  * @param  None
  * @retval None
  */
uint8_t I2C::Wait_Ack(void)
{
	uint16_t ucErrTime = 0;

	SDA_IN();
	SET_SDA();
	delay_us(1);
	SET_CLK();
	delay_us(1);

	while (READ_SDA())
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			Stop();
			return 1;
		}
	}
	CLR_CLK();
	return 0;
}

/**
  * @brief  I2C Ack.
  * @param  None
  * @retval None
  */
static void I2C_Ack(void)
{
	SDA_OUT();
	CLR_CLK();

	CLR_SDA();
	delay_us(1);

	SET_CLK();
	delay_us(1);

	CLR_CLK();
	delay_us(1);
}

/**
  * @brief  No I2C Ack.
  * @param  None
  * @retval None
  */
void I2C::NAck(void)
{
	SDA_OUT();

	SET_SDA();
	delay_us(1);

	SET_CLK();
	delay_us(1);

	CLR_CLK();
	delay_us(1);
}

/**
  * @brief  I2C Send one byte
	* @param  1:ack 0:without ack
  * @retval None
  */
void I2C::Send_Byte(uint8_t txd)
{
	uint8_t t;

	SDA_OUT();
	CLR_CLK();       //Put low CLK to send data

	for (t = 0; t < 8; t++)
	{
		if (txd & 0x80)
			SET_SDA();
		else
			CLR_SDA();

		txd <<= 1;
		delay_us(1);

		SET_CLK();
		delay_us(1);
		CLR_CLK();
		delay_us(1);
	}

	SET_SDA();
	delay_us(1);
	CLR_CLK();
	delay_us(1);
}

/**
  * @brief  I2C read one byte
	* @param  1:ack 0:without ack
  * @retval read value
  */
uint8_t I2C::Read_Byte(uint8_t ack)
{
	unsigned char i, receive = 0;
	SDA_IN();         //SDA set in
	for (i = 0; i < 8; i++)
	{
		CLR_CLK();
		delay_us(1);
		SET_CLK();
		receive <<= 1;
		if (READ_SDA())receive++;
		delay_us(1);
	}
	CLR_CLK();
	if (!ack)NAck();   //sent nACK
	else I2C_Ack();        //sent ACK   

	return receive;
}

/*******************************************************************************
* Function Name  : I2C_Send
* Description    : I2C Send bytes to the device.
* Input          : - buf: Need to send the BUF pointer.
*                  - len: The length of the array
* Output         : None
* Return         : ret
				   -0  succeed
									 -1  error
* Attention		 : None
*******************************************************************************/
uint8_t I2C::Send(uint8_t I2c_Addr, uint8_t *buf, uint8_t len)
{
	uint8_t i;
	uint8_t ret = 0;
	Start();

	Send_Byte(I2c_Addr);   //Slaver Addr
	Wait_Ack();

	for (i = 0; i < len; i++)
	{
		Send_Byte(buf[i]);
		ret = Wait_Ack();
		if (ret)break;
	}
	Stop();
	return ret;
}

/*******************************************************************************
* Function Name  : I2C_WR_Reg
* Description    : Writes to the selected register.
* Input          : - reg: address of the selected register.
*                  - buf: Need to write the BUF pointer.
*                  - len: The length of the array
* Output         : None
* Return         : ret
				   -0  succeed
									 -1  error
* Attention		 : None
*******************************************************************************/
uint8_t I2C::WriteReg(uint8_t I2c_Addr, uint16_t reg, uint8_t *buf, uint8_t len)
{
	uint8_t i;
	uint8_t ret = 0;
	Start();

	Send_Byte(I2c_Addr);   //Slaver Addr
	Wait_Ack();

#ifdef I2C_MEMADD_16BIT
	Send_Byte(reg >> 8);   	     //Data Addr high
	Wait_Ack();
#endif

	Send_Byte(reg & 0xFF);   	   //Data Addr low
	Wait_Ack();

	for (i = 0; i < len; i++)
	{
		Send_Byte(buf[i]);
		ret = Wait_Ack();
		if (ret)break;
	}
	Stop();
	return ret;
}

/*******************************************************************************
* Function Name  : i2C_RD_Reg
* Description    : Writes to the selected register.
* Input          : - reg: address of the selected register.
*                  - buf: Need to read the BUF pointer.
*                  - len: The length of the array
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void I2C::ReadReg(uint8_t I2c_Addr, uint16_t reg, uint8_t *buf, uint8_t len)
{
	uint8_t i;
	Start();
	Send_Byte(I2c_Addr);
	Wait_Ack();

#ifdef I2C_MEMADD_16BIT
	Send_Byte(reg >> 8);   	     //Data Addr high
	Wait_Ack();
#endif

	Send_Byte(reg & 0xFF);   	   //Data Addr low
	Wait_Ack();
	Stop();

	Start();
	Send_Byte(I2c_Addr + 1);
	Wait_Ack();

	for (i = 0; i < len; i++)
	{
		buf[i] = Read_Byte(i == (len - 1) ? 0 : 1);
	}
	Stop();
}
