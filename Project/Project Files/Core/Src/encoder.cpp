#include <encoder.h>
#include <io_mapping.h>
#include <processor.h>
#include <main.h>
#include <global.h>
#include <stdint.h>

void encoder::init()
{
	//SPI
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
	  Error_Handler();
	}

	//CS pin
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	  /*Configure GPIO pin : PA4 */
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//set CS high
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState)1);

	uint16_t readBit = 0x8000;
	uint16_t readAngleAddress = 0x3fff;
	readCommand = (readBit | readAngleAddress);
	dummyCommand = 0x0;

	valueMask = 0x3fff;
	value = 0;

}

uint32_t encoder::getRaw()
{
	//Setup commands
	uint16_t test = 0xffff;
	uint16_t test2 = 0xAAAA;
	uint16_t readValue = 0;
	uint8_t spi_buf[2];

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState)0);
	sendBuffer = &test;
	//HAL_SPI_Transmit(&hspi1, (uint8_t*)sendBuffer, 1, 1);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)sendBuffer,spi_buf, 1, 100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState)1);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState)0);
	sendBuffer = &test;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)sendBuffer,spi_buf, 1, 100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState)1);


	readValue = spi_buf[0] | (spi_buf[1] << 8);
	value = readValue & valueMask;
	systemState.currentPos = (uint32_t)value;
	return (uint32_t)value;

}

float encoder::getAngle(){
	int32_t adjustedPosSum = 0;
	int32_t adjustedPos = 0;
	uint32_t encoderMax = 16384;
	float encoderScaler = 0.0219;
	float angle = 0;
	for(int i = 0; i<systemState.noSamples; i++)
	{
		this->getRaw();
		if((systemState.currentPos - systemState.zeroRaw) > 0){
			adjustedPosSum += systemState.currentPos - systemState.zeroRaw;
		}
		else if((systemState.currentPos - systemState.zeroRaw) < 0){
			adjustedPosSum += (encoderMax - systemState.zeroRaw) + systemState.currentPos;
		}
	}
	adjustedPos = adjustedPosSum / systemState.noSamples;
	angle = 360 - ((float)adjustedPos * encoderScaler);
	return angle;

}

void encoder::Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
