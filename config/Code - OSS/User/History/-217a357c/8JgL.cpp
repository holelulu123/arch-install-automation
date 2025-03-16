#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include "../include/keyboard.h"

void getKey() {
    while(true){
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);           // Save current terminal settings
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);         // Disable canonical mode and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new terminal settings
        ch = getchar();                          // Read single character
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old settings
        last_valid_key = ch;
    }
}

int Keyboard_To_Direction(int direction){
    switch(last_valid_key) {
    case 'a':
        return 0; 
    case 'd':
        return 1;
    case 's':
        return 2;
    case 'w':
        return 3;

    // Left = 260 -> return 0
    // Right = 261 -> return 1
    // Down = 258 -> return 2
    // Up = 259 -> return 3
    }
    return direction;
}

