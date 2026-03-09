#include "block_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t add_pattern_to_block(block_t *block, char *pattern)
{
    (void)(pattern);
    block->total_patterns++;

    pcre2_code **temp_regex_pointer = realloc(
        block->patterns, block->total_patterns * sizeof(*temp_regex_pointer));

    if (!temp_regex_pointer)
    {
        perror(__FUNCTION__);
        block->total_patterns--;
        return (__LINE__);
    }

    block->patterns = temp_regex_pointer;
    // block->patterns[block->total_patterns - 1] = strdup(pattern);

    int error_number = 420;
    PCRE2_SIZE error_offset = 69;

    block->patterns[block->total_patterns - 1] = pcre2_compile((PCRE2_SPTR)pattern, PCRE2_ZERO_TERMINATED, 0,
                                                                &error_number, &error_offset, NULL);
    /*
     * error_offset - the offset in characters in the pattern where pcre2 thinks
     there is an error
     * error_number is pcre2 libs error code for what is wrong
    */

    if (!block->patterns[block->total_patterns - 1])
    {
        char myarray[256];
        int error_len = pcre2_get_error_message(error_number, (PCRE2_UCHAR8 *) myarray,
                                        sizeof(myarray));
        (void) (error_len);
        fprintf(stderr, "pcre2 detected an error: %s --- @ index: %zu\n",
                                                    myarray,
                                                    error_offset);
        block->total_patterns--;
        return (__LINE__);
    }

    pcre2_match_data **temp_pointer = realloc(
        block->match_data, block->total_patterns * sizeof(*temp_pointer));
    if (!temp_pointer)
    {
        perror(__FUNCTION__);
        pcre2_code_free(block->patterns[block->total_patterns - 1]);
        block->total_patterns--;
        return (__LINE__);
    }

    block->match_data = temp_pointer;
    block->match_data[block->total_patterns - 1] =
        pcre2_match_data_create_from_pattern(
            block->patterns[block->total_patterns - 1], NULL);

    if (!block->match_data[block->total_patterns -
                           1]) // not enough memory for pattern
    {
        perror(__FUNCTION__);
        pcre2_code_free(block->patterns[block->total_patterns - 1]);
        block->total_patterns--;
        return (__LINE__);
    }

    // no errors
    return (0);
}

void free_block_memory(block_t *block)
{
    if (!block)
    {
        return;
    }

    for (size_t memory = 0; memory < block->total_patterns; memory++)
    {
        // printf( "pattern\t\"%s\"\n" , block->patterns[ memory ] );
        pcre2_match_data_free(block->match_data[memory]);
        pcre2_code_free(block->patterns[memory]);
        // free(block->patterns[memory]);
    }
    free(block->patterns);
    free(block->match_data);
    free(block->out_file_name);
    // files opened need to be closed not free'd (you're welcome. Past you. kisses xx)
    printf("file\t%p\n", block->out_file);
    if (block->out_file)
    {
        fclose(block->out_file);
    }
    printf("block\t%p\n", block);
    // free(block);
}
