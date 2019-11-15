#include "mainvalve.h"


// CONSTRUCTOR
mainvalve::mainvalve(int pin, mainvalve::mainType type, 
	mainvalve::mainState state, mainvalve::mainLine line) {

	this->setHardwarePin(pin);
	this->type = type;
	this->state = state;
	this->line = line;
	initialize();

}


// METHODS

// ... initialize the mainvalve ... //
void mainvalve::initialize(void) {

	digitalWrite(this->getHardwarePin(), HIGH);

	switch (type)
	{
	case mainvalve::NO:
		state = mainState::OPEN;
		break;
	case mainvalve::NC:
		state = mainState::CLOSED;
		break;
	default:
		break;
	}

}

void mainvalve::close(void)
{

	switch (type)
	{
	case mainvalve::NO:
		digitalWrite(this->getHardwarePin(), LOW);
		break;
	case mainvalve::NC:
		digitalWrite(this->getHardwarePin(), HIGH);
		break;
	default:
		break;
	}
	this->state = mainState::CLOSED;

}

void mainvalve::open(void)
{

	switch (type)
	{
	case mainvalve::NO:
		digitalWrite(this->getHardwarePin(), HIGH);
		break;
	case mainvalve::NC:
		digitalWrite(this->getHardwarePin(), LOW);
		break;
	default:
		break;
	}
	this->state = mainState::OPEN;

}

// ... Cut power to the mainvalve to prevent overcurrenting ... //
void mainvalve::cutPower(int delay, int pin) {
	std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	digitalWrite(pin, HIGH);
}

// GETTERS
mainvalve::mainType mainvalve::getmainType(void) {
	return this->type;
}
mainvalve::mainState mainvalve::getCurrentState(void) {
	return this->state;
}
mainvalve::mainLine mainvalve::getMainLine(void) {
	return this->line;
}
