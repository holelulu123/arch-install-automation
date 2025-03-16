#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <ncurses.h>
#include <mutex>

std::atomic<bool> running(true);  // Atomic variable to control the listener thread
std::atomic<int> sharedKey(-1);  // Atomic variable to store the keypress value

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

int main() {
    initializeNcurses();

    // Start the listener in a separate thread
    std::thread keyListener(listenForKeyPress);

    // Main thread can perform other tasks
    std::cout << "Press keys to see their output. Press 'q' to quit." << std::endl;

    while (running.load()) {
        int key = sharedKey.load(); // Get the latest key press
        std::cout << "Key pressed: " << key << " (" << char(key) << ")" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Sleep to reduce CPU usage
    }

    keyListener.join(); // Wait for the key listener thread to finish
    endNcurses();
    std::cout << "Program terminated." << std::endl;

    return 0;
}
