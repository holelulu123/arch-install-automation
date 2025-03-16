#include <stdio.h>
#include <string.h>
#define STOP_CHAR ','
#define FINISH_CHAR '*'

typedef enum : __uint8_t {
    test_1 = 0x2,
    test_2 = 0x1
}Test;


int main() {
    enum Test my_enum = test_2;
    printf("Number is %d\n", my_enum);
    return 0;
}

    
