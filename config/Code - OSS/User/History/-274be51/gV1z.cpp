#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <chrono>
#include <thread>

char last_valid_key;

void getKey() {
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
int main(){
    std::thread t(getKey);
    while(true){
        switch(last_valid_key) {
            case 'w':
                last_valid_key = 'w';
                break;
            case 's':
                last_valid_key = 's';
                break;
            case 'a':
                last_valid_key = 'a';
                break;
            case 'd':
                last_valid_key = 'd';
                break;
    }

    }
}