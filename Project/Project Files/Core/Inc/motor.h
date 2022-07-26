#ifndef MOTOR_H
#define MOTOR_H

#include <global.h>

class motor final {
public:
	static void init();
	void turnOnMotor();
	void turnOffMotor();
	void turnSteps(int direction, int numSteps);
	void setSpeed(int speedVal);
	void changeMircoStepping(enum stepMode);
private:
	void setDirection(int direction);
	void step();
	static void Error_Handler();
};



#endif
