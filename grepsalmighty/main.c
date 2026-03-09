#include "block.h"
#include "block_memory.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define HERE fprintf(stderr, "Line %i\n in File %s\n" , __LINE__, __FILE__)

int main(int argc, char **argv)
{
    struct block * current_block; 
    size_t number_of_blocks = 1;
    block_t * blocks = malloc( number_of_blocks * sizeof(*blocks) );

    if (!blocks)
    {
        perror(argv[0]);
        exit(EXIT_FAILURE);
    }

    current_block = &blocks[number_of_blocks - 1];
    // create defualt values for blocks struct
    current_block->total_patterns = 0;
    // will need to malloc/realloc when size is known -- malloc(sizeof(blocks -> patterns));
    current_block->patterns = NULL;
    current_block->match_data = NULL;
    current_block->out_file_name = NULL;
    current_block->out_file = NULL;
    current_block->i = false;
    current_block->v = false;
    current_block->w = false;

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
            int error = add_pattern_to_block(current_block, optarg);
             
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
            current_block->i = true;
            break;

        case 'v':
            current_block->v = true;
            break;

        case 'w':
            current_block->w = true;
            break;

        case 'o':
            current_block->out_file_name = strdup(optarg);
            
            if (!current_block->out_file_name)
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
          
            block_t *temp_pointer = realloc(blocks, number_of_blocks * sizeof(*blocks));
            

            if( !temp_pointer)
            {   
                perror(argv[0]);
                //if temp_pointer failed then number_of_blocks is 1 too high so gets decremented
                //we do NOT want to free unknown sections of memeory
                number_of_blocks--;
                for (size_t block = 0; block < number_of_blocks; block++)
                {   
                    free_block_memory(&blocks[block]);
                }
                free(blocks);
                exit(EXIT_FAILURE);
            }
           
            blocks = temp_pointer;
            current_block = &blocks[number_of_blocks - 1];
            // create default values for last index of blocks struct
            current_block->total_patterns = 0;
            current_block->patterns = NULL;
            current_block->match_data = NULL; 
            current_block->out_file_name = NULL;
            current_block->out_file = NULL;
            current_block->i = false;
            current_block->v = false;
            current_block->w = false;
            break;
        }
    }
    
    if ( blocks[number_of_blocks - 1].patterns ) 
    {
       fprintf(stderr, "no out_file provided for last group, you suck, gerp hates you\n");
           for (size_t block = 0; block < number_of_blocks; block++)
                {
                    free_block_memory(&blocks[block]);
                }
                free(blocks);
                exit(EXIT_FAILURE);
    }

    number_of_blocks --;

    for(size_t file = 0; file < number_of_blocks; file ++)
    {   
        
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
            fprintf(stderr, "%s: errno = %i\n" , argv[ 0 ] , errno);
            break;
        }
        bool line_written = false;
        for(size_t block = 0; block < number_of_blocks; block ++ )
        {   
            if( ! line_written )
            {
                for(size_t pattern = 0; pattern < blocks[block].total_patterns; pattern ++ )
                {
                    pcre2_code * regex = blocks[block].patterns[pattern]; 

                    int rc = pcre2_match( regex, (PCRE2_SPTR) lineptr, line_length, 0, 0, blocks[block].match_data[pattern], NULL);
                    
                    if( rc > 0 )
                    {   
                        //if looking for inverse match continue to next pattern
                        if( blocks[block].v )                         {
                            continue;
                        }
                        //write matching lines to file
                        fwrite(lineptr, line_length, 1, blocks[block].out_file);
                        line_written = true;
                        break;
                    }
                    // write non-matching lines to file
                    if( rc == PCRE2_ERROR_NOMATCH )
                    {   
                        if( blocks[block].v )
                        {
                            fwrite(lineptr, line_length, 1, blocks[block].out_file);
                            line_written = true;
                            break;
                        }
                        continue;
                    }

                    fprintf(stderr, "%s: pcre2_match() returned %i in %s, line %i\n" , argv[0] , rc , __FILE__ , __LINE__ );
                    free(lineptr);
                    for (size_t block = 0; block < number_of_blocks; block++)
                    {
                        free_block_memory(&blocks[block]);
                    }
                    free(blocks);
                    exit(EXIT_FAILURE);
                }
            }
        }
        if( !line_written )
        {
            printf("%s", lineptr); 
        }
        free(lineptr);
    } while ( true );

    for (size_t block = 0; block < number_of_blocks; block++)
    {   
        free_block_memory(&blocks[block]);
    }
    free(blocks);
   
    exit(EXIT_SUCCESS);
}
