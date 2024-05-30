#include <iostream>
#include <string>
#include <memory>
#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>

//function that prints out a text in the console 

//function that takes in text and uses tokenization

//function that tells the time

//function that runs the damn thing


//main console class



class Console {

private: 

   virtual const std::string parseString() = 0;
   virtual void nextConsole(const std::string& consoleChoice) = 0;


public: 
      //string parser method
      //display text on each console
   void printTime() {

      std::time_t t = std::time(0);   // get time now
      std::tm* now = std::localtime(&t);

      std::time_t a = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::tm ltime;
      localtime_r(&a, &ltime);

      std::cout << (now->tm_mon + 1) << '-' 
         << now->tm_mday << '-'
         <<  (now->tm_year + 1900)
         << " " << std::put_time(&ltime, "%H:%M:%S") << '\n';

   }

   virtual void displayProcess() = 0;
   

   //string parser method
   //print text method
   //get date, print date method
   //exit method
   //

};