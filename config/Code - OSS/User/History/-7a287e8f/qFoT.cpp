#include <ctime>
#include <iostream>

int main(){
    time_t t = time(0);
    struct tm *datetime = localtime(&t);
    strftime(char *__restrict s, size_t maxsize, const char *__restrict format, datetime);
    

}
