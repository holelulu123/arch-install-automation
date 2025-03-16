#include <stdio.h>

int main() {
    unsigned int result = (0x1 << 0);
    printf("Result is 0x%X\n", result);
    printf("Size: %d\n", sizeof(result));
    return 0;
}
