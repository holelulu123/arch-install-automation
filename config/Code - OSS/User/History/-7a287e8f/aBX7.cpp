#include <ctime>
#include <iostream>

int main(){
    time_t t = time(0);
    struct tm *datetime = localtime(&t);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%A %B %d %H:%M:%S %Y", datetime);
    std::cout << "Date: " << buffer << std::endl;
    return 0;

}
