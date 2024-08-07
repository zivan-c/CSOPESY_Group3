#include "OSConfig.h"

int OSConfig::NUM_CPU = 1;
int OSConfig::TIMESLICE = 1;
bool OSConfig::PREEMPTIVE = 1;
float OSConfig::BATCH_PROCESS_FREQ = 1.0f;
int OSConfig::MIN_INS = 1;
int OSConfig::MAX_INS = 5;
float OSConfig::DELAYS_PER_EXEC = 0.5f;
int OSConfig::MAX_OVR_MEMORY = 1;
int OSConfig::MIN_MEM = 1;
int OSConfig::MAX_MEM = 1;
int OSConfig::MIN_PAGE = 1;
int OSConfig::MAX_PAGE = 1;
OSConfig::SCHEDULER OSConfig::type = FCFS;

bool isPowerOfTwo(int x)
{
	return (x != 0) && ((x & (x - 1)) == 0);
}
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
	/*if (umap["scheduler"] == "\"rr\"") {
		OSConfig::type = RR;
		algo = CPUScheduler::SchedulerAlgorithm::RR;
	}
	else if (umap["scheduler"] == "\"sjf\"") {
		OSConfig::type = SJF;
		algo = CPUScheduler::SchedulerAlgorithm::SJF;
	}
	else if (umap["scheduler"] == "\"fcfs\"") {
		OSConfig::type = FCFS;
		algo = CPUScheduler::SchedulerAlgorithm::FCFS;
	}*/

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
	OSConfig::DELAYS_PER_EXEC = std::stod(umap["delay-per-exec"]);
	OSConfig::MAX_OVR_MEMORY = std::stoi(umap["max-overall-mem"]);
	OSConfig::MIN_MEM = std::stoi(umap["min-mem-per-proc"]);
	OSConfig::MAX_MEM = std::stoi(umap["max-mem-per-proc"]);
	OSConfig::MIN_PAGE = std::stoi(umap["min-page-per-proc"]);
	OSConfig::MAX_PAGE = std::stoi(umap["max-page-per-proc"]);

	/*==if (isPowerOfTwo(std::stoi(umap["max-overall-mem"])))
		OSConfig::MAX_OVR_MEMORY = std::stoi(umap["max-overall-mem"]);
	else
		std::cerr << "Invalid: max memory is not power of two";


	if (isPowerOfTwo(std::stoi(umap["min-mem-per-proc"])))
		OSConfig::MIN_MEM = std::stoi(umap["min-mem-per-proc"]);
	else
			std::cerr << "Invalid: min memory is not power of two";


	if (isPowerOfTwo(std::stoi(umap["max-mem-per-proc"])))
		OSConfig::MAX_MEM = std::stoi(umap["max-mem-per-proc"]);
	else
			std::cerr << "Invalid: max memory is not power of two";


	if (isPowerOfTwo(std::stoi(umap["min-page-per-proc"])))
		OSConfig::MIN_PAGE = std::stoi(umap["min-page-per-proc"]);
	else
			std::cerr << "Invalid: min page is not power of two";
	if (isPowerOfTwo(std::stoi(umap["max-page-per-proc"])))
		OSConfig::MAX_PAGE = std::stoi(umap["max-page-per-proc"]);
	else
			std::cerr << "Invalid: max page is not power of two";*/

	// Scheduler
	// CPUScheduler::initialize(NUM_CPU, algo, DELAYS_PER_EXEC, TIMESLICE, PREEMPTIVE, BATCH_PROCESS_FREQ, MIN_INS, MAX_INS);
}

void OSConfig::initialize() {
	// Implementation code
	sharedInstance = new OSConfig();
}