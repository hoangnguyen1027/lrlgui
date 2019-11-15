#pragma once
#include "b1_hardware.h"

class sbat : public b1_hardware {

public:

	// ENUMS
	enum sbatState {
		OPEN = 0,
		CLOSED = 1
	};

	// CONSTRUCTORS
	sbat(int, sbatState);

	// METHODS
	void batOn(void);
	void batOff(void);
	bool isOpen(void);

	//solenoidType getSolenoidType(void);
	sbatState getCurrentState(void);
	//sbatLine getBatteryLine(void);
	void setCurrentState(sbatState);

private:

	// VARIABLES
	sbatState state;

};
