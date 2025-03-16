#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <atomic>

char last_valid_key = 'd';

int Keyboard_To_Direction(int direction);
void getKey();

#endif 