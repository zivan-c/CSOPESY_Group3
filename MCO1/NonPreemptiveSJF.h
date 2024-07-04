#pragma once
#include "Scheduler.h"

class NonPreemptiveSJF : public Scheduler {

public:
	void runScheduler() override;
private:

};