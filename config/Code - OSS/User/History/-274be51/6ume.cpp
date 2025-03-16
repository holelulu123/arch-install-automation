#include <iostream>
#include <chrono>
#include <thread>

char last_press; 


int main(){
    while(true){
        std::cin >> last_press;
        std::cout << "Last press is: " << last_press << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
}