#include <motor.h>
#include <io_mapping.h>
#include <processor.h>
#include <main.h>
#include <global.h>
#include <cstdlib>

void motor::init()
{


	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_12, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

	/*Configure GPIO pin : PF12 */
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*Configure GPIO pins : PE10 PE12 PE14 */
	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : PB10 PB12 PB14 */
	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PB15 */
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PD10 PD12 */
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin : PD14 */
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : PG13 PG14 */
	GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);


	htim14.Instance = TIM14;
	htim14.Init.Prescaler = 16000 -1;
	htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim14.Init.Period = 100 - 1;
	htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
	{
	  Error_Handler();
	}
	HAL_TIM_Base_Start_IT(&htim14);
	//this->setSpeed(0);

}

void motor::turnOnMotor(void)
{

	//Set enable pin to 0 for turn on outputs of fets
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, (GPIO_PinState)0);  //EN
	//Set reset pin (0 = outputs off, 1 = outputs on)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (GPIO_PinState)1);  //RST
	//Assert sleepmode logic high is on, logic low is off
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, (GPIO_PinState)1);  //SLP
	//Set the microsteping pins based on stepMode
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, (GPIO_PinState)0);  //MS1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, (GPIO_PinState)0);  //MS2
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, (GPIO_PinState)0);  //MS2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, (GPIO_PinState)0);  //MS3
	//Assert direction
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, (GPIO_PinState)1);  //DIR
	//Assert logic low on STEP pin
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, (GPIO_PinState)0);  //STP

	//Turn on timer and set motor speed to 0

}

void motor::turnOffMotor(void)
{

	//Set enable pin to 1 for turn off outputs of fets
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, (GPIO_PinState)1);  //EN
	//Set reset pin (1 = outputs off, 0 = outputs on)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (GPIO_PinState)1);  //RST
	//Assert sleepmode is off (1)
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, (GPIO_PinState)1);  //SLP
	//Assert direction
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, (GPIO_PinState)1);  //DIR
	//Assert logic low on STEP pin
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, (GPIO_PinState)0);  //STP

}

void motor::turnSteps(int direction, int numSteps)
{
	int currentStep = 0;
	this->setDirection(direction);
	while (currentStep < numSteps)
	{
		this->step();
		currentStep++;
	}
}

void motor::setSpeed(int speedVal)
{


	//Determine if value needs changing
	if(systemState.currentSpeed == speedVal)
	{
		//do nothing
	}
	else
	{
		HAL_TIM_Base_Stop(&htim14);
		if(speedVal < -100){speedVal = -100;}
		if(speedVal > 100){speedVal = 100;}

		if(speedVal == 0){
			//turn off interrupt/stop timer
			HAL_TIM_Base_Stop(&htim14);
			systemState.motorOnOff = 0;
		}
		else
		{
			//First set direction
			if(speedVal < 0)
			{
				this->setDirection(0);
			}
			else
			{
				this->setDirection(1);
			}
			//update interrupt for new time (determined via speedVal)
			//First determine prescaler (assume clock tick = 1ms)
			int prescaler = 1000/(abs(speedVal));
			htim14.Init.Period = prescaler - 1;
			if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
				{
				  Error_Handler();
				}
			HAL_TIM_Base_Start_IT(&htim14);
			systemState.currentSpeed = speedVal;

		}
	}
}

void motor::setDirection(int direction){
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, (GPIO_PinState)direction);  //DIR
}

void motor::step()
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, (GPIO_PinState)1);  //STP
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, (GPIO_PinState)0);  //STP
}

void motor::changeMircoStepping(enum stepMode mircoStepMode)
{
	//First power down board
	this->turnOffMotor();


	//Set the microsteping pins based on microSteppingMode
	int MS1, MS2, MS3 = 1;
	switch(mircoStepMode){
		case fullStep :
			MS1 = MS2 = MS3 = 0;
			break;
		case halfStep:
			MS1 = 1;
			MS2 = MS3 = 0;
			break;
		case quarterStep:
			MS2 = 1;
			MS1 = MS3 = 0;
			break;
		case eightStep:
			MS3 = 1;
			MS1 = MS2 = 1;
			break;
		case sixteenthStep:
			MS3 = 1;
			MS1 = MS2 = MS3= 1;
			break;
	}

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, (GPIO_PinState)MS1);  //MS1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, (GPIO_PinState)MS2);  //MS2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, (GPIO_PinState)MS3);  //MS3



}

void motor::Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
