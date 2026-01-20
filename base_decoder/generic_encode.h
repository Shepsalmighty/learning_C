#ifndef GENERIC_ENCODE_H
    #define GENERIC_ENCODE_H
    #include <stdint.h>
int generic_encode(uint8_t * input_buffer, int length, uint8_t * output_buffer, int bits_per_symbol, const char * symbols, char padding);

#endif // ! GENERIC_ENCODE_H
