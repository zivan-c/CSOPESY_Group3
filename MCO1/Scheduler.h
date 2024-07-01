#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Process.h"
#include "CPUCore.h"


class Scheduler {


public:

  Scheduler();
  ~Scheduler() = default;
  virtual void runScheduler();
  bool isRunning;
  int executionDelay;

private:


};
