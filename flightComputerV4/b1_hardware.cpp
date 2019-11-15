#include "b1_hardware.h"


// CONSTRUCTOR
b1_hardware::b1_hardware()
{
}

// GETTERS
int b1_hardware::getHardwarePin(void) {
	return gpio_pin;
}

// SETTERS
void b1_hardware::setHardwarePin(int pin) {
	gpio_pin = pin;
}