#include "RRScheduler.h"
#include "CPUScheduler.h"
#include <thread>

void RRScheduler::runScheduler(){

  std::thread schedulerThread;
  isRunning = 1;

  schedulerThread = std::thread([this](){

    //Implementation is within the CPUCore class
    //
  });
  
  schedulerThread.detach();

};
