#include "Console.cpp"
#include <chrono>
#include <ctime>    

class NvidiaConsole : public Console {


public: 

    void displayProcess() override {

        std::cout << "You are currently in the NVIDIA CONSOLE" << std::endl;
    
    }

};