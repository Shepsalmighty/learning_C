#include <stdio.h>
#include <stdlib.h>

int faculty(int num)
{
    if(num <= 1)
    {
        return 1;
    }
    return faculty(num - 1) * num;

}

int main(int argc, char * argv[])
{
    if (argc != 2)
        {
            puts("Error: takes 1 arg ONLY");
            return EXIT_FAILURE;
        }
    char * endptr;
    long num = strtol(argv[1], &endptr, 10);

    printf("%d", faculty(num));
    return EXIT_SUCCESS;
}
