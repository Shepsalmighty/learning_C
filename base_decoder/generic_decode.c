#include "generic_decode.h"
#include "maths.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int generic_decode(uint8_t* input_buffer, int length, uint8_t* output_buffer,
                   int bits_per_symbol, const char* symbols, char padding) {

	if (!input_buffer || !output_buffer) {
		return EXIT_FAILURE;
	}
	int bits_per_block = lcm(bits_per_symbol, 8);
	int symbols_per_block = bits_per_block / bits_per_symbol;
	int bytes_per_block = bits_per_block / 8;
    int number_of_blocks = length / symbols_per_block; 
	int mask = ((1 << (bits_per_symbol)) - 1); // mask X LSB of a byte
	int bytes_decoded = 0;

	for (int block_number = 0; block_number < number_of_blocks; block_number++) 
    {
		uint64_t buffer = 0;
		int pad_count = 0;

		for (int input_count = 0; input_count < symbols_per_block; input_count++) 
        {
			buffer = buffer << bits_per_symbol;	
            uint8_t symbol = input_buffer[block_number * symbols_per_block + input_count];

			if (symbol == padding) 
            {
				pad_count++;
			} 
            else {

				char* found_symbol = strchr(symbols, symbol);

				if (!found_symbol) 
                {
					fprintf(stderr, "Invalid symbol %02X in input",symbol); //%02X show as hex in case "symbol" is
					                 //binary, so not printed to terminal
					exit(EXIT_FAILURE);
				}

				uint8_t value = found_symbol - symbols; // variable to store 6 bit chunks from Base32
				             // characters (3 bits will be unused)

				buffer = buffer | (value & mask);
			}
		}
		int bytes_to_copy = (bits_per_block - (pad_count * bits_per_symbol)) / 8;
		if (!bytes_to_copy) {

			fprintf(stderr, "invalid padding -> invalid base encoding input\n");
			exit(EXIT_FAILURE);
		}

		for (int output_count = (bytes_per_block - 1); output_count >= 0; output_count--)
        {
			if (bytes_to_copy > output_count) 
            {
				output_buffer[block_number * bytes_per_block + output_count] = buffer & 0xFF;
				bytes_decoded++;
			}
			buffer = buffer >> 8;
		}
	}

	return bytes_decoded;
}
