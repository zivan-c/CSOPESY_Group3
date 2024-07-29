#pragma once
#include "Process.h"
#include <memory>
#include <mutex>
#include <semaphore>

class ReadyQueue{
public: 

  std::shared_ptr<Process> removeProcessFromReadyQueue();
  int returnLowestRemainingInstructions();
  bool isReadyQueueAvailable();
  void sortReadyQueue();
  void pushToReadyQueue(std::shared_ptr<Process> process);
  std::shared_ptr<Process> getProcessPointer(std::string process);
  static void initialize();
  static ReadyQueue* getInstance(); 

private:
  std::mutex queueMutex;
  static ReadyQueue* singletonInstance;
  std::vector<std::shared_ptr<Process> > readyQueue; 
  static std::counting_semaphore<1> accessSemaphore;

};
