#include <termios.h>
#include <unistd.h>

char last_valid_key;

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

int Keyboard_To_Direction(char key, int direction){
    // Left = 260 -> return 0
    switch(key) {
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
    if (key == 260){
        return 0;
    }
    // Right = 261 -> return 1
    else if (key == 261){
        return 1;

    }
    // Down = 258 -> return 2
    else if (key == 258){
        return 2;
    }
    // Up = 259 -> return 3
    else if (key == 259){
        return 3;

    }
    else{
        return direction;
    }
}

// int main() {
//     // Initialize ncurses
//     initializeNcurses();

//     // Start the keyboard listener in a separate thread
//     std::thread keyListener(listenForKeyPress);

//     std::cout << "Main loop is running. Press keys to see them. Press 'q' to quit." << std::endl;

//     // Main loop
//     while (true) {
//         int key = sharedKey.load();  // Read the latest keypress
//         if (key != -1) {  // If a key was pressed
//             std::cout << "Key pressed: " << key << " (" << char(key) << ")" << std::endl;
//             sharedKey.store(-1);  // Reset the keypress after processing
//         }

//         // Check if 'q' is pressed to quit
//         if (key == 'q') {
//             break;  // Exit the main loop if 'q' is pressed
//         }

//         // Simulate main loop workload
//         std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Example: Sleep for 500ms
//     }

//     // Wait for the key listener thread to finish (though it's infinite, we'll join here for completeness)
//     keyListener.join();

//     // End ncurses
//     endNcurses();

//     std::cout << "Program terminated." << std::endl;
//     return 0;
// }

