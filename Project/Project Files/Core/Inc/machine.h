
#ifndef MACHINE_H
#define MACHINE_H

class Machine final {
public:
	static void init();
private:
	static void Config_Clock();
	static void Error_Handler();
};

#endif
