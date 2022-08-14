
#ifndef MACHINE_H
#define MACHINE_H

class Machine final {
public:
	static void init();
private:
	static void Config_Clock();
	static void MX_TIM1_Init();
	static void Error_Handler();
	static void MX_TIM13_Init();
};

#endif
