#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


int main( void )
{   
    int random_num = 69;

    int count = 0;

    int guess = 0;

    do{  

        char * input = readline( "pick a number, any number\n " );
        count ++;

        guess = atoi( input );
        
        if( guess == random_num )
            {
                printf( "omg good guess, you did it how could you guess the funny number in only %d guesses \n", count); 
                return (EXIT_SUCCESS);

            }

        else if( guess < random_num )
            { 
                puts( "lol no, think bigger... idiot " );
            }
        else{
                puts( "wooaah there big guy, slow it down, think a lil smaller\n" );
             }
    }while( guess != random_num );  
}

