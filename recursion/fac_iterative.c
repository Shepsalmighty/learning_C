#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int num = 1;

    char* endptr;

    if (argc != 2)
        {
            puts("Error: incorrect number of arguments, 1 required");
            return EXIT_FAILURE;
        }

    long range = strtol(argv[1], &endptr, 10);

    for (int i = 0; i < range; i++)
        {
            num *= i + 1;
            printf("%d\n", num);
        }

    printf("%d", num);
    return (EXIT_SUCCESS);
}
