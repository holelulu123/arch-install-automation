#include <ctime>
#include <iostream>

int main(){
    time_t t = time(0);
    struct tm *datetime = localtime(&t);
    std::cout << datetime->tm_hour << std::endl;
    

}
