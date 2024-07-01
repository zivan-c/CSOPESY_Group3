#include "OSConfig.h"
#include <fstream>
OSConfig* OSConfig::sharedInstance = nullptr;
OSConfig* OSConfig::getInstance()
{
	return sharedInstance;
}
OSConfig::OSConfig() {
	std::ifstream configReader;
}