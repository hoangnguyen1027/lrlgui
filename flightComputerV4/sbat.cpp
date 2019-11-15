#include "sbat.h"

// CONSTRUCTOR
sbat::sbat(int pin,
	sbat::sbatState state){

	this->setHardwarePin(pin);
	setCurrentState(state);

}
// METHODS

// ... check if sbat is open ... //
// ... check if b1 is using battery power ... //
bool sbat::isOpen(void) {

	switch (state)
	{
	case sbat::CLOSED:
		return false;
		break;
	case sbat::OPEN:
		return true;
		break;
	}

	return false;
}

// ... turn on battery power ... //
void sbat::batOn(void) {
	digitalWrite(this->getHardwarePin(), LOW);
	//this->state = sbatState::CLOSED;
}
// ... turn off battery power ... //
void sbat::batOff(void) {
	digitalWrite(this->getHardwarePin(), HIGH);
	//this->state = sbatState::CLOSED;
}


// GETTERS
sbat::sbatState sbat::getCurrentState(void) {
	return this->state;
}

// ... set the current solenoid state ... //
void sbat::setCurrentState(sbat::sbatState new_state) {

	switch (new_state)
	{
	case sbat::OPEN:
		this->batOff();
		break;
	case sbat::CLOSED:
		this->batOn();
		break;
	default:
		break;
	}

}