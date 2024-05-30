#include "Console.cpp"
#include <chrono>
#include <ctime>    

class NvidiaConsole : public Console {

private:

    const std::string parseString() override {

        std::string commandString;
        std::cin >> commandString;
        std::stringstream ss(commandString);

        std::vector<std::string> tokens;
        std::string placeholder; 

        while(getline(ss, placeholder, ' ')){ 
            tokens.push_back(placeholder);
        }

        std::string returnString;

        if (tokens[0] == "exit") {
            return "exit";
        }
        else {
            return tokens[2];
        }

    }

    void nextConsole(const std::string& consoleChoice) override {

        if (consoleChoice == "exit"){
            
        }
        else
        {
            
        }

    }

public: 

    void displayProcess() override {

        std::cout << "You are currently in the NVIDIA CONSOLE" << std::endl;
        printTime();

        std::string command = parseString();
        nextConsole(command);

    
    }

};