#pragma once
#include "pinout.h"

class b1_hardware {

public:

	// CONSTRUCTOR
	b1_hardware();

	// METHODS
	int getHardwarePin(void);
	void setHardwarePin(int);

private:

	// VARIABLES
	int gpio_pin;

};

