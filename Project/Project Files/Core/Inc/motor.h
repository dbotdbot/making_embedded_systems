#ifndef MOTOR_H
#define MOTOR_H

#include <global.h>

class motor final {
public:
	void init();
	void turnOnMotor();
	void turnOffMotor();
	void turnSteps(int direction, int numSteps);
	void setSpeed(int speedVal);
	void changeMircoStepping(enum stepMode);
	void step();
private:
	void setDirection(int direction);

	static void Error_Handler();
};



#endif
