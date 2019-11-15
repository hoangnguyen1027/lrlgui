#include "solenoid.h"



// CONSTRUCTOR
solenoid::solenoid(int pin, solenoid::solenoidType type,
	solenoid::solenoidState state,
	solenoid::solenoidLine line) 
{

	this->setHardwarePin(pin);
	this->type = type;
	this->line = line;
	setCurrentState(state);

}

// METHODS

// ... check if solenoid is open ... //
bool solenoid::isOpen(void) 
{
	
	switch (state)
	{
	case solenoid::CLOSED:
		return false;
		break;
	case solenoid::OPEN:
		return true;
		break;
	}

	return false;

}

// ... open the solenoid ... //
void solenoid::open(void) 
{

	switch (type)
	{
	case solenoid::NO:
		digitalWrite(this->getHardwarePin(), HIGH);
		break;
	case solenoid::NC:
		digitalWrite(this->getHardwarePin(), LOW);
		break;
	default:
		break;
	}
	this->state = solenoidState::OPEN;

}

// ... close the solenoid ... //
void solenoid::close(void) 
{

	switch (type)
	{
	case solenoid::NO:
		digitalWrite(this->getHardwarePin(), LOW);
		break;
	case solenoid::NC:
		digitalWrite(this->getHardwarePin(), HIGH);
		break;
	default:
		break;
	}
	this->state = solenoidState::CLOSED;

}

// ... toggle the solenoid's state ... //
void solenoid::toggle(void) 
{

	switch (this->state)
	{
	case solenoid::OPEN:
		this->close();
		break;
	case solenoid::CLOSED:
		this->open();
		break;
	default:
		break;
	}

}

// GETTERS
solenoid::solenoidType solenoid::getSolenoidType(void) 
{
	return this->type;
}
solenoid::solenoidState solenoid::getCurrentState(void) 
{
	return this->state;
}
solenoid::solenoidLine solenoid::getSolenoidLine(void) 
{
	return this->line;
}

// ... set the current solenoid state ... //
void solenoid::setCurrentState(solenoid::solenoidState new_state) 
{

	switch (new_state)
	{
	case solenoid::OPEN:
		this->open();
		break;
	case solenoid::CLOSED:
		this->close();
		break;
	default:
		break;
	}

}
