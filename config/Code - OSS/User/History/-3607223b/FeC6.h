#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <atomic>

extern char last_valid_key;

int Keyboard_To_Direction(int direction);

void getKey();

#endif 