#include "GF.h"

uint8_t GF( size_t drive_number, uint8_t byte )
{  
    // if result is greater than 255 (0xFF) it over flows and must be XOR'd with 285 (0x11D)

    uint16_t result = 0;

    if( drive_number >= 2 )
    {
        result = GF( drive_number / 2, byte ) * 2;  
        if( result > 0xFF )
        {
            result ^= 0x11D;
        }
    }
    if( drive_number % 2 == 1 )
    {
        result ^= byte;
    }
    return( (uint8_t) result );
}


