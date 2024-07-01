#pragma once
#include <fstream>
// reader class for config, when initializing

class OSConfig {
public:
	static int NUM_CPU;
	enum SCHEDULER {
		FCFS,
		SJF,
		RR
	};
	static int TIMESLICE;
	static bool PREEMPTIVE; 
	static int BATCH_PROCESS_FREQ;
	static int MIN_INS;
	static int MAX_INS;
	static int DELAYS_PER_EXEC;

	static void readConfig();
	
	static OSConfig* getInstance();
	static void initialize();
	static void destroy();

	

private:
	static OSConfig* sharedInstance;
	OSConfig();
};
