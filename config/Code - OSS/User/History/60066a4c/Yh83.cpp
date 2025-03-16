#include <complex>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include "../include/snake.h"


int main(){
    int direction = 1;
    int rows = 30, cols = 50;
    bool snake_good; 
    std::vector<std::complex<int>> complexArray;
    complexArray.push_back(std::complex<int>(3, 6));
    while(true){
        complexArray = snake_movement(complexArray, direction);
        // Handling Snake game tile limit
        snake_good = check_snake_limit(complexArray, rows, cols);
        if (!snake_good){
            std::cout << "GAME OVER \n";
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}