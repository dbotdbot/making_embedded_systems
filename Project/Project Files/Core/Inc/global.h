#ifndef GLOBAL_H
#define GLOBAL_H

#include "main.h"

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim1;
extern GPIO_InitTypeDef GPIO_InitStruct;

enum stepMode {fullStep = 0, halfStep = 2, quarterStep = 4, eightStep = 8, sixteenthStep = 16};


struct MachineState {
	uint32_t setpoint;
	uint32_t setAngle;
	uint32_t currentPos;
	uint32_t LEDRed;
	uint32_t LEDGreen;
	uint32_t LEDBlue;
	uint32_t turnDirection;
	uint32_t Enable;
	uint32_t resetDevice;
	uint32_t lowPowerState;
	int32_t currentSpeed;
	uint32_t motorOnOff;
	uint32_t lastTime; 		//Tick count of last PID loop through
	uint32_t loopTime; 		//min number of clock ticks between PID loop
	float zeroRaw;			//Raw encoder output when at zero position
	uint32_t angleErrorSum;
	uint32_t angleLastError;
	uint32_t angleErrorDer;
	enum stepMode stepMode;
	uint32_t mode;
	uint32_t kp;
	uint32_t ki;
	uint32_t kd;
	uint32_t output;

};

extern struct MachineState systemState;



#endif /*GLOBAL_H*/
