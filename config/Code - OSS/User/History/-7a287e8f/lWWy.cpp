#include <ctime>
#include <iostream>

int main(){
    struct tm datetime;
    int minutes = datetime.tm_hour;

    std::cout << &minutes << std::endl;

}
