#include <stdio.h>
#include <string.h>
#define STOP_CHAR ','
#define FINISH_CHAR '*'

typedef enum {
    test_1,
    test_2,
}Test;


int main() {
    Test my_enum;
    printf("Number is %p\n", &test_1);
    // printf("Number is %p\n", &my_enum2);
    return 0;
}

    
