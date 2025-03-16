#include <ctime>
#include <iostream>

int main(){
    time_t t = time(0);
    struct tm *datetime = localtime(&t);
    char buffer[80];
    strftime(, , , datetime);
    

}
