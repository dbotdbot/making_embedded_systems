#include <communication.h>
#include <io_mapping.h>
#include <processor.h>
#include <main.h>
#include "string.h"

//UART_HandleTypeDef huart4;

void communication::init()
{
	//CAN_HandleTypeDef hcan1;
	//UART_HandleTypeDef huart4;

	//CAN
	/*hcan1.Instance = CAN1;
	hcan1.Init.Prescaler = 16;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.AutoBusOff = DISABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.AutoRetransmission = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;
	if (HAL_CAN_Init(&hcan1) != HAL_OK)
	{
	  Error_Handler();
	}
    */

	//UART


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
}

void communication::testOut(void){
	//String output = "Hello";
	HAL_UART_Transmit(&huart4, (uint8_t *)"hello\n", strlen("hello\n"), 10);
}

void communication::Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
