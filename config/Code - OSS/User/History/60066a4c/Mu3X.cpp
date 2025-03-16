#include <complex>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include "../include/snake.h"


int main(){
    uint8_t direction = 1;
    std::vector<std::complex<int>> complexArray;
    complexArray.push_back(std::complex<int>(1, 1));
    while(true){
        complexArray = snake_movement(complexArray, direction);
        // std::cout << "Values are: " << complexArray[0].real() << " " << complexArray[0].imag() << "j" <<"\n"; 
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}