#include "input.h"
#include "b1_states.h"
#include "state_machine.h"
#include <iomanip> 

state_machine& input_sm = state_machine::getInstance();
std::unique_lock<std::mutex> lock(input_sm.mtx_isRunning);


// ... Read User Input ... //

int getUserInput(void) {

	b1_states& states = b1_states::getInstance();
	
	input_sm.cv_isRunning.wait(lock, [] {return input_sm.isRunning(); });
	//std::cout << "Gathering User Input...\n";
	logger::info(__FILE__, "Gathering User Input");

	int input;
	char confirm;

	while (input_sm.isRunning()) {

		std::cin >> input;
		//input = logger::getIntInput();

		if (static_cast<b1_states::b1_event>(input) == b1_states::EV_LAUNCH) {

			//std::cout << "Are you sure you want to launch? Y/N\n";
			
			prompt:
			logger::info(__FILE__, "Are you sure you want to launch? Y/N");
			std::cin >> confirm;
			//confirm = logger::getCharInput();

			switch (confirm) {
			case 'Y':
				input_sm.pushEvent(static_cast<b1_states::b1_event>(input));
				break;
			case 'y':
				input_sm.pushEvent(static_cast<b1_states::b1_event>(input));
				break;
			case 'N': 
				input_sm.pushEvent(states.EV_CANCEL);
				break;
			case 'n':
				input_sm.pushEvent(states.EV_CANCEL);
				break;
			default:
				goto prompt;
				break;
			}

		}
		else {
			input_sm.pushEvent(static_cast<b1_states::b1_event>(input));
		}

	}
	//std::cout << "Gathered User Input\n";
	logger::info(__FILE__, "Gathered User Input");
	return 0;
}
// ... Read Pressure Transducer Data ... //
void getPressureTransducerReadings(void)
{
	b1_states& states = b1_states::getInstance();

	//input_sm.cv_isRunning.wait(lock, [] {return input_sm.isRunning(); });
	//std::cout << "Getting Pressure Transducer Readings..\n";
	logger::info(__FILE__, "Getting Pressure Transducer Readings");
	//lock.unlock();

	// DATA FILE FOR LOGGING
	std::ofstream data_file;
	
	// ... adc parameters ... // 
	int pinbase1 = 100;
	int pinbase2 = 64;
	int LOX_i2address = 0x49;
	int FUE_i2address = 0x48;
	int adcbits = 16;
	int chan[2] = { 0,1 };
	int max_val = 750;
	float correction = -89.6604;
	int LOX_0; float LOXvol_0; float LOXpsi_0 = 0.0;// channel 0 for LOX ADC
	int LOX_1; float LOXvol_1; float LOXpsi_1 = 0.0;// channel 1
	int LOX_2; float LOXvol_2; float LOXpsi_2 = 0.0;// channel 2
	int LOX_3; float LOXvol_3; float LOXpsi_3 = 0.0;// channel 3
	int FUE_0; float FUEvol_0; float FUEpsi_0 = 0.0;// channel 0 for FUE ADC
	int FUE_1; float FUEvol_1; float FUEpsi_1 = 0.0;// channel 1
	int FUE_2; float FUEvol_2; float FUEpsi_2 = 0.0;// channel 2
	int FUE_3; float FUEvol_3; float FUEpsi_3 = 0.0;// channel 3
	float a2dpsi_correction;
	int max_LOX_pressure = 330;
	int max_FUE_pressure = 330;
	int count = 0;
	// try to connect to the ADC
	if (ads1115Setup(pinbase1, LOX_i2address) < 0) {
		//std::cout << "Failed setting up I2C device :(\n";
		logger::warn(__FILE__, "Failed setting up LOX I2C device :(");
	}
	else if (ads1115Setup(pinbase2, FUE_i2address) < 0) {
		//std::cout << "Failed setting up I2C device :(\n";
		logger::warn(__FILE__, "Failed setting up FUE I2C device :(");
	}
	else {

		//std::cout << "Reading PT data...\n";
		// for logging data
		data_file.open("pt_data.txt");
		data_file << "Pressure Transducer Data Log\n";
		data_file << "LOX_1\t\t";
		data_file << "LOX_2\t\tLOX_3\t\tLOX_4\t"; // comment this line out for only LOX_1
		data_file << "\tFUE_1";
		data_file << "\t\tFUE_2\t\tFUE_3\t\tFUE_4\n"; // comment this line out for only FUE_1

		while (count < 20)
		{
			for (int j : chan)
			{
				// ADC conversions
				LOX_0 = analogRead(pinbase1);
				LOXvol_0 = LOX_0 * 4.096 / (pow(2, (adcbits - 1)) - 1);
				LOXpsi_0 = LOXvol_0 * 198.66f - 112.66f + 23 - correction;

				LOX_1 = analogRead(pinbase1 + 1);
				LOXvol_1 = LOX_1 * 4.096 / (pow(2, (adcbits - 1)) - 1);
				LOXpsi_1 = LOXvol_1 * 198.66f - 112.66f + 23 - correction;

				LOX_2 = analogRead(pinbase1 + 2);
				LOXvol_2 = LOX_2 * 4.096 / (pow(2, (adcbits - 1)) - 1);
				LOXpsi_2 = LOXvol_2 * 198.66f - 112.66f + 23 - correction;

				LOX_3 = analogRead(pinbase1 + 3);
				LOXvol_3 = LOX_3 * 4.096 / (pow(2, (adcbits - 1)) - 1);
				LOXpsi_3 = LOXvol_3 * 198.66f - 112.66f + 23 - correction;


				FUE_0 = analogRead(pinbase2);
				FUEvol_0 = FUE_0 * 4.096 / (pow(2, (adcbits - 1)) - 1);
				FUEpsi_0 = FUEvol_0 * 198.66f - 112.66f + 23 - correction;

				FUE_1 = analogRead(pinbase2);
				FUEvol_1 = FUE_1 * 4.096 / (pow(2, (adcbits - 1)) - 1);
				FUEpsi_1 = FUEvol_1 * 198.66f - 112.66f + 23 - correction;

				FUE_2 = analogRead(pinbase2);
				FUEvol_2 = FUE_2 * 4.096 / (pow(2, (adcbits - 1)) - 1);
				FUEpsi_2 = FUEvol_2 * 198.66f - 112.66f + 23 - correction;

				FUE_3 = analogRead(pinbase2);
				FUEvol_3 = FUE_3 * 4.096 / (pow(2, (adcbits - 1)) - 1);
				FUEpsi_3 = FUEvol_3 * 198.66f - 112.66f + 23 - correction;
				//a2dpsi = a2dval * max_val / (pow(2, (adcbits - 1) - 1)) - correction;

				data_file << std::setprecision(4) << std::fixed << LOXpsi_0 << "\t";
				data_file << std::setprecision(4) << std::fixed << LOXpsi_1 << "\t" << LOXpsi_2 << "\t" << LOXpsi_3 << "\t"; // comment this line out for only LOX_1
				data_file << std::setprecision(4) << std::fixed << FUEpsi_0 << "\t";
				data_file << std::setprecision(4) << std::fixed << FUEpsi_1 << "\t" << FUEpsi_2 << "\t" << FUEpsi_3 << "\n"; // comment this line out for only FUE_1

				if (j == 0)
				{
					std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
					printf("LOX_1: %.3f LOX_2: %.3f LOX_3: %.3f LOX_4: %.3f FUE_1: %.3f FUE_2: %.3f FUE_3: %.3f FUE_4: %.3f\n ", LOXpsi_0, LOXpsi_1, LOXpsi_2, LOXpsi_3,
						FUEpsi_0, FUEpsi_1, FUEpsi_2, FUEpsi_3);

					if (LOXpsi_0 >= max_LOX_pressure) {
						logger::warn(__FILE__, "LOX_1 line is at operating pressure");
						if (!states.vent_LOX_1->isOpen()) {
							input_sm.pushEvent(states.EV_OVR_PR_LOX);
						}
					}
					else if (LOXpsi_1 >= max_LOX_pressure) {
						logger::warn(__FILE__, "LOX_2 line is at operating pressure");
						if (!states.vent_LOX_1->isOpen()) {
							input_sm.pushEvent(states.EV_OVR_PR_LOX);
						}

					}
					else;
				}
				if (j == 1)
				{
					//printf("FUE Tank Pressure: %f [psi]\n", a2dpsi);
					if (FUEpsi_0 >= max_FUE_pressure) {
						logger::warn(__FILE__, "FUE_1 line is at operating pressure");
						if (!states.vent_FUE->isOpen()) {
							input_sm.pushEvent(states.EV_OVR_PR_FUE);
						}
					}
					else if (FUEpsi_1 >= max_FUE_pressure) {
						logger::warn(__FILE__, "FUE_2 line is at operating pressure");
						if (!states.vent_FUE->isOpen()) {
							input_sm.pushEvent(states.EV_OVR_PR_FUE);
						}
					}
					else;

				}
			}
			count++;
		}
		data_file.close();
	}
	//std::cout << "Gathered Pressure Transducer Readings\n";
	logger::info(__FILE__, "Gathered Pressure Transducer Readings");
}

void check_pt_readings(void) {

	// ... adc parameters ... // 
	int pinbase = 100;
	int i2cloc = 0x48;
	int adcbits = 16;
	int chan[2] = { 0,1 };
	int max_val = 750;
	int correction = -82;
	int a2dval;
	float a2dvol;
	float a2dpsi;
	int max_LOX_pressure = 330;
	int max_FUE_pressure = 330;

	// try to connect to the ADC
	if (ads1115Setup(pinbase, i2cloc) < 0) {
		logger::warn(__FILE__, "Failed setting up I2C device :(");
		return;
	}
	else {

	loop:
		logger::info(__FILE__, "Checking PT readings...");
		for (int i = 1; i <= 100; i++) {

			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			for (int j : chan) {

				a2dval = analogRead(pinbase + j);
				a2dvol = a2dval * 4.096 / (pow(2, (adcbits - 1)) - 1);
				a2dpsi = a2dvol * 198.66f - 112.66f + 23;
				//a2dpsi = a2dval * max_val / (pow(2, (adcbits - 1) - 1)) - correction;

				if (j == 0) {
					std::cout << "chan" << j << "\t" << a2dpsi << "\t";
					if (a2dpsi >= max_LOX_pressure) {
						logger::warn(__FILE__, "LOX line is at operating pressure");
					}
				}
				if (j == 1) {
					std::cout << "chan" << j << "\t" << a2dpsi << "\n";
					if (a2dpsi >= max_FUE_pressure) {
						logger::warn(__FILE__, "FUE line is at operating pressure");
					}

				}

			}

		}
		char check;
		std::cin >> check;
		//check = logger::getCharInput();
		switch (check) {
		case 'Y':
			logger::info(__FILE__, "PT readings look good ;)");
			return;
			break;
		case 'N':
		default:
			goto loop;
			break;
		}

	}

}
// ... MAX31855 Set Up ... //
int max31855Setup(const int pinBase, int spiChannel)
{
	struct wiringPiNodeStruct *node;
	if (wiringPiSPISetup(spiChannel, 5000000) < 0)
	//{
	//	logger::info(__FILE__, "MAX31855 Failed to run");
	//}
		return FALSE;

	node = wiringPiNewNode(pinBase, 8); // ... GPIO
	node->fd			= spiChannel;
	//node->analogRead	= myAnalogread;

	return TRUE;
}

//static int myAnalogRead(struct wiringPiNodeStruct *node, int pin)
