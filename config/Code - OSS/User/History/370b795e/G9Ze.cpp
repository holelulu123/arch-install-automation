#include <iostream>
#include <complex>
#include <vector>
#include <string>

int main(){
    std::vector<std::complex<float>> my_vector {{1, 5}, {1, 6}, {4, 1}, {-3, 6}};
    std::cout << "first address of the vector: " << &my_vector[0] << std::endl;
    for (const auto value: my_vector){
        std::cout << "Adress With copying: " << &value << std::endl;
    }
    for (const auto& value: my_vector){
        std::cout << "Adress With referencing: " << &value << std::endl;
    }
    
}