#include "OSConfig.h"

int OSConfig::NUM_CPU = 1;
int OSConfig::TIMESLICE = 1;
bool OSConfig::PREEMPTIVE = 1;
float OSConfig::BATCH_PROCESS_FREQ = 1.0f;
int OSConfig::MIN_INS = 1;
int OSConfig::MAX_INS = 5;
float OSConfig::DELAYS_PER_EXEC = 0.5f;
OSConfig::SCHEDULER OSConfig::type = FCFS;


OSConfig* OSConfig::sharedInstance = nullptr;
OSConfig* OSConfig::getInstance()
{
	return sharedInstance;
}
OSConfig::OSConfig() {
	
}
void OSConfig::destroy() {
	delete sharedInstance;
}
void OSConfig::readConfig() {
	// Implementation code
	std::ifstream configReader("config.txt");
	// num-CPU
	std::string line;
	std::unordered_map<std::string, std::string> umap;
		while (std::getline(configReader, line)) {
			std::istringstream is(line);
			std::string name;
			std::string value;

			is >> name;

			is >> value;

			umap[name] = value;

	}
	std::unordered_map<std::string, std::string>::iterator itr;
	for (itr = umap.begin(); itr != umap.end(); itr++) {
		std::cout << itr->first << " = " << itr->second << std::endl;
	}
	if (umap["scheduler"] == "\"rr\"") {
		OSConfig::type = RR;
	}
	else if (umap["scheduler"] == "\"sjf\"") {
		OSConfig::type = SJF;
	}
	else if (umap["scheduler"] == "\"fcfs\"") {
		OSConfig::type = FCFS;
	}

	if (umap["preemptive"] == "0") {
		OSConfig::PREEMPTIVE = 0;
	}
	else {
		OSConfig::PREEMPTIVE = 1;
	}
	OSConfig::NUM_CPU = std::stoi(umap["num-cpu"]);
	OSConfig::TIMESLICE = std::stoi(umap["quantum-cycles"]);
	OSConfig::BATCH_PROCESS_FREQ = std::stod(umap["batch-process-freq"]);
	OSConfig::MIN_INS = std::stoi(umap["min-ins"]);
	OSConfig::MAX_INS = std::stoi(umap["max-ins"]);
	OSConfig::DELAYS_PER_EXEC = std::stod(umap["delays-per-exec"]);

	// Scheduler
	// CPUScheduler::initialize(NUM_CPU, algo, DELAYS_PER_EXEC, TIMESLICE, PREEMPTIVE, BATCH_PROCESS_FREQ, MIN_INS, MAX_INS);
}

void OSConfig::initialize() {
	// Implementation code
	sharedInstance = new OSConfig();
}