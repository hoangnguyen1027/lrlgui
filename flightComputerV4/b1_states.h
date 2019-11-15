#pragma once
#include <wiringPi.h>
#include <iostream>
#include <string>
//#include <unistd.h>
#include <cstdio>
#include <thread>
#include <chrono>

#include "solenoid.h"
#include "mainvalve.h"
#include "sbat.h"
#include "logger.h"

class state_machine;

class b1_states {

public:

	// SINGLETON
	static b1_states& getInstance() {
		static b1_states instance;
		return instance;
	}
	b1_states(b1_states const&) = delete;
	void operator=(b1_states const&) = delete;

	// ENUMS
	enum b1_state {

		ST_ANY = -1,
		ST_INIT = 0,
		ST_IDLE = 1,
		ST_FILL = 2,
		ST_PRESSURIZE = 3,
		ST_PRESSURIZED = 4,
		ST_CONBAT = 5,
		ST_LAUNCH = 6,
		ST_EMERG = 800,
		ST_TERM = 9999

	};

	enum b1_event {

		EV_ANY = -1,
		EV_INIT = 0,
		EV_START = 1,
		EV_FILL = 2,
		EV_PRESSURIZE = 3,
		EV_PRESSURIZED = 4,
		EV_CONBAT = 5,
		EV_LAUNCH = 6,
		EV_BURNOUT = 7,
		EV_OVR_PR_LOX = 701,
		EV_OVR_PR_FUE = 702,
		EV_DATA = 777,
		EV_MANUAL = 888,
		EV_EMERG = 911,
		EV_CANCEL = 999
	};

	// STRUCTS
	typedef struct {
		b1_state st;
		b1_event ev;
		b1_state new_st;
		b1_state(*fn)(b1_state);
	} tTransition;

	b1_states::tTransition trans[15] = {

		{ ST_INIT,			EV_INIT,		ST_INIT,			&fn_init_init				 },	// START
		{ ST_INIT,			EV_START,		ST_IDLE,			&fn_init_idle				 }, // INITIALIZE
		{ ST_IDLE,			EV_FILL,		ST_FILL,			&fn_idle_fill				 }, // FILL
		{ ST_FILL,			EV_PRESSURIZE,	ST_PRESSURIZE,		&fn_fill_pressurize			 }, // PRESSURIZE
		{ ST_PRESSURIZE,	EV_PRESSURIZED, ST_PRESSURIZED,		&fn_pressurize_pressurized	 }, // READY
		{ ST_PRESSURIZED,	EV_CONBAT,		ST_CONBAT,			&fn_connect_battery			 }, // CONNECT BATTERY
		{ ST_CONBAT,		EV_LAUNCH,		ST_LAUNCH,			&fn_pressurized_launch		 }, // LAUNCH
		{ ST_LAUNCH,		EV_BURNOUT,		ST_TERM,			&fn_launch_term				 }, // BURN OUT

		{ ST_ANY,			EV_OVR_PR_LOX,	ST_ANY,				&fn_vent_LOX				 }, // VENT LOX
		{ ST_ANY,			EV_OVR_PR_FUE,	ST_ANY,				&fn_vent_FUE				 }, // VENT FUE
		{ ST_ANY,			EV_EMERG,		ST_INIT,			&fn_EMERGENCY				 },	// STOP PROCEDURE AND CHOOSE NEXT OPTION
		{ ST_ANY,			EV_DATA,		ST_INIT,			&fn_DATA					 }, // DATA
		{ ST_ANY,			EV_CANCEL,		ST_TERM,			&fn_CANCEL					 }, // ABORT ENTIRE PROCESS & DRAIN
		{ ST_ANY,			EV_MANUAL,		ST_TERM,			&fn_MANUAL					 },	// MANUAL
		{ ST_ANY,			EV_ANY,			ST_TERM,			&fn_ERROR 					 }  // ERROR


	};
	
	// STATE FUNCTIONS
	static b1_state fn_init_init(b1_state);
	static b1_state fn_init_idle(b1_state);
	static b1_state fn_idle_fill(b1_state);
	static b1_state fn_fill_pressurize(b1_state);
	static b1_state fn_pressurize_pressurized(b1_state);
	static b1_state fn_connect_battery(b1_state);
	static b1_state fn_pressurized_launch(b1_state);
	static b1_state fn_launch_term(b1_state);

	static b1_state fn_vent_LOX(b1_state);
	static b1_state fn_vent_FUE(b1_state);
	static b1_state fn_EMERGENCY(b1_state);
	static b1_state fn_CANCEL(b1_state);
	static b1_state fn_ERROR(b1_state);
	static b1_state fn_MANUAL(b1_state);
	static b1_state fn_DATA(b1_state);

	static void vent_LOX_pressure(void);
	static void vent_FUE_pressure(void);
	static void launch_countdown(void);
	static void burnout_timer(void);
	static void drain_lines(void);

	// METHODS
	int transCount(void);

	// VARIABLES
	static solenoid* pres_LOX;
	static solenoid* pres_FUE;
	static solenoid* vent_LOX_1;
	static solenoid* vent_LOX_2;
	static solenoid* vent_FUE;
	static mainvalve* main_LOX;
	static mainvalve* main_FUE;
	static sbat* bat_5V;
	static sbat* bat_24V;

private:

	// CONSTRUCTOR
	b1_states();

};