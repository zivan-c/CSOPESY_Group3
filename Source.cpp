#include <iostream>
#include <ostream>
#include <string>



void displayHelp(){
    std::cout << "\nCommand list: \n";
    std::cout << "help          displays commands\n";
    std::cout << "marquee       display marquee console\n";
    std::cout << "process-smi   display process SMI\n";
    std::cout << "nvidia-smi    display nvidia SMI\n";
    std::cout << "screen        view process-specific screen\n";
    std::cout << "clear         clear screen\n";
    std::cout << "exit          exit OS\n";
}
void printHeader(){
    std::cout << "------------------------------------- \n";
    std::cout << "Welcome to CSOPESY Group 3's Lite OS! \n";
    std::cout << "                                      \n\n\n\n";
    std::cout << "------------------------------------- \n";
    
    
}

int main(){
    int exitFlag = 0; 
    printHeader();
    while(exitFlag == 0){
        std::cout << "Please enter command here (use help for a list of commands!): ";
        std::string command;
        std::getline(std::cin, command);

        if(command == "help"){
            displayHelp();
            
        }
        else if(command == "marquee"){
            std::cout << "marquee command recognized. Doing something.\n";
        }
        else if(command == "screen"){
            std::cout << "screen command recognized. Doing something.\n";
        }
        else if(command == "process-smi"){
            std::cout << "process-smi command recognized. Doing something.\n";
        }
        else if(command == "nvidia-smi"){
            std::cout << "nvidia-smi command recognized. Doing something.\n";
        }
        else if(command == "clear"){
            system("cls");
            printHeader();
        }
        else if(command == "exit"){
            exitFlag = 1;
        }
        else{
            std::cout << "Command not recognized.\n";
        }

    }
    return 0;
}