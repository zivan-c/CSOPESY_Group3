#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <unordered_map>
#include <memory>
#include "Console.h"

const std::string MAINCON = "MAIN_CONSOLE";     // Constant string representing the "Main Console"
const std::string MMRYCON = "MEMORY_CONSOLE";   // Constant string representing the "Memory Console"
const std::string SCHDCON = "SCHEDULE_CONSOLE"; // Constant string representing the "Schedule Console"
const std::string MARQCON = "MARQUEE_CONSOLE";  // Constant string representing the "Marquee Console"

class ConsoleManager
{
public:
    std::unordered_map<std::string, std::shared_ptr<Console>> conMap;  // Map of available consoles

    //Functions
    static ConsoleManager* getInst();               // Get the singleton instance of the ConsoleManager
    static void initConMgr();                       // Initialize the ConsoleManager
    static void destroy();                          // Destroy the ConsoleManager

    void drawConsole() const;                       // Draw the currently accessed console
    void process() const;                           // Process the currently accessed console
    void changeConsole(std::string name);           // Change the currently accessed console

    void changeScreen(std::string name);            // Change the currently accessed screen
    void registerScreen(std::string name);          // Register a screen into the conMap
    void unregisterScreen(std::string name);        // Unregister a screen from the conMap
    void prvsCon();                                 // Go back to the previously accessed console
    void exitApp();                                 // Exit the application
    bool isRunning() const;                         // Check if the application is running

    HANDLE getConHandle() const;                    // Get the console handle
    void setCursorPosition(int posX, int posY) const;   // Set the cursor position

private:
    ConsoleManager();                                       // Private constructor for singleton pattern
    ~ConsoleManager() = default;                            // Default destructor
    ConsoleManager(ConsoleManager const&) {};               // Private copy constructor
    ConsoleManager& operator=(ConsoleManager const&) {};    // Private assignment operator

    HANDLE conHandle;                               // The console's handle
    static ConsoleManager* cmmnInst;                // Singleton instance of the ConsoleManager

    std::shared_ptr<Console> curCon;                // Currently accessed console
    std::shared_ptr<Console> lastCon;               // Previously accessed console

    bool isConsoleRunning = true;                   // Flag indicating if the console is running
};