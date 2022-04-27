// Console IO is a wrapper between the actual in and output and the console code
// In an embedded system, this might interface to a UART driver.
#include <io_mapping.h>
#include <processor.h>
#include <main.h>
#include "string.h"
#include "consoleIo.h"
#include <stdio.h>

UART_HandleTypeDef huart4;

eConsoleError ConsoleIoInit(void)
{
	    huart4.Instance = UART4;
		huart4.Init.BaudRate = 115200;
		huart4.Init.WordLength = UART_WORDLENGTH_8B;
		huart4.Init.StopBits = UART_STOPBITS_1;
		huart4.Init.Parity = UART_PARITY_NONE;
		huart4.Init.Mode = UART_MODE_TX_RX;
		huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart4.Init.OverSampling = UART_OVERSAMPLING_16;
		if (HAL_UART_Init(&huart4) != HAL_OK)
		{
		  Error_Handler();
		}


	return CONSOLE_SUCCESS;
}
eConsoleError ConsoleIoReceive(uint8_t *buffer, const uint32_t bufferLength, uint32_t *readLength)
{
	uint32_t i = 0;
	char ch;
	
	ch = getch_noblock();
	while ( ( EOF != ch ) && ( i < bufferLength ) )
	{
		buffer[i] = (uint8_t) ch;
		i++;
		ch = getch_noblock();
	}
	*readLength = i;
	return CONSOLE_SUCCESS;
}

eConsoleError ConsoleIoSendString(const char *buffer)
{
	HAL_UART_Transmit(&huart4, (uint8_t *)buffer, strlen(buffer) , 10);
	return CONSOLE_SUCCESS;
}

