#ifndef BASE8_H
    #define BASE8_H
    
    constexpr int BASE8_BITS_PER_SYMBOL = 3;
    constexpr char BASE8_SYMBOLS[] = "QWERTYUI";
    constexpr char BASE8_PADDING = '=';

#endif // ! BASE8_H
       //
       //you're tyring to find n such that 2^n = M (the base e.g. 64, 32, 16)
       // It's base(2^n)
       // 2^n = 8
       // nlog(2)=log(8)
       // n=log(8)/log(2)
       // n=3.
