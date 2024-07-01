#include "Process.h"
#include <string>
#include <iostream>


class ProcessScreen {


public:

  static ProcessScreen* getInstance();
  void attachProces(Process& process);
  void processUserInput(std::string input);
  void initialize();
  void runScreen();

private:

  ProcessScreen();
  ~ProcessScreen() = default;
  bool isRunning;
  Process screenProcess;
  static ProcessScreen* singletonInstance;

};



