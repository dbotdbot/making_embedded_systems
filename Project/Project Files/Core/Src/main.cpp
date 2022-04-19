
#include "main.h"
#include "machine.h"
#include "led.h"


int main(void)
{

  Machine::init();
  led::init();

  while (1)
  {
	  led::SetRGB(65535,0,0);
	  HAL_Delay(300);
	  led::SetRGB(0,65535,0);
	  HAL_Delay(300);
	  led::SetRGB(0,0,65535);
	  HAL_Delay(300);
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
