#include "generic_encode.h"
#include "maths.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>


int generic_encode(unsigned char * input_buffer, int length, unsigned char * output_buffer, 
                    int bits_per_symbol, const char * symbols, char padding){

    
    if( !input_buffer ||  !output_buffer)
    {
        return EXIT_FAILURE;
    }
    int bits_per_block = lcm(bits_per_symbol, 8); 
    int bytes_per_block = bits_per_block / 8; 
    int symbols_per_block = bits_per_block / bits_per_symbol; 
    int number_of_blocks = length / bytes_per_block;
    int mask = ((1<<(bits_per_symbol)) -1); // mask X LSB of a byte
    int bytes_encoded = 0;

     uint64_t buffer = 0;

    for(int block_number = 0; block_number < number_of_blocks; block_number ++) 
    {    
       
        for (int input_count = 0; input_count < bytes_per_block; input_count++) 
        {
            uint8_t input_byte = input_buffer[block_number * bytes_per_block + input_count];
            buffer = buffer << 8; // left shift by one byte
            buffer = buffer | input_byte;
         }
        for (int symbol_count = (symbols_per_block - 1); symbol_count >= 0; symbol_count--) {
            uint8_t value = (buffer >> (symbol_count * bits_per_symbol)) & mask; // mask masks the 6 LSB of a Byte
            uint8_t symbol;
            
            symbol = symbols[value];
        
            output_buffer[block_number * symbols_per_block + ((symbols_per_block - 1) - symbol_count)] = symbol;
            bytes_encoded ++;
        } 
    }
    
    if( length % bytes_per_block != 0 ) {
     
        for (int input_count = 0; input_count < bytes_per_block; input_count++) 
        {
            uint8_t input_byte = 0;
            if( (number_of_blocks * bytes_per_block + input_count) < length )
            {
                input_byte = input_buffer[number_of_blocks * bytes_per_block + input_count];
            }
            buffer = buffer << 8; // left shift by one byte
            buffer = buffer | input_byte;
             
        }

        for (int symbol_count = (symbols_per_block - 1); symbol_count >= 0; symbol_count--) 
        {
            uint8_t symbol = padding;
            if(((symbols_per_block - 1) - symbol_count) * bits_per_symbol <  (length % bytes_per_block) * 8)
            {
                uint8_t value = (buffer >> (symbol_count * bits_per_symbol)) & mask; // mask masks the 6 LSB of a Byte
                symbol = symbols[value];
               
            } 
            
            output_buffer[number_of_blocks * symbols_per_block + ((symbols_per_block - 1) - symbol_count)] = symbol;
            bytes_encoded ++;
        } 
   }
return bytes_encoded;
}












