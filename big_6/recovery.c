#include "recovery.h"
#include "GF_lookup_tables.h"
#include <stdint.h>
#include <stdio.h>

#define INDEX_OF_P_SYNDROME 0
#define INDEX_OF_Q_SYNDROME 1

int P_XOR( uint8_t ** sorted_blocks, size_t number_of_drives, size_t stripe_size, uint8_t * recovery_space )
{   
    if( ! recovery_space )
    {   
        fprintf(stderr, "no recovery_space memory");
        return -1;
    }
    //sorted_blocks == [P, Q, D, D]
    for( size_t offset = 0; offset < stripe_size; offset ++)
    {   
        if( sorted_blocks[INDEX_OF_P_SYNDROME] == NULL )
        {    
            recovery_space[offset] = 0;
        }
        else
        {
            recovery_space[offset] = sorted_blocks[INDEX_OF_P_SYNDROME][offset];
        }
        for( size_t drive = 2; drive < number_of_drives; drive ++ )
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


int Q_XOR( uint8_t ** sorted_blocks, size_t number_of_drives, size_t stripe_size, uint8_t * recovery_space )
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
    for( size_t offset = 0; offset < stripe_size; offset ++)
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
        for( size_t drive = 2; drive < number_of_drives; drive ++ ) //drive starts at index 2 - first data drive
        {   //case - missing P drive and Data drive
            // GF(data1) XOR GF(data-N) XOR Q
            if( sorted_blocks[drive] == NULL )
            {
                continue;
            }

            recovery_space[offset] ^= GF_array[drive -1][ sorted_blocks[drive][offset]];
        }

    }
    return 0; 
}


