#pragma once
#include "Scheduler.h"

class NonPreemptiveSJF : public Scheduler {

public: 

  NonPreemptiveSJF();
  ~NonPreemptiveSJF();
  void sortReadyQueue(std::vector<std::shared_ptr<Process> >& readyQueue); 
  void runScheduler() override;

private: 

};
