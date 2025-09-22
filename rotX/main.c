#include <errno.h>
// #include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROTATE 13
#define ALPHABET_SIZE 26

int main([[maybe_unused]]int argc, char* argv[])
{
    //(void)(argc);
        do
        {
            errno = 0;
            int letter = getchar();
            if (letter == EOF) // program ends or we encountered an error 
                {
                    if (errno != 0) //error encountered 
                        {
                            perror(argv[0]);
                            exit(EXIT_FAILURE);
                        }
                    break;
                }
            if ('a' <= letter && letter <= 'z')
                {
                    letter = (((letter - 'a') + ROTATE) % ALPHABET_SIZE) + 'a';
                }
            else if ('A' <= letter && letter <= 'Z')
                {
                    letter = (((letter - 'A') + ROTATE) % ALPHABET_SIZE) + 'A';
                }
            putchar(letter);
        }
    while (true);

    return (EXIT_SUCCESS);
}
