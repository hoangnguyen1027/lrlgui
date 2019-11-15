#pragma once
#include "b1_hardware.h"

class solenoid : public b1_hardware {

public:
	
	// ENUMS
	enum solenoidType {
		NO = 0,
		NC = 1
	};
	enum solenoidState {
		OPEN = 0,
		CLOSED = 1
	};
	enum solenoidLine {
		LOX = 0,
		FUE = 1
	};

	// CONSTRUCTORS
	solenoid(int, solenoidType, solenoidState, solenoidLine);

	// METHODS
	bool isOpen(void);
	void open(void);
	void close(void);
	void toggle(void);


	solenoidType getSolenoidType(void);
	solenoidState getCurrentState(void);
	solenoidLine getSolenoidLine(void);
	void setCurrentState(solenoidState);

private:

	// VARIABLES
	solenoidType type;
	solenoidState state;
	solenoidLine line;

};

