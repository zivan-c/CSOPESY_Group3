#pragma once
#include "Scheduler.h"

class PreemptiveSJF : public Scheduler {

public:
	void runScheduler() override;
private:

};