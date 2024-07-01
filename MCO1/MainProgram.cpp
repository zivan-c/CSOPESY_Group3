#include <iostream>
#include <memory>
#include <string>

#include "ConsoleManager.h"


int main(){
  
  bool running = 1;

  while(running){

        ConsoleManager::start();
        ConsoleManager::getInstance()->startConsole();
        running = 0;
  }      
  
  



};
