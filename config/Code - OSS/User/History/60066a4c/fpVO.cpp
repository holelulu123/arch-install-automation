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
    for (int i=0; i < complexArray.size(); i++){
        complexArray[i] = std::complex<double>(complexArray[i].real() - 1, complexArray[i].imag());
        std::cout << "Real Value is: " << complexArray[i].real() << "\n"; 
        std::cout << "Imag Value is: " << complexArray[i].imag() << "\n"; 
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(800));

}