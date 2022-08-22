#ifndef MOTOR_H
#define MOTOR_H

#include <global.h>
#include <encoder.h>
#include <led.h>

class motor final {
public:
	void init();
	void turnOnMotor();
	void turnOffMotor();
	void turnSteps(int direction, int numSteps);
	void setSpeed(int speedVal);
	void changeMircoStepping(enum stepMode);
	void step();
	void zeroMotor(encoder*, led*);
	void setDirection(int direction);
	void loopPID(encoder*);
private:


	static void Error_Handler();
};



#endif
