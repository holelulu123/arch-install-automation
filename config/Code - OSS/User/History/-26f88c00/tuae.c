#include <stdio.h>
#include <string.h>
#define STOP_CHAR ','
#define FINISH_CHAR '*'

typedef enum Mode : __uint8_t {
    InputMode       = 0x0,
    OutputMode      = 0x1,
    AlternateMode   = 0x2,
    AnalogMode      = 0x3,
    
}PinModes;


int main() {
    Test my_enum;
    printf("Number is %d\n", );
    // printf("Number is %p\n", &my_enum2);
    return 0;
}

    
