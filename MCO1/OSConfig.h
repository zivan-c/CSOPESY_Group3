#pragma once
#include <fstream>
// reader class for config, when initializing

class OSConfig {
public:
	static int NUM_CPU;
	enum SCHEDULER {
		FCFS,
		NPSJF,
		PSJF,
		RR,
	};

};
