#include <complex>
#include <vector>
#include <iostream>
#include "../include/snake.h"

int main(){
    std::vector<std::complex<double>> complexArray;
    complexArray.push_back(std::complex<double>(1.0, 1.0));
    
    for (int i=0; i < complexArray.size(); i++){
        complexArray[i] = std::complex<double>(complexArray[i].real() - 1, complexArray[i].imag());
        std::cout << "Real Value is: " << complexArray[i].real() << "\n"; 
        std::cout << "Imag Value is: " << complexArray[i].imag() << "\n"; 
    }
}