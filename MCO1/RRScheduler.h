#pragma once
#include "Scheduler.h"

class RRScheduler : public Scheduler {

public:

  RRScheduler();
  ~RRScheduler() = default;
  void runScheduler() override; 

private:

};

