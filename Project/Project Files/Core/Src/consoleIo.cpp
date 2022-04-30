// Console IO is a wrapper between the actual in and output and the console code
// In an embedded system, this might interface to a UART driver.
#include <io_mapping.h>
#include <processor.h>
#include <main.h>
#include "string.h"
#include "consoleIo.h"
#include <stdio.h>
#include <global.h>

UART_HandleTypeDef huart1;

eConsoleError ConsoleIoInit(void)
{
	    huart1.Instance = USART1;
		huart1.Init.BaudRate = 115200;
		huart1.Init.WordLength = UART_WORDLENGTH_8B;
		huart1.Init.StopBits = UART_STOPBITS_1;
		huart1.Init.Parity = UART_PARITY_NONE;
		huart1.Init.Mode = UART_MODE_TX_RX;
		huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart1.Init.OverSampling = UART_OVERSAMPLING_16;
		if (HAL_UART_Init(&huart1) != HAL_OK)
		{
		  Error_Handler();
		}


	return CONSOLE_SUCCESS;
}
eConsoleError ConsoleIoReceive(uint8_t *buffer, const uint32_t bufferLength, uint32_t *readLength)
{


	HAL_UART_Receive(&huart1, (uint8_t *)buffer, bufferLength, 100);
	*readLength = strlen((const char*)buffer);

	return CONSOLE_SUCCESS;
}

eConsoleError ConsoleIoSendString(const char *buffer)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer) , 10);
	return CONSOLE_SUCCESS;
}

eConsoleError ConsleGetVal()
{
	char in[8] = {0};
	HAL_UART_Receive(&huart1, (uint8_t *)in, 8, 1);
	HAL_UART_Transmit(&huart1, (uint8_t *)in, strlen(in) , 10);
	return CONSOLE_SUCCESS;
}

