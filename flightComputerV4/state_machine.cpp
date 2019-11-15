#include "state_machine.h"

// instance b1_states
b1_states& states = b1_states::getInstance();


// CONSTRUCTOR
state_machine::state_machine() {

	currentState = states.ST_INIT;
	currentEvent = states.EV_INIT;
	pushEvent(currentEvent);
	previousState = currentState;
	previousEvent = currentEvent;

	bisRunning = false;

}

// MAIN LOOP
void state_machine::run(void) {

	bisRunning = true;
	cv_isRunning.notify_all();

	//std::cout << "Starting State Machine\n";
	logger::info(__FILE__, "Starting State Machine");
	// while currentState != ST_TERM
	//		while !eventQueue.empty()
	//			if state == currentstate or state == any state
	//				if event == currentevent or event == any event
	//					change the state

	while (currentState != states.ST_TERM) {

		if (!eventQueue.empty()) {

			currentEvent = eventQueue.front();

			for (int i = 0; i < states.transCount(); i++) {

				if (currentState == states.trans[i].st || states.ST_ANY == states.trans[i].st) {
					if (currentEvent == states.trans[i].ev || states.EV_ANY == states.trans[i].ev) {
						previousState = currentState;
						currentState = (states.trans[i].fn)(states.trans[i].new_st);
						eventQueue.pop();
						break;
					}
				}

			}

		}

	}
	bisRunning = false;
}

// METHODS
void state_machine::pushEvent(b1_states::b1_event ev) {
	eventQueue.push(ev);
}
bool state_machine::isRunning(void) {
	return bisRunning;
}

// GETTERS
std::queue<b1_states::b1_event> state_machine::getEventQueue(void) {
	return eventQueue;
}
b1_states::b1_state state_machine::getCurrentState(void) {
	return currentState;
}

b1_states::b1_event state_machine::getCurrentEvent(void) {
	return currentEvent;
}

b1_states::b1_state state_machine::getPreviousState(void) {
	return previousState;
}
b1_states::b1_event state_machine::getPreviousEvent(void) {
	return previousEvent;
}

// SETTERS
void state_machine::setCurrentState(b1_states::b1_state st) {
	currentState = st;
}

void state_machine::setCurrentEvent(b1_states::b1_event ev) {
	currentEvent = ev;
}