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

void canBus::filterConfig(void){

}

void canBus::Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
