#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <mutex>
#ifndef __LOGGER__
#define __LOGGER__
namespace logger {

	void info(const char*, std::string);
	void warn(const char*, std::string);

	char getCharInput(void);
	int getIntInput(void);
	
	std::string stripFilename(const char*);
	static std::mutex mtx;

}
#endif

