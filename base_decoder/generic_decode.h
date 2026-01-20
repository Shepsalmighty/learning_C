#ifndef GENERIC_DECODE_H
    #define GENERIC_DECODE_H
    #include <stdint.h>
    int generic_decode(uint8_t *input_buffer, int length, uint8_t *output_buffer, int bits_per_symbol, const char *symbols, char padding);

#endif // ! GENERIC_DECODE_H
