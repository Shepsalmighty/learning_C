#include "maths.h"
#include "base2.h"
#include "base4.h"
#include "base8.h"
#include "base16.h"
#include "base32.h"
#include "base64.h"
#include "generic_decode.h"
#include "generic_encode.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define BUFFER_SIZE 256
int main(int argc, char *argv[])
{
    uint8_t input_buffer[BUFFER_SIZE];  //[INPUT_BLOCK_SIZE * BLOCKS];
    uint8_t output_buffer[BUFFER_SIZE]; //[OUTPUT_BLOCK_SIZE * BLOCKS];
    ssize_t got;
    int bytes_size = 0;

    char *optstring = "-"
                      "e:" //encode
                      "d:" //decode
                      "";

    // getopt should return -1 --> opt == -1.... we'll use optind to find the input string to be worked on
    int opt; 
    intmax_t base_size = 0;
    bool decode = false;
    int bits_per_symbol = 0;
    const char * symbols;
    char padding;

    while ((opt = getopt(argc, argv, optstring)) != -1)
    {
        switch (opt)
        {
        case 'e':
        {
            char *endptr;
            decode = false;
            base_size = strtoimax(optarg, &endptr, 10);

            switch (base_size)
            {
            case 2:
                bits_per_symbol = BASE2_BITS_PER_SYMBOL;
                symbols = BASE2_SYMBOLS;
                padding = BASE2_PADDING;
                break;

            case 4:
                bits_per_symbol = BASE4_BITS_PER_SYMBOL;
                symbols = BASE4_SYMBOLS;
                padding = BASE4_PADDING;
                break;

            case 8:
                bits_per_symbol = BASE8_BITS_PER_SYMBOL;
                symbols = BASE8_SYMBOLS;
                padding = BASE8_PADDING;
                break;

            case 16:
                bits_per_symbol = BASE16_BITS_PER_SYMBOL;
                symbols = BASE16_SYMBOLS;
                padding = BASE16_PADDING;
                break;

            case 32:
                bits_per_symbol = BASE32_BITS_PER_SYMBOL;
                symbols = BASE32_SYMBOLS;
                padding = BASE32_PADDING;
                break;

            case 64:
                bits_per_symbol = BASE64_BITS_PER_SYMBOL;
                symbols = BASE64_SYMBOLS;
                padding = BASE64_PADDING;
                break;
            }
        }
        break;

        case 'd':
        {
            char *endptr;
            decode = true;
            base_size = strtoimax(optarg, &endptr, 10);
        
            switch (base_size)
            {
            case 2:
                bits_per_symbol = BASE2_BITS_PER_SYMBOL;
                symbols = BASE2_SYMBOLS;
                padding = BASE2_PADDING;
                break;

            case 4:
                bits_per_symbol = BASE4_BITS_PER_SYMBOL;
                symbols = BASE4_SYMBOLS;
                padding = BASE4_PADDING;
                break;

            case 8:
                bits_per_symbol = BASE8_BITS_PER_SYMBOL;
                symbols = BASE8_SYMBOLS;
                padding = BASE8_PADDING;
                break;

            case 16:
                bits_per_symbol = BASE16_BITS_PER_SYMBOL;
                symbols = BASE16_SYMBOLS;
                padding = BASE16_PADDING;
                break;

            case 32:
                bits_per_symbol = BASE32_BITS_PER_SYMBOL;
                symbols = BASE32_SYMBOLS;
                padding = BASE32_PADDING;
                break;

            case 64:
                bits_per_symbol = BASE64_BITS_PER_SYMBOL;
                symbols = BASE64_SYMBOLS;
                padding = BASE64_PADDING;
                break;
            }
        }
            break;
        }
    }
    if(bits_per_symbol == 0)
    {
        fprintf(stderr, "bits_per_symbol not set in main.c\n");
        exit(EXIT_FAILURE);
    }

   
   
    int bits_per_block = lcm(bits_per_symbol, 8);
	int symbols_per_block = bits_per_block / bits_per_symbol;
	int bytes_per_block = bits_per_block / 8;
    int maximum_buffer_usage = (BUFFER_SIZE / symbols_per_block) * bytes_per_block; 


    do
    {
        // read() returns number of bytes read
        got = fread(input_buffer, 1, maximum_buffer_usage, stdin); 
        if (got < 0)
        {
            perror("read()");
            exit(EXIT_FAILURE);
        }
        if (got == 0)
        {
            break;
        }
        if (decode)
        {
            bytes_size = generic_decode(input_buffer, got, output_buffer,
                                        bits_per_symbol, symbols, padding);
        }
        else
        {
            bytes_size = generic_encode(input_buffer, got, output_buffer,
                                        bits_per_symbol, symbols, padding);
        }
                
        fwrite(output_buffer, 1, bytes_size, stdout);

    } while (got > 0);
    if (!decode)
    {
        printf("\n");
    }

    exit(EXIT_SUCCESS);
}
