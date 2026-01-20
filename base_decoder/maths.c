#include "maths.h"
#include <stdint.h>

uint8_t gcd(uint8_t a, uint8_t b)
{
    if (a > b)
        {
            uint8_t c = b;
            b = a;
            a = c;
        }

    while (a != 0)
        {
            uint8_t temp = a;
            a = b % a;
            b = temp;
        }

    return b;
}

uint8_t lcm(uint8_t a, uint8_t b)
{

    if (a == 0 && b == 0)
        {
            return 0;
        }
    return ((a / gcd(a, b)) * b);
}

/* if a > b swamp them
 * while a != 0
 *          temp = a
 *          a = b % a
 *          b = temp
 *
 * return b */
