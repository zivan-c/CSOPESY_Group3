#pragma once
#include <string>
#include <thread>
#include <algorithm>
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include "IETThread.h"

class MarqueeWorkerThread : public IETThread {
private:
    bool isRunning = true;
    short x, y;  // Current Position
    short dx, dy;  // Direction of Movement
    const std::string msg = "Hello world in marquee!";
    std::vector<std::string> userInputs;

    int screenWidth;
    int screenHeight;
    int refreshDelay;

    void process() {
        this->x += dx;  // Increase position by dx
        this->y += dy;  // Increase position by dy

        if (x <= 0 || x >= screenWidth - msg.length())  // Invert (dx) if (x) becomes negative or does not fit the console
            dx = -dx;

        if (y <= 4 || y >= screenHeight - 6)  // Invert (dy) if (y) becomes negative or does not fit the console
            dy = -dy;
    }

public:
    MarqueeWorkerThread(short sW, short sH, int refreshDelay) {
        // Set Variables
        this->isRunning = true;
        this->x = 0;
        this->y = 4;  // First 3 Lines are Marquee Console Header
        this->dx = 1;
        this->dy = 1;

        // Get Screen Parameters
        this->screenWidth = sW;  // Gets Console's Width
        this->screenHeight = sH;  // Gets Console's Height
        this->refreshDelay = refreshDelay;
    }

    ~MarqueeWorkerThread() {    // Destructor, Called during dismiss()
        dismiss();
    };

    bool isThreadRunning() {
        return this->isRunning;
    }

    void run() override { // Runs the Marquee Thread
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        while (this->isRunning) {
            this->process();
            render(hConsole, x, y);
            IETThread::sleep(this->refreshDelay);
        }
    }

    void dismiss() { // Stops Marquee Thread
        system("cls");
        this->isRunning = false;
    }

    void addCommand(std::string s) {
        this->userInputs.push_back(s);
    }

    void render(HANDLE console, short x, short y) {
        // Save the current cursor position
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(console, &csbi);
        short savedX = csbi.dwCursorPosition.X;
        short savedY = csbi.dwCursorPosition.Y;

        // Clear the marquee area, but not the entire screen
        for (short i = 4; i < screenHeight - 1; ++i) {
            SetConsoleCursorPosition(console, { 0, i });
            std::cout << std::string(screenWidth, ' ');
        }

        // Redraw the header
        SetConsoleCursorPosition(console, { 0, 0 });
        std::cout << "*****************************************" << std::endl;
        std::cout << "*     Displaying a marquee console!     *" << std::endl;
        std::cout << "*****************************************" << std::endl;

        // Draw the marquee message
        SetConsoleCursorPosition(console, { x, y });
        std::cout << msg;

        // Restore the saved cursor position
        SetConsoleCursorPosition(console, { savedX, savedY });

        // Move cursor to input area
        SetConsoleCursorPosition(console, { 0, (short)(screenHeight - 4) });

        // Move cursor to input area
        SetConsoleCursorPosition(console, { 0, (short)(screenHeight - 4) });
        std::cout << "Enter a command for MARQUEE_CONSOLE: ";

        // Render the last 3 inputted commands
        short commandY = savedY + 1;
        for (int i = max(0, (int)userInputs.size() - 3); i < userInputs.size(); i++) {
            SetConsoleCursorPosition(console, { 37, commandY++ });
            std::cout << userInputs[i];
        }

        // Restore the saved cursor position
        SetConsoleCursorPosition(console, { savedX, savedY });
    }
};