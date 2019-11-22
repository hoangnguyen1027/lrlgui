#pragma once
#include <wiringPi.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
//#include <unistd.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include <ctime>
#include <condition_variable>
//#include <byteswap.h>
#include <cstdint>
#include <ads1115.h>
#include <max31855.h>
#include <wiringPiSPI.h>
#include "logger.h"



// Forward Declaration
class b1_states;
class state_machine;

// METHODS
void getPressureTransducerReadings(void);
int getUserInput(void);


void check_pt_readings();