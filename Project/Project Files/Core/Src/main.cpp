
#include "main.h"
#include "canBus.h"
#include "machine.h"
#include "led.h"
#include "motor.h"
#include <string.h>
#include "consoleCommands.h"
#include "console.h"
#include "consoleIo.h"
#include "encoder.h"
#include "console.h"
#include "global.h"
#include <stdio.h>
#include <cstdlib>
//

//prototype
int32_t controlLoop(encoder *encoder);

struct MachineState systemState;
struct controlLoopParam PID;
CAN_HandleTypeDef hcan1;
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim13; //clock used for PID loop
TIM_HandleTypeDef htim14; //clock used for step function
TIM_HandleTypeDef htim1;
GPIO_InitTypeDef GPIO_InitStruct = {0};
//Can bus stuff
CAN_TxHeaderTypeDef pHeader; //declare a specific header for message transmittions
CAN_RxHeaderTypeDef pRxHeader; //declare header for message reception
CAN_TxHeaderTypeDef   TxHeader;
uint8_t               TxData[8];
uint8_t				 RxData[8];
uint32_t              TxMailbox;

int timer_val = 0;

int main(void)
{
  //Initialize the various parts of the system (each is encapsulated in its own c++ class with the exception of globals above)
  Machine::init();

  led led1;
  led1.init();
  led *led1ptr;
  led1ptr = &led1;
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

  canBus canBus1;
  canBus1.init();
  canBus1.transmitHeaderConfig();
  canBus1.filterConfig();
  canBus1.turnOn();


  systemState.setpoint = 100;
  systemState.setAngle = 45;
  systemState.currentPos = 0;
  systemState.LEDRed = 0;
  systemState.LEDGreen = 0;
  systemState.LEDBlue = 0;
  systemState.currentSpeed = 0;
  systemState.motorOnOff = 0;
  systemState.mode = 0;
  systemState.zeroRaw = -1;
  systemState.currentMode = 0;
  systemState.noSamples = 10;
  systemState.backOffSteps = 10;

  PID.lastTime = 0;
  PID.loopTime = 100;
  PID.angleErrorSum = 0;
  PID.angleLastError = 0;
  PID.angleErrorDer = 0;
  PID.kp = 1;
  PID.ki = 0.001;
  PID.kd = 0;
  PID.minOut = -100;
  PID.maxOut = 100;
  PID.output = 0;

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
		  	  if(systemState.mode == systemState.currentMode){
		  		  //Do nothing already in Off mode
		  	  }
		  	  else{
		  		  motor1.turnOffMotor();
		  		  systemState.currentMode = 0;
	  	  	  }
		  	  break;
	  case 1:
		  	  if(systemState.mode != systemState.currentMode){
		  		  //Motor needs to be turned on
		  		  motor1.turnOnMotor();
		  		  if(systemState.zeroRaw < 0){
		  			  motor1.zeroMotor(&encoder1, &led1);
		  		  }
		  		  systemState.currentMode = 1;
		  	  }
		  	  //Code for control loop
		  	  controlLoop(&encoder1);
		  	  motor1.setSpeed(PID.output);
		  	  if(abs(PID.angleLastError) < 2)
		  	  {
		  		  led1.SetRGB(0,65535,0);
		  	  }
		  	  else
		  	  {
		  		  led1.SetRGB(65535, 0, 0);
		  	  }
		  	  uint8_t testData[2];
		  	  testData[0] = 1;
		  	  testData[1] = 2;
		  	  canBus1.transmit(2, &testData[0]);
		  	  break;

	  case 2:
		  	  //code for speed mode
		  	  systemState.currentMode = 2;
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

void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */

  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */
  if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &pRxHeader, RxData) != HAL_OK)
	  {
	  Error_Handler();
	  }
  if ((pRxHeader.StdId == 0x103))
    {
  	  //Do stuff
	}
  //GPIOD->ODR=r<<12; //use output data register to turn on LEDs

  /* USER CODE END CAN1_RX0_IRQn 1 */
}

int32_t controlLoop(encoder *encoder)
{
	int32_t waiting = 1;
	int32_t timeNow = (int32_t)__HAL_TIM_GET_COUNTER(&htim13);
	int32_t timeSinceLast = (int32_t)(timeNow - PID.lastTime);

		if(timeSinceLast > PID.loopTime)
		{
			//Calculate Errors
			float currentAngle = encoder->getAngle();
			float angleError = systemState.setAngle - currentAngle;
			PID.angleErrorSum += (angleError * (timeSinceLast/1000));
			PID.angleErrorDer = (angleError - PID.angleLastError)/(timeSinceLast/1000);

			//Compute PID output
			float prop = PID.kp * angleError;
			float integral = PID.ki * PID.angleErrorSum;
			float derivitive = PID.kd * PID.angleErrorDer;

			//Limit Outputs if necessary
			if(prop < PID.minOut){prop = PID.minOut;}
			if(prop > PID.maxOut){prop = PID.maxOut;}
			if(integral < PID.minOut){integral = PID.minOut;}
			if(integral > PID.maxOut){integral = PID.maxOut;}
			if(derivitive < PID.minOut){derivitive = PID.minOut;}
			if(derivitive > PID.maxOut){derivitive = PID.maxOut;}


			//Final output calc
			PID.output = (int32_t)(prop + integral + derivitive);

			//update old variables
			PID.angleLastError = angleError;
			PID.lastTime = timeNow;

			ConsoleIoSendString("Set Angle = ");
			ConsoleSendParamInt16((uint16_t)systemState.setAngle);
			ConsoleIoSendString(" Current Angle = ");
			ConsoleSendParamInt16((uint16_t)currentAngle);
			ConsoleIoSendString(" Encoder Raw = ");
			ConsoleSendParamInt16((uint16_t)systemState.currentPos);
			ConsoleIoSendString(" Output = ");
			ConsoleSendParamInt16((uint16_t)PID.output);
			ConsoleIoSendString(" Error = ");
			if(angleError > 0){ConsoleSendParamInt16((uint16_t)angleError);}
			else if (angleError < 0){
				ConsoleIoSendString("-");
				ConsoleSendParamInt16((uint16_t)(angleError * -1));
			}
			ConsoleIoSendString(STR_ENDLINE);

			waiting = 0;
			return 1;
		}
		return 0;
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
