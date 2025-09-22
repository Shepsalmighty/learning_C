#include <stdio.h>
#include "add.h"
#define NICE 69
#define BLAZE 42

int main( void ) {
    int a = BLAZE;
    int b = NICE;
    printf("%i\n", add(a, b));
    return 0;
}

