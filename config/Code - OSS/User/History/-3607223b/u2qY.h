#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <atomic>

extern std::atomic<bool> running;
extern std::atomic<int> sharedKey;

int Keyboard_To_Direction(int key, int direction);

void initializeNcurses();
void endNcurses();
void listenForKeyPress();

#endif 