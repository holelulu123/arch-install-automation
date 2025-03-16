#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <atomic>

extern std::atomic<bool> running;
extern std::atomic<int> sharedKey;

void initializeNcurses();
void endNcurses();
void listenForKeyPress();

#endif 