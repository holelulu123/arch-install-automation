#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <list>

#include <iostream>

int main(){
    // Vector Object
    // For loop on the vector values
    std::vector<int> my_vector = {6,2,3,1,5,6,7};
    for (int i : my_vector){
        std::cout << i << std::endl; 
    }
    // Add another value to the vector
    my_vector.push_back(6);

}