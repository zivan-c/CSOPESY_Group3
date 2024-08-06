#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <unordered_map>
// reader class for config, when initializing

class OSConfig {
public:
	static int NUM_CPU; //ok
	enum SCHEDULER { //ok
		FCFS,
		SJF,
		RR
	};
	static SCHEDULER type;
	static int TIMESLICE; //ok
	static bool PREEMPTIVE;
	static float BATCH_PROCESS_FREQ;
	static int MIN_INS;
	static int MAX_INS;
	static float DELAYS_PER_EXEC;
	static int MAX_OVR_MEMORY;
	static int MIN_MEM;
	static int MAX_MEM;
	static int MIN_PAGE;
	static int MAX_PAGE;

	static void readConfig();

	static OSConfig* getInstance();
	static void initialize();
	static void destroy();


private:
	static OSConfig* sharedInstance;
	OSConfig();
};