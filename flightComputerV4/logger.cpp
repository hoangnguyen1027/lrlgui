#include "logger.h"

namespace logger {
	
	void info(const char *fname, std::string message) {

		std::lock_guard<std::mutex> lock(mtx);
		std::cout.width(28); std::cout << std::left << "[INFO][" + stripFilename(fname) + "]";
		std::cout.width(20); std::cout << std::left << message + "\n";
		//std::cout.width(5);  std::cout << std::left << round_to_nearest + "\n";
		lock.~lock_guard();

	}
	void warn(const char *fname, std::string message) {

		std::lock_guard<std::mutex> lock(mtx);
		std::cout.width(28); std::cout << std::left << "[WARN][" + stripFilename(fname) + "]";
		std::cout.width(20); std::cout << std::left << message + "\n";
		lock.~lock_guard();

	}

	char getCharInput(void) {

		retry:
		char input;
		std::lock_guard<std::mutex> lock(mtx);
		std::cin >> input;
		if (!std::cin.fail()) {
			lock.~lock_guard();
			return input;
		}
		else {
			logger::warn(__FILE__, "Invalid input");
			goto retry; 
		}

	}

	int getIntInput(void) {

	retry:
		int input = 0;
		std::lock_guard<std::mutex> lock(mtx);
		std::cin >> input;
		if (!std::cin.fail()) {
			lock.~lock_guard();
			return input;
		}
		else {
			logger::warn(__FILE__, "Invalid input");
			goto retry;
		}

	}

	std::string stripFilename(const char *fname) {
		return std::string(fname).substr(std::string(fname).find_last_of("\\/") + 1);
	}

}