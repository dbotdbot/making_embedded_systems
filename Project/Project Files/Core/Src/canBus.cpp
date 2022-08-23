#include <canBus.h>
#include <io_mapping.h>
#include <processor.h>
#include <main.h>
#include <global.h>
#include <cstdlib>


void canBus::init(){

	  hcan1.Instance = CAN1;
	  hcan1.Init.Prescaler = 16;
	  hcan1.Init.Mode = CAN_MODE_NORMAL;
	  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	  hcan1.Init.TimeSeg1 = CAN_BS1_4TQ;
	  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
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
	  /* USER CODE BEGIN CAN1_Init 2 */

	  /* USER CODE END CAN1_Init 2 */


}

void canBus::turnOn(){
	HAL_CAN_Start(&hcan1);
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	  {
		  Error_Handler();
	  }
}

void canBus::transmitHeaderConfig(void)
{
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.StdId = 0x446;
	TxHeader.RTR = CAN_RTR_DATA;
}

void canBus::transmit(uint32_t numDataBytes, uint8_t *data)
{
	TxHeader.DLC = numDataBytes;
	for(uint32_t i = 0; i<numDataBytes; i++)
	{
		TxData[i] = data[i];
	}
	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	{
	   Error_Handler ();
	}
}

void canBus::filterConfig(void){
	CAN_FilterTypeDef canfilterconfig;
	canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank = 10;  // which filter bank to use from the assigned ones
	canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	canfilterconfig.FilterIdHigh = 0x446<<5;
	canfilterconfig.FilterIdLow = 0;
	canfilterconfig.FilterMaskIdHigh = 0x446<<5;
	canfilterconfig.FilterMaskIdLow = 0x0000;
	canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
	canfilterconfig.SlaveStartFilterBank = 20;  // how many filters to assign to the CAN1
	HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig);
}

void canBus::Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
