#pragma once
#include "Scheduler.h"

class PreemptiveSJF : public Scheduler {

public: 

  PreemptiveSJF();
  ~PreemptiveSJF() = default;
  void runScheduler() override; 
  void sortReadyQueue(std::vector<std::shared_ptr<Process>>& readyQueue); 


private: 

};
