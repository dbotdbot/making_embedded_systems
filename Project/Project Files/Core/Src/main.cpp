
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
TIM_HandleTypeDef htim1;
GPIO_InitTypeDef GPIO_InitStruct = {0};

int timer_val = 0;

int main(void)
{
  //Initialize the various parts of the system (each is encapsulated in its own c++ class with the exception of globals above)
  Machine::init();
  //led::init();
  //motor::init();
  //encoder::init();

  motor motor1;
  motor1.init();
  motor *motor1ptr;
  motor1ptr = &motor1;

  encoder encoder1;
  encoder1.init();
  encoder *encoder1ptr;
  encoder1ptr = &encoder1;


  systemState.setpoint = 100;
  systemState.currentPos = 0;
  systemState.LEDRed = 0;
  systemState.LEDGreen = 0;
  systemState.LEDBlue = 0;
  systemState.currentSpeed = 0;
  systemState.motorOnOff = 0;
  systemState.mode = 0;
  systemState.zeroRaw = -1;
  systemState.lastTime = 0;
  systemState.loopTime = 100;
  systemState.angleErrorSum = 0;
  systemState.angleLastError = 0;
  systemState.angleErrorDer = 0;
  systemState.kp = 1;
  systemState.ki = 1;
  systemState.kd = 1;
  systemState.output = 0;


  uint32_t currentMode = 0;

  ConsoleInit();
  //motor1.turnOnMotor();


  //motor1.setDirection(1);
  //motor1.setSpeed(10);
  //HAL_Delay(5000);
  //motor1.setSpeed(0);
  //motor1.zeroMotor(&encoder1);
  //motor1.turnOffMotor();


  //timer_val = __HAL_TIM_GET_COUNTER(&htim14);
  while (1)
  {
	  switch(systemState.mode){
	  case 0:
		  	  if(systemState.mode == currentMode){
		  		  //Do nothing already in Off mode
		  	  }
		  	  else{
		  		  motor1.turnOffMotor();
		  		  currentMode = 0;
	  	  	  }
		  	  break;
	  case 1:
		  	  if(systemState.mode != currentMode){
		  		  //Motor needs to be turned on
		  		  motor1.turnOnMotor();
		  		  if(systemState.zeroRaw < 0){
		  			  motor1.zeroMotor(&encoder1);
		  		  }
		  		  currentMode = 1;
		  	  }
		  	  //Code for control loop

		  	  break;

	  case 2:
		  	  //code for speed mode
		  	  currentMode = 2;
		  	  break;

	  default:
		  motor1.turnOffMotor();

	  }
	  //led::SetRGB(systemState.LEDRed, systemState.LEDGreen, systemState.LEDBlue);
	  //HAL_Delay(50);
	  //led::SetRGB(0,65535,0);
	  //HAL_Delay(100);
	  //led::SetRGB(0,0,65535);
	  //HAL_Delay(100);

	  //motor1.setSpeed(50);
	  //HAL_Delay(5000);
	  //motor1.setSpeed(0);
	  //HAL_Delay(50000);
	  ///motor1.setSpeed(80);
	  //HAL_Delay(5000);

	  ConsoleProcess(&encoder1);

	  encoder::getRaw();
	  //ConsoleGet();

  }
}

//motor steping interrupt
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim14){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, (GPIO_PinState)1);  //STP
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, (GPIO_PinState)0);  //STP
	}

}

void controlLoop(encoder *encoder)
{
	uint32_t timeNow = (uint32_t)__HAL_TIM_GET_COUNTER(&htim1);
	uint32_t timeSinceLast = (uint32_t)(timeNow - systemState.lastTime);
	if(timeSinceLast > systemState.loopTime)
	{
		//Calculate Errors
		uint32_t currentAngle = encoder->getAngle();
		uint32_t angleError = systemState.setAngle - currentAngle;
		systemState.angleErrorSum += (angleError * timeSinceLast);
		systemState.angleErrorDer = (angleError - systemState.angleLastError)/timeSinceLast;

		//Compute PID output
		uint32_t prop = systemState.kp * angleError;
		uint32_t integral = systemState.ki * systemState.angleErrorSum;
		uint32_t derivitive = systemState.kd * systemState.angleErrorDer;

		//Limit outputs

		//Final output calc
		systemState.output = prop + integral + derivitive;

		//update old variables
		systemState.angleLastError = angleError;
		systemState.lastTime = timeNow;
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
