#include <ctime>
#include <iostream>

int main(){
    time_t t = time(0)
    struct tm datetime;
    int wday = datetime.tm_wday;

    std::cout << wday << std::endl;
    

}
