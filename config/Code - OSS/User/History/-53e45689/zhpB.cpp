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
    for (int value : my_vector){
        std::cout << value << std::endl; 
    }
    // Add the value 6 to the vector
    my_vector.push_back(6);
    
    // Remove the last value of the vector
    my_vector.pop_back();
    
    //Get the first value of the vector
    int vector_first_value = my_vector.front();
    
    //Get the last value of the vector
    int vector_last_value =  my_vector.back();




}