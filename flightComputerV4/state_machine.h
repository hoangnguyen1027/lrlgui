#pragma once
#include <iostream>	
#include <string>
//#include <unistd.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include <queue>
#include <condition_variable>

#include "b1_states.h"
#include "logger.h"


class state_machine {

public:

	// SINGLETON
	static state_machine& getInstance() {
		static state_machine instance;
		return instance;
	}
	state_machine(state_machine const&) = delete;
	void operator=(state_machine const&) = delete;


	// METHODS
	void run(void);
	void pushEvent(b1_states::b1_event);
	std::queue<b1_states::b1_event> getEventQueue(void);

	bool isRunning(void);

	// GETTERS
	b1_states::b1_state getCurrentState(void);
	b1_states::b1_event getCurrentEvent(void);
	b1_states::b1_state getPreviousState(void);
	b1_states::b1_event getPreviousEvent(void);
	void setCurrentState(b1_states::b1_state);
	void setCurrentEvent(b1_states::b1_event);

	// VARIABLES
	std::condition_variable cv_isRunning;
	std::mutex mtx_isRunning;

private:

	// CONSTRUCTOR
	state_machine();

	// VARIABLES
	std::queue<b1_states::b1_event> eventQueue;

	b1_states::b1_state currentState;
	b1_states::b1_event currentEvent;
	b1_states::b1_state previousState;
	b1_states::b1_event previousEvent;

	bool bisRunning;

};
