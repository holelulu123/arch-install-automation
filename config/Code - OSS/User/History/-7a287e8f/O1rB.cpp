#include <ctime>
#include <iostream>

int main(){
    // How to work with date and how to format the date as string
    // Very useful for a lot of applications.
    
    time_t t = time(0);
    struct tm *datetime = localtime(&t);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H%M%S", datetime);
    std::cout << "Date: " << buffer << std::endl;
    std::cout << buffer[16] << std::endl;
    return 0;

}
