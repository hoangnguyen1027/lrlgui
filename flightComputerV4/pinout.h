#pragma once
#include <iostream>
#include <string>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringPiSPI.h>
#include <wiringSerial.h>
#include <wiringShift.h>

#include "logger.h"
// Delete this comment

// ENUMS
enum PIN_ALIAS {
	PIN_PRES_LOX = 5,
	PIN_PRES_FUE = 13,
	PIN_VENT_LOX_1 = 6,
	PIN_VENT_LOX_2 = 13,
	PIN_VENT_FUE = 19,
	PIN_MAIN_LOX = 12,
	PIN_MAIN_FUE = 16,
	PIN_BAT_5V  = 20,
	PIN_BAT_24V = 21
};

// STRUCTS
typedef struct {
	int pin;	// BCM Pin
	int mode;	// 0:input, 1:output
	int state;  // 0:LOW, 1:HIGH
} pins;

// METHODS
int initializePins(void);
int cleanupPins(void);
int countPins(void);