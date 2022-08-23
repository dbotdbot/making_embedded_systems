#ifndef CANBUS_H
#define CANBUS_H

#include <global.h>

extern CAN_TxHeaderTypeDef pHeader; //declare a specific header for message transmittions
extern CAN_RxHeaderTypeDef pRxHeader; //declare header for message reception

extern CAN_TxHeaderTypeDef   TxHeader;

extern uint8_t               TxData[8];
extern uint8_t				  RxData[8];

extern uint32_t              TxMailbox;

class canBus final {
public:
	void init();
	void filterConfig();
	void transmitHeaderConfig();
	void transmit(uint32_t, uint8_t*);
	void turnOn();
private:


	static void Error_Handler();
};



#endif
