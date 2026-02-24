//#define PCRE2_CODE_UNIT_WIDTH 8
//#include <pcre2.h>
#include "block.h"
#include "block_memory.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// stderr - I would suggest a struct of "number of patterns" and a pointer to an
// array of those patterns.

int main(int argc, char **argv)
{
    size_t number_of_blocks = 0;
    block_t * blocks = malloc( (number_of_blocks + 1) * sizeof(*blocks) );

    if (!blocks)
    {
        perror(argv[0]);
        exit(EXIT_FAILURE);
    }
    // create defualt values for blocks struct
    blocks[number_of_blocks].total_patterns = 0;
    // will need to malloc/realloc when size is known -- malloc(sizeof(blocks ->
    // patterns));
    blocks[number_of_blocks].patterns = NULL;
    blocks[number_of_blocks].match_data = NULL;
    //.out_file_name dependant on infile type etc
    blocks[number_of_blocks].out_file_name = NULL;
    blocks[number_of_blocks].out_file = NULL;
    blocks[number_of_blocks].i = false;
    blocks[number_of_blocks].v = false;
    blocks[number_of_blocks].w = false;

    char *optstring =
        "e:" // search pattern
        "i"  // ignore case
        "o:" // write to this file
        "v"  // select non matching lines
        "w"  // word regex -- exact word match (e.g. mat in match does not work)
        "";

    int opt;
    while ((opt = getopt(argc, argv, optstring)) != -1)
    {
        switch (opt)
        {
        case 'e':
            // error should be ZERO if function returned successfully
            // 1 compile reg exp pattern
            // 2 give regexp instead of optarg to add_pattern_to_block
            // 1 and 2 now being done in add_pattern_to_block
            int error = add_pattern_to_block(&blocks[number_of_blocks], optarg);
            
             
            // if error is truthy function failed -> free memory
            if (error)
            {
                perror(argv[0]);
                // free each pattern, free the lists of patterns, and then free
                // the blocks. and the list of blocks.
                for (size_t block = 0; block < number_of_blocks; block++)
                {
                    free_block_memory(&blocks[block]);
                }
                // free the inital memory allocated for all of our blocks
                free(blocks);
                exit(EXIT_FAILURE);
            }
            
            break;

        case 'i':
            blocks[number_of_blocks].i = true;
            break;

        case 'v':
            blocks[number_of_blocks].v = true;
            break;

        case 'w':
            blocks[number_of_blocks].w = true;
            break;

        case 'o':
            blocks[number_of_blocks].out_file_name = strdup(optarg);
            
            if (!blocks[number_of_blocks].out_file_name)
            {
                perror(argv[0]);
                for (size_t block = 0; block < number_of_blocks; block++)
                {
                    free_block_memory(&blocks[block]);
                }
                free(blocks);
                exit(EXIT_FAILURE);
            }
            // allocate memory for one more block in case additional
            // file(s)/pattern(s) provided
            number_of_blocks++;
            printf("Line%i\n" , __LINE__);
            block_t *temp_pointer = realloc(blocks, (number_of_blocks + 1) * sizeof(*blocks));
            printf("Line%i\n" , __LINE__);
           
            if( !temp_pointer)
            {   
                perror(argv[0]);
                //if temp_pointer failed then number_of_blocks is 1 too high so gets decremented
                //we do NOT want to free unknown sections of memeory
               // number_of_blocks--;
                for (size_t block = 0; block < number_of_blocks; block++)
                {
                    free_block_memory(&blocks[block]);
                }
                free(blocks);
                exit(EXIT_FAILURE);
            }
            blocks = temp_pointer;
            // create default values for last index of blocks struct
            blocks[number_of_blocks].total_patterns = 0;
            blocks[number_of_blocks].patterns = NULL;
            blocks[number_of_blocks].match_data = NULL;
            blocks[number_of_blocks].out_file_name = NULL;
            blocks[number_of_blocks].out_file = NULL;
            blocks[number_of_blocks].i = false;
            blocks[number_of_blocks].v = false;
            blocks[number_of_blocks].w = false;
            break;
        }
    }
    printf("something something whatever\n");
    fflush(stdout);


    for(size_t file = 0; file < number_of_blocks; file ++)
    {   
        //printf("got to %s, line %i\n file = %zu\n", __FILE__, __LINE__, file);
        //fflush(stdout);
        blocks[file].out_file = fopen(blocks[file].out_file_name, "w");
        if( ! blocks[file].out_file)
        {
           perror(argv[0]);
           for (size_t block = 0; block < number_of_blocks; block++)
                {
                    free_block_memory(&blocks[block]);
                }
                free(blocks);
                exit(EXIT_FAILURE);
        }

    }

#if 0
    //You need to read lines from stdin.
    //For each line check if it matches any of the patterns.
    //If it does, write it to the correct file.
    //If it doesn't match any of the patterns, write it to stdout.
    //Loop over all lines.
  
    //read stdin for patterns, if exists write to out_file else print to stdout
    do
    {
        char * lineptr = NULL;
        size_t buffer_size = 0;
        ssize_t line_length = getline(&lineptr, &buffer_size, stdin);
       

        //getline returns -1 on error or end of file
        if ( line_length < 0 )
        {   
            if( ferror(stdin) )
            {   
                perror(argv[0]);
                free(lineptr);
                break;
            }
            if ( feof(stdin) ) 
            {   
                free(lineptr);
                break;
                
            } 
            fprint(stderr, "%s: errno = %i\n" , argv[ 0 ] , errno);
            break;
        }
        printf( "n\t%zu\nlineptr\t\"%s\"\n" , buffer_size , lineptr );

        
       //TODO - READ 140-144
       //lineptr points to a line, check if a pattern exists in that line
        free(lineptr);
    } while ( true );
#endif
    //Note to self: Remember to check if the last block have an -o or not.
#if 1
    for (size_t block = 0; block < number_of_blocks; block++)
    {   
        printf("THIS IS THE BLOCK\t%zu\n", block);
        fflush(stdout);

        free_block_memory(&blocks[block]);

        printf("do we crash here?\n");
        fflush(stdout);
    }
    #endif

    printf("before final free\n");
    fflush(stdout);
    free(blocks);
    printf("final free works\n");
    fflush(stdout);
    exit(EXIT_SUCCESS);
}
