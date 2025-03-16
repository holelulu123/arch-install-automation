#include <ctime>
#include <iostream>

int main(){
    time_t timestamp;
    time(&timestamp);

    std::cout << ctime(&timestamp) << std::endl;

}
