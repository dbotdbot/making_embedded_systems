#ifndef LED_H
#define LED_H

class led final {
public:
	static void init();
	static void SetRGB(int red, int green, int blue);
private:

	static void Error_Handler();
};



#endif
