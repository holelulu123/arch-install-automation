#include <stdio.h>
#include <string.h>
#define STOP_CHAR ','
#define FINISH_CHAR '*'

typedef enum : __uint8_t{
    test_1,
    test_2,
}Test;


int main() {
    Test my_enum = test_1;
    printf("Number is %p\n", &my_enum);
    return 0;
}

    
