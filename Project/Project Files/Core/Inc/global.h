#ifndef GLOBAL_H
#define GLOBAL_H

#include "main.h"

extern SPI_HandleTypeDef hspi1;
extern GPIO_InitTypeDef GPIO_InitStruct;

enum stepMode {fullStep = 0, halfStep = 2, quarterStep = 4, eightStep = 8, sixteenthStep = 16};

struct MachineState {
	uint32_t setpoint;
	uint32_t currentPos;
	uint32_t LEDRed;
	uint32_t LEDGreen;
	uint32_t LEDBlue;
	uint32_t turnDirection;
	uint32_t Enable;
	uint32_t resetDevice;
	uint32_t lowPowerState;
	enum stepMode stepMode;

};

extern struct MachineState systemState;



#endif /*GLOBAL_H*/
