#ifndef GLOBAL_H
#define GLOBAL_H


struct MachineState {
	uint32_t setpoint;
	uint32_t currentPos;
	uint32_t LEDRed;
	uint32_t LEDGreen;
	uint32_t LEDBlue;
};

extern struct MachineState systemState;


#endif /*GLOBAL_H*/
