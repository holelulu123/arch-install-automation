#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <chrono>
#include <thread>

char getKey() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);           // Save current terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);         // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new terminal settings
    ch = getchar();                          // Read single character
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old settings
    return ch;
}
int main(){
    while(true){
    char key = getKey();
    switch(key) {
        case 'w':
            std::cout << "You pressed W \n";
            break;
        case 's':
            std::cout << "You pressed S \n";
            break;
        case 'a':
            std::cout << "You pressed A \n";
            break;
        case 'd':
            std::cout << "You pressed D \n";
            break;
    }

    }
}