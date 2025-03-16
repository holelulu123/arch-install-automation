#include <complex>
#include <vector>
#include <iostream>

int main(){
    std::vector<std::complex<double>> complexArray;
    complexArray.push_back(std::complex<double>(1.0, 2.0));
    complexArray.push_back(std::complex<double>(-5.0, -2.0));
    for (int i=0; i < complexArray.size(); i++){
        
    }
    std::cout << "Size of complex array is: " << complexArray.size() << "\n";
}