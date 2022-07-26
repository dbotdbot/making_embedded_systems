
#include "main.h"
#include "machine.h"
#include "led.h"
#include "motor.h"
//#include "communication.h"
#include "encoder.h"
#include "console.h"
#include "global.h"

struct MachineState systemState;
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim14;
GPIO_InitTypeDef GPIO_InitStruct = {0};

int main(void)
{
  //Initialize the various parts of the system (each is encapsulated in its own c++ class with the exception of globals above)
  Machine::init();
  led::init();
  //motor::init();
  encoder::init();

  motor motor1();

  systemState.setpoint = 100;
  systemState.currentPos = 0;
  systemState.LEDRed = 0;
  systemState.LEDGreen = 0;
  systemState.LEDBlue = 0;
  systemState.currentSpeed = 0;
  systemState.motorOnOff = 0;


  ConsoleInit();
  //motor::turnOnMotor();

  while (1)
  {
	  led::SetRGB(systemState.LEDRed, systemState.LEDGreen, systemState.LEDBlue);
	  //HAL_Delay(50);
	  //led::SetRGB(0,65535,0);
	  //HAL_Delay(100);
	  //led::SetRGB(0,0,65535);
	  //HAL_Delay(100);

	  ConsoleProcess();

	  encoder::getRaw();
	  //ConsoleGet();

  }
}

//motor steping interrupt
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim14){
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);
	}

}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
