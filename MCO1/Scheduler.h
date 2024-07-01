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
  virtual void runScheduler();
  bool isRunning;
  int executionDelay;

private:


};
