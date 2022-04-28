
#include "main.h"
#include "machine.h"
#include "led.h"
#include "motor.h"
//#include "communication.h"
#include "encoder.h"
#include "console.h"



int main(void)
{
  //Initialize the various parts of the system (each is encapulated in its own c++ class with the excption of globals above)
  Machine::init();
  led::init();
  motor::init();
  //communication::init();
  encoder::init();
  ConsoleInit();


  while (1)
  {
	  //led::SetRGB(65535,0,0);
	  //HAL_Delay(50);
	  //led::SetRGB(0,65535,0);
	  //HAL_Delay(100);
	  //led::SetRGB(0,0,65535);
	  //HAL_Delay(100);

	  //ConsoleProcess();
	  ConsoleGet();

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
