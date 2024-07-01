#pragma once
#include "Scheduler.h"

class NonPreemptiveSJF : public Scheduler {

public: 
  void sortReadyQueue(std::vector<std::shared_ptr<Process> >& readyQueue); 
  void runScheduler() override;
private: 

};
