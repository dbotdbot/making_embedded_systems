#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>


static uint16_t readCommand;
static uint8_t dummyCommand;
static uint16_t* sendBuffer;
static uint8_t recieve;
static uint16_t* recieveBuffer;
static uint16_t value;
static uint16_t valueMask;

class encoder{

public:


	static void init();
	static uint32_t getRaw();
	float getAngle();


private:
	static void Error_Handler();

};



#endif
