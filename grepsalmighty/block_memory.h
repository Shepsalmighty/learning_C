#ifndef BLOCK_MEMORY_H
    #define BLOCK_MEMORY_H
    #include "block.h"
    #include <stddef.h>
    size_t add_pattern_to_block( block_t * block, char *pattern );
    void free_block_memory( block_t * block );
#endif // ! BLOCK_MEMORY_H
