#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <ncurses.h>
#include <mutex>
#include "../include/keyboard.h"

// Function to initialize ncurses
void initializeNcurses() {
    initscr();       // Initialize the ncurses screen
    cbreak();        // Disable line buffering
    noecho();        // Do not echo characters to the screen
    nodelay(stdscr, TRUE); // Make getch() non-blocking
    keypad(stdscr, TRUE);  // Enable special keys like arrows
}

// Function to clean up ncurses
void endNcurses() {
    endwin(); // End ncurses mode
}

// Function to listen for key presses in a separate thread
void listenForKeyPress() {
    while (running.load()) {
        int ch = getch(); // Get a key press
        if (ch != ERR) {  // Check if a key was pressed
            sharedKey.store(ch); // Update the shared variable with the keypress
            if (ch == 'q') {     // Stop the thread when 'q' is pressed
                running.store(false);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep for 100 ms
    }
}


