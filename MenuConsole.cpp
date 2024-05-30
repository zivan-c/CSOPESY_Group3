#include "MarqueeConsole.cpp"
#include "NvidiaConsole.cpp"
#include "ProcessConsole.cpp"
#include "Console.cpp"
#include <string>
#include <memory>
#include <chrono>
#include <ctime>    


class MenuConsole : public Console {

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
        std::cout << "------------------------------------- \n";
        std::cout << "Welcome to CSOPESY Group 3's Lite OS! \n";
        std::cout << "\n _____ _____ _____ _____ _____ _____ __ __    _____     _       _    _____ _____ \n";
        std::cout << "|     |   __|     |  _  |   __|   __|  |  |  |   __|___| |_ ___|_|  |     |   __|\n";
        std::cout << "|   --|__   |  |  |   __|   __|__   |_   _|  |  |  |___|  _|  _| |  |  |  |__   |\n";
        std::cout << "|_____|_____|_____|__|  |_____|_____| |_|    |_____|   |_| |_| |_|  |_____|_____|\n";
        std::cout << "\n------------------------------------- \n";

        std::cout << "\nCommand list: \n";
        std::cout << "screen -s marquee       display marquee console\n";
        std::cout << "screen -s process  display process SMI console\n";
        std::cout << "screen -s nvidia   display nvidia process console\n";
        std::cout << "screen -s main          display main menu console\n";
        std::cout << "exit                    display main menu console as well\n";

        printTime();

        std::string command = parseString();
        nextConsole(command);


        //Timestamp of when the screen is created in (MM/DD/YYYY, HH:MM:SS AM/PM) format.

    }
};





int main() {

}