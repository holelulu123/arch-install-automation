#include <ctime>
#include <iostream>

int main(){
    time_t t = time(0);
    struct tm *datetime = localtime(&t);
    char buffer[80];
    // std::cout << "size of buffer is: " << sizeof(buffer);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H%M%S", datetime);
    std::cout << "Date: " << buffer << std::endl;
    std::cout << buffer[16] << std::endl;
    return 0;

}
