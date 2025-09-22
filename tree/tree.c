#include <stdio.h>
#include <stdlib.h>

#define str char*
int main(int argc, str argv[])  // TODO add "int argc" if it fails to compile
{
    int base_size = (atoi(argv[1]) + 1) / 2;
    for (int i = 1; i <= base_size; i++)
        {
            for (int j = 1; j <= base_size - i; j++)
                {
                    printf("%s", " ");
                }
            for (int k = 1; k <= 2 * i - 1; k++)
                {
                    printf("%s", "*");
                }

            printf("\n");
        }
    for (int j = 1; j <= base_size - 1; j++)
        {
            printf("%s", " ");
        }
    printf("%s", "*");

    printf("\n");
}
