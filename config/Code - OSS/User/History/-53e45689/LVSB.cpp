#include <vector>
#include <iostream>

int main(){
    // For loop on the vector values
    // Vector
    std::vector<int> my_vector = {6,2,3,1,5,6,7};
    for (int i : my_vector){
        std::cout << i << std::endl; 
    }

}