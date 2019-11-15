#pragma once
#include "b1_hardware.h"
#include <thread>
#include <chrono>

// If the design changes from main Valve to Servo Actuator Valve
// Just change Delay time at line 39 to properly open specific valve.
class mainvalve : public b1_hardware {

public:

	// ENUMS
	enum mainType {
		NO = 0,
		NC = 1
	};
	enum mainState {
		OPEN = 0,
		CLOSED = 1
	};
	enum mainLine {
		LOX = 0,
		FUE = 1
	};

	// CONSTRUCTORS
	mainvalve(int, mainType, mainState, mainLine);

	// METHODS
	void initialize(void);
	void close(void);
	void open(void);
	static void cutPower(int, int);

	mainType getmainType(void);
	mainState getCurrentState(void);
	mainLine getMainLine(void);

private:

	// VARIABLES
	mainType type;
	mainState state;
	mainLine line;

	int mainCutOffDelayMS = 5000;

};