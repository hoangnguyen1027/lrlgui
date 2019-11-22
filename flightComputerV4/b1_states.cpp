#include "b1_states.h"
#include "state_machine.h"
#include <string>
#include <iostream>
#include "input.h"

// ************************* CREATE HARDWARE ****************************************** // 
// **************** SPECIFY HARDWARE PROPERTIES HERE ********************************** //

// If there are changes in valve configuration (new normally "OPEN" or "CLOSE") just change following setting in the code.
// Type:  NC = Normally Closed ---- NO = Normally Open		   //
// State: OPEN = Currently Open ---- ClOSED = Currently Closed //
// Line:  LOX = Liquid Oxygen ---- FUE = Liquid Methane		   //
// Line:  _5V = 5V Power ---- _24V = 24V Power				   //
// ... PINOUT ... //
// PIN_pres_LOX = 5	//
// PIN_pres_FUE = 13	//
// PIN_VENT_LOX   = 6	//
// PIN_VENT_FUE   = 19	//
// PIN_MAIN_LOX   = 12	//
// PIN_MAIN_FUE   = 16	//
// PIN_BAT_5V     = 20	//
// PIN_BAT_24V    = 21	//

solenoid* b1_states::pres_LOX = new solenoid(PIN_PRES_LOX, solenoid::solenoidType::NC, solenoid::solenoidState::CLOSED, solenoid::solenoidLine::LOX);
solenoid* b1_states::pres_FUE = new solenoid(PIN_PRES_FUE, solenoid::solenoidType::NC, solenoid::solenoidState::CLOSED, solenoid::solenoidLine::FUE);
solenoid* b1_states::vent_LOX_1 = new solenoid(PIN_VENT_LOX_1, solenoid::solenoidType::NO, solenoid::solenoidState::CLOSED, solenoid::solenoidLine::LOX);
solenoid* b1_states::vent_LOX_2 = new solenoid(PIN_VENT_LOX_2, solenoid::solenoidType::NO, solenoid::solenoidState::CLOSED, solenoid::solenoidLine::LOX);
solenoid* b1_states::vent_FUE = new solenoid(PIN_VENT_FUE, solenoid::solenoidType::NO, solenoid::solenoidState::CLOSED, solenoid::solenoidLine::FUE);
mainvalve* b1_states::main_LOX = new mainvalve(PIN_MAIN_LOX, mainvalve::mainType::NC, mainvalve::mainState::CLOSED, mainvalve::mainLine::LOX);
mainvalve* b1_states::main_FUE = new mainvalve(PIN_MAIN_FUE, mainvalve::mainType::NC, mainvalve::mainState::CLOSED, mainvalve::mainLine::FUE);
sbat* b1_states::bat_5V =  new sbat(PIN_BAT_5V, sbat::sbatState::CLOSED);
sbat* b1_states::bat_24V = new sbat(PIN_BAT_24V, sbat::sbatState::CLOSED);
// GET INSTANCE OF STATE MACHINE
state_machine& sm_st = state_machine::getInstance();


// CONSTRUCTOR
b1_states::b1_states() {
}

// STATE FUNCTIONS

// ... fn_init_init ... //
b1_states::b1_state b1_states::fn_init_init(b1_states::b1_state nextState) {
	logger::info(__FILE__, "State Machine started successfully. All valves in their unpowered states");
	return nextState;
}


// ... fn_init_idle ... //
b1_states::b1_state b1_states::fn_init_idle(b1_states::b1_state nextState) {
	
	pres_LOX->close();
	pres_FUE->close();

	vent_LOX_1->close();
	vent_FUE->close();

	logger::info(__FILE__, "pres Valves closed. Vent Valves closed.");
	return nextState;
}

// ... fn_idle_fill ... //
b1_states::b1_state b1_states::fn_idle_fill(b1_states::b1_state nextState) {

	vent_LOX_1->open();
	vent_FUE->open();
	
	logger::info(__FILE__, "pres Valves Closed. Vent Valves open.");
	logger::info(__FILE__, "Begin Filling Sequence.");
	return nextState;
}

// ... fn_fill_pressurize ... //
b1_states::b1_state b1_states::fn_fill_pressurize(b1_states::b1_state nextState) {
	
	pres_LOX->open();
	pres_FUE->open();

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	vent_LOX_1->close();
	vent_FUE->close();

	//std::cout << "Bronco One is being pressurized...\nContinue when pressurization is finished\n";
	logger::info(__FILE__, "Bronco One is being pressurized...");
	logger::info(__FILE__, "Continue when pressurization is finished.");
	return nextState;
}

// ... fn_disconnect_battery ... //
b1_states::b1_state b1_states::fn_connect_battery(b1_states::b1_state nextState) {
	logger::info(__FILE__, "Engaging battery power now");
	logger::info(__FILE__, "Start disconnecting external power");
	bat_24V->batOn();
	bat_5V->batOn();
	return nextState;
}

// ... fn_pressurize_pressurized ... //
b1_states::b1_state b1_states::fn_pressurize_pressurized(b1_states::b1_state nextState) {
	//std::cout << "Bronco One is pressurized and ready to launch\nWaiting on your go...\n";
	logger::info(__FILE__, "Bronco One is pressurized and ready to launch.");
	logger::info(__FILE__, "Waiting on your go...");
	return nextState;
}

// ... fn_pressurized_launch ... //
b1_states::b1_state b1_states::fn_pressurized_launch(b1_states::b1_state nextState) {

	launch_countdown();

	main_FUE->open();

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	main_LOX->open();

	std::thread(burnout_timer).detach();

	return nextState;
}

// ... fn_launch_term ... //
b1_states::b1_state b1_states::fn_launch_term(b1_states::b1_state nextState) {

	pres_LOX->open();
	pres_FUE->open();
	vent_LOX_2->open();
	vent_FUE->open();

	//std::cout << "Bronco One has successfully launched...\n";
	//std::cout << "Good luck on recovery :)\n";
	logger::info(__FILE__, "Bronco One has successfully launched!");
	logger::info(__FILE__, "Good luck on recovery :)");
	return nextState;
}

// ... fn_vent_LOX ... //
b1_states::b1_state b1_states::fn_vent_LOX(b1_states::b1_state nextState) {

	//std::cout << "Venting LOX line\n";
	logger::info(__FILE__, "Venting LOX line");
	std::thread(vent_LOX_pressure).detach();
	return sm_st.getPreviousState();

}

// ... fn_vent_FUE ... //
b1_states::b1_state b1_states::fn_vent_FUE(b1_states::b1_state nextState) {

	//std::cout << "Venting FUE line\n";
	logger::info(__FILE__, "Venting FUE line");
	std::thread(vent_FUE_pressure).detach();
	return sm_st.getPreviousState();

}

// ... fn_EMERGENCY ... //
b1_states::b1_state b1_states::fn_EMERGENCY(b1_states::b1_state nextState) {

	vent_FUE->open();
	vent_LOX_1->open();

	pres_FUE->close();
	pres_LOX->close();

	//std::cout << "Launch has been cancelled\n";
	//std::cout << "Bronco One is being depressurized...\n";
	//std::cout << "press ENTER to continue\n";
	logger::info(__FILE__, "Launch has been cancelled");
	logger::info(__FILE__, "Bronco One is being depressurized...");
	//logger::info(__FILE__, "press ENTER to continue");
	//std::cin.ignore(5, ' ');
	//std::cin.ignore();

	// Try pressurizing again or drain the lines
	char mode;
	//std::cout << "R: return to pressurization state\nD: drain lines\n";S

prompt:
	logger::info(__FILE__, "Select from one of the following:");
	logger::info(__FILE__, "R - return to pressurization state");
	logger::info(__FILE__, "D - drain lines");
	std::cin >> mode;

	switch (mode) {
	case 'R': case 'r':
		//std::cout << "Returning to pressurization state\n";
		//std::cout << "Enter 3 to pressurize\n";
		logger::info(__FILE__, "Returning to ...?");
		return nextState;
		//fn_fill_pressurize(ST_presSURIZE);
		//return ST_FILL;
	case 'D': case 'd':
		//std::cout << "Executing drain procedure\n";
		logger::info(__FILE__, "Executing drain procedure");
		drain_lines();
		return nextState;
	default:
		goto prompt;
		break;
	}
	//return nextState;
}

// ... fn_CANCEL ... //
b1_states::b1_state b1_states::fn_CANCEL(b1_states::b1_state nextState) {

	//std::cout << "Cancel procedure has been executed. Draining fuel lines...\n";
	logger::info(__FILE__, "Cancel procedure has been executed. Draining fuel lines");
	drain_lines();
	return ST_TERM;
}

// ... fn_ERROR ... //
b1_states::b1_state b1_states::fn_ERROR(b1_states::b1_state nextState) {
	//std::cout << "State Machine not constructed properly. Check transition table.\n";
	logger::warn(__FILE__, "State Machine transition table not constructed properly");
	return sm_st.getPreviousState();
}

b1_states::b1_state b1_states::fn_DATA(b1_states::b1_state nextState)
{
	logger::info(__FILE__, "Reading pressure transducer data\n");
	getPressureTransducerReadings();
	logger::info(__FILE__, "Returning to launch sequence.");
	return nextState;
}

// ... fn_MANUAL ... //
b1_states::b1_state b1_states::fn_MANUAL(b1_states::b1_state previousState)
{

	logger::info(__FILE__, "Manual override of valves\n");
	logger::info(__FILE__, "11 - CLOSE pres_LOX 12\t12 - OPEN pres_LOX");
	logger::info(__FILE__, "21 - CLOSE pres_FUE\t22 - OPEN pres_FUE");
	logger::info(__FILE__, "31 - CLOSE vent_LOX_1\t32 - OPEN vent_LOX_1");
	logger::info(__FILE__, "41 - CLOSE vent_LOX_2\t42 - OPEN vent_LOX_2");
	logger::info(__FILE__, "51 - CLOSE vent_FUE\t52 - OPEN vent_FUE");
	logger::info(__FILE__, "61 - CLOSE main_LOX\t62 - OPEN main_LOX");
	logger::info(__FILE__, "71 - CLOSE main_FUE\t72 - OPEN main_FUE");
	logger::info(__FILE__, "Select from one of the following or -1 to return to launch sequence: ");

	int action = 0;
	while (sm_st.getCurrentState() != previousState)
	{
		action = logger::getIntInput();
		switch (action)
		{
		case 11:
			pres_LOX->close();
			logger::info(__FILE__, "pres_LOX is now closed");
			continue;
		case 12:
			pres_LOX->open();
			logger::info(__FILE__, "pres_LOX is now open");
			continue;
		case 21:
			pres_FUE->close();
			logger::info(__FILE__, "pres_FUE is now closed");
			continue;
		case 22:
			pres_FUE->open();
			logger::info(__FILE__, "pres_FUE is now open");
			continue;
		case 31:
			vent_LOX_1->close();
			logger::info(__FILE__, "vent_LOX_1 is now close");
			continue;
		case 32:
			vent_LOX_1->open();
			logger::info(__FILE__, "vent_LOX_1 is now open");
			continue;
		case 41:
			vent_LOX_2->close();
			logger::info(__FILE__, "vent_LOX_2 is now closed");
			continue;
		case 42:
			vent_LOX_2->open();
			logger::info(__FILE__, "vent_LOX_2 is now open");
			continue;
		case 51:
			vent_FUE->close();
			logger::info(__FILE__, "vent_FUE is now closed");
			continue;
		case 52:
			vent_FUE->open();
			logger::info(__FILE__, "vent_FUE is now open");
			continue;
		case 61:
			main_LOX->close();
			logger::info(__FILE__, "main_LOX is now closed");
			continue;
		case 62:
			main_LOX->open();
			logger::info(__FILE__, "main_LOX is now open");
			continue;
		case 71:
			main_FUE->close();
			logger::info(__FILE__, "main_FUE is now closed");
			continue;
		case 72:
			main_FUE->open();
			logger::info(__FILE__, "main_FUE is now open");
			continue;
		case -1:
			logger::info(__FILE__, "Returning to launch sequence:");
			sm_st.setCurrentState(previousState);
			break;
		}
	}
	return previousState;

}

// METHODS

// ... count the number of transitions ... //
int b1_states::transCount(void) {
	return (sizeof(trans) / sizeof(*trans));
}

// ... vent_LOX - to be spawned in a thread to prevent backing up the state machine ... //
void b1_states::vent_LOX_pressure(void) {

	// open LOX vent valve
	// wait for 3 seconds
	// close LOX vent valve

	vent_LOX_1->open();
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	vent_LOX_2->close();

}

// ... vent_FUE - to be spawned in a thread to prevent backing up the state machine ... //
void b1_states::vent_FUE_pressure(void) {

	// open FUE vent valve
	// wait for 3 seconds
	// close FUE vent valve

	vent_FUE->open();
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	vent_FUE->close();

}

// ... launch countdown ... //
void b1_states::launch_countdown(void) {
	
	for (int i = 5; i > -1; i--) {
		std::cout << "-------------[ " << i << " ]-------------" << "\n";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	//std::cout << "Bronco One is launching in...\n";
	logger::info(__FILE__, "Bronco One is launching NOW...");

}

// ... burnout timer ... //
void b1_states::burnout_timer(void) {
	std::this_thread::sleep_for(std::chrono::minutes(1));
	sm_st.pushEvent(b1_states::b1_event::EV_BURNOUT);
}

// ... drain lines ... //
void b1_states::drain_lines(void) {

	std::this_thread::sleep_for(std::chrono::seconds(10));

	//std::cout << "Draining LOX line\n";
	logger::info(__FILE__, "Draining LOX line");
	pres_FUE->close();
	vent_FUE->open();
	vent_LOX_2->close();
	pres_LOX->open();
	main_LOX->open();

	std::this_thread::sleep_for(std::chrono::minutes(5));

	//std::cout << "press 'ENTER' to proceed with draining FUE line\n";
	logger::info(__FILE__, "press ENTER to proceed with draining FUE line");
	std::cin.ignore();
	std::cin.ignore();

	//std::cout << "Draining FUE line...\n";
	logger::info(__FILE__, "Draining FUE line");
	vent_LOX_2->open();
	vent_FUE->close();
	pres_FUE->open();
	main_FUE->open();

	std::this_thread::sleep_for(std::chrono::minutes(5));

	vent_FUE->open();
	//std::cout << "Fuel lines drained successfully\n";
	logger::info(__FILE__, "Fuel lines drained successfully");

}