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
    while(true);
    std::cout << "Press a key: ";
    char key = getKey();
    std::cout << "\nYou pressed: " << key << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));    

    }
    
}