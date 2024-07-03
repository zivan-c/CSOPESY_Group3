#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <conio.h>
#include "marqueeWorkerThread.h"
//#include "functions.h"
#include "Windows.h"

// Command Variables
std::string cmd;
std::mutex cmdMutex;  // Mutex to protect shared command variable

// Marquee Thread Variables
std::unique_ptr<MarqueeWorkerThread> mwThread;
bool mwStart = false;

// Display Rates
const short REFRESH_VAL = 50;  // Refresh Rate
const short POLLING_VAL = 1;   // Polling Rate

// Screen Values
short conX;
short conY;
short conW;
short conH;

// Console
HANDLE hConsole;

// Input Buffer
void polling() {
    std::string userInput;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    short savedX = csbi.dwCursorPosition.X;
    short savedY = csbi.dwCursorPosition.Y;

    while (mwThread->isThreadRunning()) {
        char ch;
        if (_kbhit()) {
            ch = _getch();

            if (ch == '\r') {
                std::lock_guard<std::mutex> lock(cmdMutex);
                cmd = userInput;
                mwThread->addCommand(cmd);
                userInput.clear();

                if (cmd == "exit") {
                    mwThread->dismiss();
                    mwThread->~MarqueeWorkerThread();
                    // Return to Main Menu;
                }

                if (cmd == "clear" || cmd == "cls") {
                    system("clear");
                }

                cmd.clear();

                // Restore the saved cursor position
                SetConsoleCursorPosition(hConsole, { savedX, savedY });
            }
            else if (ch == '\b') {
                if (!userInput.empty()) {
                    userInput.pop_back();
                    // Move cursor back and clear character
                    SetConsoleCursorPosition(hConsole, { (short)(37 + userInput.length()), (short)(conH - 4) });
                    std::cout << ' ';
                    SetConsoleCursorPosition(hConsole, { (short)(37 + userInput.length()), (short)(conH - 4) });
                }

                // Restore the saved cursor position
                SetConsoleCursorPosition(hConsole, { savedX, savedY });
            }
            else {
                userInput += ch;
                // Display the current user input
                SetConsoleCursorPosition(hConsole, { 37, (short)(conH - 4) });
                std::cout << userInput;

                // Restore the saved cursor position
                SetConsoleCursorPosition(hConsole, { (short)(37 + userInput.length()), (short)(conH - 4) });
            }

            // Polling Sleeps
            IETThread::sleep(POLLING_VAL);
        }
    }
}

void process() {
    mwThread->run();
}

void marquee() {
    //Clear Entire Screen
    system("cls");

    // Get Screen Data
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    // Current Cursor Positions
    conX = csbi.dwCursorPosition.X;
    conY = csbi.dwCursorPosition.Y;

    // Console Parameters
    conW = csbi.dwSize.X;
    conH = csbi.dwSize.Y;

    // Initialize Marquee Thread
    mwThread = std::make_unique<MarqueeWorkerThread>(conW, conH, REFRESH_VAL);

    // Start the Marquee Thread
    std::thread marqueeThread(process);
    std::thread pollingThread(polling);

    // Join the threads to ensure the main function waits for their completion
    marqueeThread.join();
    pollingThread.join();

    // Clear Screen, then Return to Main Menu
    system("cls");
}