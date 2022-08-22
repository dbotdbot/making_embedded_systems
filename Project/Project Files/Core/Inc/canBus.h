#ifndef CANBUS_H
#define CANBUS_H

#include <global.h>

class canBus final {
public:
	void init();
	void filterConfig();
private:


	static void Error_Handler();
};



#endif
