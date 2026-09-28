#include "recovery.h"
#include <stdint.h>
#include <stdio.h>

#define INDEX_OF_P_SYNDROME 0
#define INDEX_OF_Q_SYNDROME 1

#if 0
// return the index of P syndrome
size_t P_location(size_t stripe_number, size_t number_of_drives)
{
    return number_of_drives - (stripe_number % number_of_drives) - 1;
}
#endif 

int P_XOR( char ** sorted_blocks, int number_of_drives, int stripe_size, char * recovery_space )
{   
    if( ! recovery_space )
    {   
        fprintf(stderr, "no recovery_space memory");
        return -1;
    }
    //sorted_blocks == [P, Q, D, D]
    for( int offset = 0; offset < stripe_size; offset ++)
    {   
        if( sorted_blocks[INDEX_OF_P_SYNDROME] == NULL )
        {    
            recovery_space[offset] = 0;
        }
        else
        {
            recovery_space[offset] = sorted_blocks[INDEX_OF_P_SYNDROME][offset];
        }
        for( int drive = 2; drive < number_of_drives; drive ++ )
        {
            if( sorted_blocks[drive] == NULL )
            {
                continue;
            }   
            recovery_space[offset] ^= sorted_blocks[drive][offset];
        }
    }
    return 0;
}
#if 1
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

int Q_XOR( char ** sorted_blocks, int number_of_drives, int stripe_size, char * recovery_space )
{   
    // 2 failed drives - P & Data missing --- XOR Q with GF'ed existing data (then un-GF the output to get the actual data)
    // 2 failed drives - Data & Data missing --- GF hell :'(

    //The Q_XOR function looks very much like the P_XOR function, except sorted_blocks[drive][offset] needs to be GF'ed before the XOR.
    //And the recovery_space we get back is GF'ed, so we need to undo that.
    if( ! recovery_space )
    {   
        fprintf(stderr, "no recovery_space memory");
        return -1;
    }

    //sorted_blocks == [P, Q, D, D]
    for( int offset = 0; offset < stripe_size; offset ++)
    {   
        if( sorted_blocks[INDEX_OF_Q_SYNDROME] == NULL )
        {
            recovery_space[offset] = 0;
        }
        else
        {
            recovery_space[offset] = sorted_blocks[INDEX_OF_Q_SYNDROME][offset];
        }

        //sorted_blocks == [P, Q, D, D] 
        for( int drive = 2; drive < number_of_drives; drive ++ ) //drive starts at index 2 - first data drive
        {   //case - missing P drive and Data drive
            // GF(data1) XOR GF(data-N) XOR Q
            if( sorted_blocks[drive] == NULL )
            {
                continue;
            } 
            recovery_space[offset] ^= GF( drive - 1, sorted_blocks[drive][offset] );
        }
        //after XOR'ing all data drives, XOR with Q bytes for full recovery :0

    }
    return 0; 
}
#endif

