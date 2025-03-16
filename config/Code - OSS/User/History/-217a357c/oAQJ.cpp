#include <iostream>
#include <thread>
#include <atomic>
#include <ncurses.h>

using namespace std;


std::atomic<bool> running(true);

void listenForKeyPress() {
    initscr();       // Initialize the ncurses screen
    cbreak();        // Disable line buffering
    noecho();        // Do not echo characters to the screen
    nodelay(stdscr, TRUE); // Make getch() non-blocking
    keypad(stdscr, TRUE);  // Enable special keys like arrows

    while (running.load()) {
        int ch = getch()
        if (ch != ERR) { // Detect keypress
            char ch = _getch(); // Get the pressed key
            std::cout << "Key pressed: " << ch << std::endl;
            if (ch == 'q') { // Example: quit when 'q' is pressed
                running.store(false);
            }
        }
    }
}
char listener(){
    thread KeyboardListener(listenForKeyPress);
    cout << "Press Keys to see their output. press 'q' to quit." << endl;
    KeyboardListener.join();
    return 'a';
}