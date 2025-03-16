#include <ctime>
#include <iostream>

int main(){
    struct tm datetime;
    time(&timestamp);

    std::cout << ctime(&timestamp) << std::endl;

}
