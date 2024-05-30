#include "Console.cpp"
#include <chrono>
#include <ctime>    

class ProcessConsole : public Console {

private:

public: 

    void displayProcess() override {

        std::cout << "You are currently in the PROCESS CONSOLE" << std::endl;
        
    }
};