#include "Process.h"
#include <string>
#include <iostream>


class ProcessScreen {


public:

  static ProcessScreen* getInstance();
  void attachProcess(std::shared_ptr<Process> process);
  void processUserInput(std::string input);
  static void initialize();
  void runScreen(std::shared_ptr<Process> process);

private:

  ProcessScreen();
  ~ProcessScreen() = default;
  bool isRunning;
  std::shared_ptr<Process> screenProcess;

  static ProcessScreen* singletonInstance;

};



