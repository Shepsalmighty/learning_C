#include "recovery.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>

//don't put the fuggin ; in or it'll fuck up the if statements
#define VERBOSE_DATA_BLOCKS 1
#define VERBOSE_STRIPE_NUMBER 2
#define VERBOSE_STRIPE_SIZE 3
#define VERBOSE_HEXDUMP 4
#define P_LOCATION( stripe_number , number_of_drives ) ( ( number_of_drives ) - ( ( stripe_number ) % ( number_of_drives ) ) - 1 )

// stripe 0 = data on sdb and sdc (in order). P on sdd, Q on sda
// stripe 1 = data on sda and sdb (in order). P syndrome on sdc and Q on sdd 1
// stripe 2 = data on sdd and sda (in order). P on sdb and Q on sdc
// stripe 3 = data on c and d (in order). P on sda and Q on sdb
// repeat

// stderrs RAIDS are always going left - can hard code that part
// in stripe 0, P is always the last drive e.g. stripe 0 = [[Q, D1, D2, P]],
// stripe 1 = [[D1, D2, P, Q]] etc WRAPPING AROUND

#if 0
// return the index of P syndrome
size_t P_location(size_t stripe_number, size_t number_of_drives)
{
    return number_of_drives - (stripe_number % number_of_drives) - 1;
}

int P_XOR( char ** sorted_blocks, int number_of_drives, int stripe_size, char * recovery_space )
{   
    if( ! recovery_space )
    {   
        fprintf(stderr, "no recovery_space memory");
        return -1;
    }
    
    int count = 0;
    //sorted_blocks == [P, Q, D, D]
    for( int offset = 0; offset < stripe_size; offset ++)
    {   
        recovery_space[offset] = 0;
        
        for( int drive = 0; drive < number_of_drives; drive ++ )
        {
            if( drive == 1 || sorted_blocks[drive] == NULL )
            {
                continue;
            }   
            recovery_space[offset] ^= sorted_blocks[drive][offset];
            count ++;
        }
    }
    return 0;
}

int Q_XOR( char ** sorted_blocks, int number_of_drives, int stripe_size, char * recovery_space )
{
    //The Q_XOR function looks very much like the P_XOR function, except sorted_blocks[drive][offset] needs to be GF'ed before the XOR.
    //And the recovery_space we get back is GF'ed, so we need to undo that.
}
//inside recovery()
    my_var = P_XOR(example)>
    if( my_var < 0 )
        fprintf something went wrong
        fclose
        free
    recovery_space[index] = my_var
#endif

#if 1
int recovery( char ** sorted_blocks, int number_of_drives, int stripe_size, char ** recovery_space )
{   
    // this function will do both P (XOR) recovery from N-data blocks AND Galois Field stuff
    // if one of sorted_blocks[2] to sorted_blocks[N] is NULL = XOR
    // if two indexes are NULL GF recovery
    // 1 failed drive - P --- nothing 
    // 1 failed drive - Q --- nothing  
    // 1 failed drive - Data --- P XOR existing_Data XOR existing_Data etc
    // 2 failed drives - P & Q --- nothing
    // 2 failed drives - P & Data --- XOR Q with GF'ed existing data (then un-GF the output to get the actual data)
    // 2 failed drives - Q & Data --- P XOR existing data (Q doesn't matter)
    // 2 failed drives - Data & Data --- GF hell :'(
    // 3 failed drives - cry
    int number_of_failed_drives = 0; 
    int failed_drives[2];
    for( int index = 0; index < number_of_drives; index ++)
    {
        if( sorted_blocks[index] == NULL )
        {   
            failed_drives[number_of_failed_drives] = index;
            number_of_failed_drives ++;
            if( number_of_failed_drives > 2 )
            {   
                fprintf(stderr, "AAAAH");
                return 1;
            }
        }
    }
    if( number_of_failed_drives == 0 )
    {   
        return 0;
    }
    if( number_of_failed_drives == 1)
    {   
        if( failed_drives[0] >= 2 ) //we have 1 failed drive and it's index is 2 or more == Data missing
        {
            return P_XOR( sorted_blocks, number_of_drives, stripe_size, recovery_space[0] );
        }
        return 0; // only P or Q missing - no data to recover
    }
    if( number_of_failed_drives == 2)
    {
        if( failed_drives[0] == 0 && failed_drives[1] == 1 )
        {
            return 0;
        }
        if( failed_drives[0] == 1 ) // 2 failed drives, 1st is Q the other must be data
        {
            return P_XOR( sorted_blocks, number_of_drives, stripe_size, recovery_space[0] );
        } 
        return 69; // 2 drives missing which aren't Q and a data
    }
    return 1;
}
#endif

int main( int argc, char **argv )
{
    int verbosity_level = 1;
    struct option const longopts[] = {
                                     {"stripe-size", required_argument, NULL, 's'},
                                     {"quiet", no_argument , NULL, 'q'},
                                     {"verbose", no_argument, NULL, 'v'},
                                     {NULL, no_argument, NULL, '\0'},
                                     };

    char * optstring =  "s:"
                        "q"
                        "v"
                        "";

    size_t stripe_size = 16;
    int option;

    while ( (option = getopt_long( argc, argv, optstring, longopts, NULL)) != -1)
    {
        switch (option)
        {
            case 's':
            {
                char *endptr;
                intmax_t temp_var = strtoimax(optarg, &endptr, 10);
                
                //check expected range of stripe size - guessing at 1 to 256 bytes
                if( temp_var < 1 || temp_var > 256)
                {
                    fprintf(stderr, "aaaaaah\n");
                    exit(EXIT_FAILURE);
                }
                stripe_size = temp_var;
            }
            break;

            case 'v':
            {
                verbosity_level ++;
            }
            break;

            case 'q':
            {
                verbosity_level = 0;
            }
            break;

            default:
            { 
                fprintf( stderr, "%s: Missing code for -%c in %s, near line %i\n",
                        argv[0] , option , __FILE__ , __LINE__ ); 
                exit( EXIT_FAILURE );
            }
            break;

            case '?':
            {   
                fprintf(stderr, "No help is coming, figure it out on your own\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
    }
    if( verbosity_level >= VERBOSE_STRIPE_SIZE )
    {
        printf("stripe size = %zu\n", stripe_size);
    }
    size_t number_of_drives = argc - optind; 

    if ( number_of_drives < 3 )
    {
        fprintf(stderr, "Either not enough args, or your RAID is dumb and you "
                        "deserve what you got\n");
        exit(EXIT_FAILURE);
    }

    FILE ** drives = malloc( number_of_drives * sizeof(*drives) );
    if( !drives )
    {
        perror(argv[0]);
        exit(EXIT_FAILURE);
    }

    char ** blocks_from_drive = malloc( number_of_drives * sizeof( * blocks_from_drive) );
    if( ! blocks_from_drive )
    {
        perror(argv[0]);
        free(drives);
        exit(EXIT_FAILURE);
    }

    for( size_t drive = 0; drive < number_of_drives; drive ++ )
    {
        char * drive_name = argv[optind + drive];
        drives[drive] = fopen(drive_name, "rb");

        if( ! drives[drive] )
        {
            perror( drive_name );
            while( drive > 0 )
            {
                drive --;
                free(blocks_from_drive[drive]);
                fclose(drives[drive]);
            }
            free(blocks_from_drive);
            free(drives);
            exit(EXIT_FAILURE);
        }
        blocks_from_drive[drive] = malloc( stripe_size );
        if( ! blocks_from_drive[drive] )
        {
            perror( drive_name );
            fclose(drives[drive]);
            while( drive > 0 )
            {
                drive --;
                free(blocks_from_drive[drive]);
                fclose(drives[drive]);
            }
            free(blocks_from_drive);
            free(drives);
            exit(EXIT_FAILURE);
        }
    }

    char ** sorted_blocks = malloc( number_of_drives * sizeof ( * sorted_blocks ) );  // data is stored thusly sorted_blocks[P,Q,D1,D2,D-N]
    if( ! sorted_blocks )
    {
        perror(argv[0]);
        for( size_t drive = 0; drive < number_of_drives; drive ++ )
        {   
            free( blocks_from_drive[drive] );
            fclose( drives[drive] );
        }
        free(blocks_from_drive);
        free(drives);
        exit(EXIT_FAILURE);
    }

    char * recovery_space[2]; 

    recovery_space[0] = malloc( stripe_size );

    if( !recovery_space[0] )
    {
        perror(argv[0]);
        free(sorted_blocks);
        for( size_t drive = 0; drive < number_of_drives; drive ++ )
        {   
            free( blocks_from_drive[drive] );
            fclose( drives[drive] );
        }
        free(blocks_from_drive);
        free(drives);
        exit(EXIT_FAILURE);
    }
    recovery_space[1] = malloc( stripe_size );

    if( !recovery_space[1] )
    {
        perror(argv[0]);
        free(recovery_space[0]);
        free(sorted_blocks);
        for( size_t drive = 0; drive < number_of_drives; drive ++ )
        {   
            free( blocks_from_drive[drive] );
            fclose( drives[drive] );
        }
        free(blocks_from_drive);
        free(drives);
        exit(EXIT_FAILURE);
    }
    
    //read data from drives
    size_t stripe_number = 0; 

    do
    {   
    // stderrs RAIDS are always going left - can hard code that part
    // in stripe 0, P is always the last drive e.g. stripe 0 = [[Q, D1, D2, P]],
    // stripe 1 = [[D1, D2, P, Q]] etc WRAPPING AROUND
        int errors = 0;
        if( verbosity_level >= VERBOSE_STRIPE_NUMBER )
        {
            printf("stripe number %zu\n", stripe_number);
        }
        size_t P = P_LOCATION(stripe_number, number_of_drives);
        
        //read from drives[P] to blocks_from_drive[P] 
        size_t result = fread(blocks_from_drive[P], 1, stripe_size, drives[P]);
        if( result != stripe_size )
        {
            sorted_blocks[0] = NULL;
            errors ++;
            fprintf(stderr, "failed to read from drive# %zu\n", P);
        }
        else
        {
            sorted_blocks[0] =  blocks_from_drive[P];
        }

        size_t Q = (P + 1) % number_of_drives;
        //read from drives[Q] to blocks_from_drive[Q]
        result = fread(blocks_from_drive[Q], 1, stripe_size, drives[Q]);
        if( result != stripe_size )
        {
            sorted_blocks[1] = NULL;
            errors ++;
            fprintf(stderr, "failed to read from drive# %zu\n", Q);

        }
        else
        {
            sorted_blocks[1] = blocks_from_drive[Q];
        }

        //for loop for remaining data blocks
        for( size_t index = 2; index < number_of_drives; index ++)
        {   
            size_t data_block_index = (P + index) % number_of_drives;
            
            result = fread(blocks_from_drive[data_block_index], 1, stripe_size, drives[data_block_index]);
            if( result != stripe_size )
            {
                sorted_blocks[index] = NULL;
                errors ++;
                fprintf(stderr, "failed to read from drive# %zu\n", data_block_index);
            }
            else
            {
                sorted_blocks[index] = blocks_from_drive[data_block_index];
            }
        }
        if( errors == 1 )
        {
            int success_is_a_zero = P_XOR(sorted_blocks, number_of_drives, stripe_size, recovery_space[0]);
        }
        //we can still recover data with 2 failed drives, if we reach 3 we're done
        if( errors > 2 )
        {   
            break;
        }
#if 0        
        if( errors )
        {  
            CALL RECOVERY()!!!!!!!! not P_XOR 

            int success_is_a_zero = P_XOR(sorted_blocks, number_of_drives, stripe_size, recovery_space[0]);
            if recovery is successful, then write to the known missing data block NOT to sorted_blocks[index]
            sorted_blocks[index] is NULL

            stderr - We need to write it to the block for the missing drive and set sorted_block[ index ] 
            to point to that block (as if we had read it from disk).
            blocks_from_drive[ missing_drive ] 
            is the buffer.
            

        }
#endif
        for( size_t index = 2; index < number_of_drives; index ++)
        {   
            if( sorted_blocks[index] == NULL )
            {   
                printf( "Missing Data \n" );
                continue;
            }

            fwrite( sorted_blocks[index], 1, stripe_size, stdout );
        }
        
        stripe_number ++; 
      
    } while( true ); 



    //clean up and exit
    free(recovery_space[0]);
    free(recovery_space[1]);
    free(sorted_blocks);
    for( size_t drive = 0; drive < number_of_drives; drive ++ )
    {   
        // WE DONT free(sorted_blocks[index]) because those pointers are THE SAME pointers 
        // as in blocks_from_drive[index]
        free( blocks_from_drive[drive] );
        fclose( drives[drive] );
    }
    free( blocks_from_drive );
    free( drives );
    exit(EXIT_SUCCESS);
}
