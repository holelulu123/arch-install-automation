#include <stdio.h>
#include <string.h>
#define STOP_CHAR ','
#define FINISH_CHAR '*'

typedef enum : __uint8_t{
    test_1,
    test_2,
}Test;


int main() {
    Test my_enum = test_2;
    printf("Number is %d\n", sizeof(my_enum));
    return 0;
}

    
