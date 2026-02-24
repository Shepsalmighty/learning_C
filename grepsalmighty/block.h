#ifndef BLOCK_H
    #define BLOCK_H
    #define PCRE2_CODE_UNIT_WIDTH 8
    #include <pcre2.h>
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdio.h>

    typedef struct block
    {
        size_t total_patterns;
        pcre2_code ** patterns;
        pcre2_match_data ** match_data;
        char * out_file_name;
        FILE * out_file;
        bool i;
        bool v;
        bool w;
    }
    block_t;

#endif // ! BLOCK_H

