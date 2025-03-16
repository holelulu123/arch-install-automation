#include <complex>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include "../include/snake.h"


int main(){
    int direction = 2;
    std::vector<std::complex<int>> complexArray;
    complexArray.push_back(std::complex<int>(3, 6));
    while(true){
        cout << "Direction is: " << direction << "\n";
        complexArray = snake_movement(complexArray, direction);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}