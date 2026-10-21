#ifndef RECOVERY_H
    #define RECOVERY_H
    #include<stdint.h>
    #include<stddef.h>
        int P_XOR( uint8_t ** sorted_blocks, size_t number_of_drives, size_t stripe_size, uint8_t * recovery_space );
        int Q_XOR( uint8_t ** sorted_blocks, size_t number_of_drives, size_t stripe_size, uint8_t * recovery_space );

#endif // ! RECOVERY_H

