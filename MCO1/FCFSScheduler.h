#pragma once
#include "Scheduler.h"



class FCFSScheduler : public Scheduler {

public:

  FCFSScheduler();
  ~FCFSScheduler() = default;
  void runScheduler() override;

private: 

};
