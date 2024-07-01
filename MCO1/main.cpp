// #include stuff here
// ...
#include "OSConfig.h"
#include "Console.h"
#include "Scheduler.h"

int main()
{
	OSConfig::initialize();
	OSConfig::readConfig();
}