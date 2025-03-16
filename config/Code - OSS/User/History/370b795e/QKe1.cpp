#include <iostream>
#include <complex>
#include <vector>
#include <string>

int main(){
    std::vector<std::string> my_vector {{1, 5}, {1, 6}, {4, 1}, {-3, 6}};
    for (const auto value: my_vector){
        std::cout << "value is: " << value.<< std::endl; 
    }
}