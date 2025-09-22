#include <stdio.h>
//#include "box_struct.h"
#include "cost.h"

int main(int argc, char *argv[])
{

    box customer_box;


    printf("Enter box height in meters ");
    scanf("%f", &customer_box.height);

    printf("Enter box width in meters ");
    scanf("%f", &customer_box.width);

    printf("Enter box depth in meters ");
    scanf("%f", &customer_box.depth);

    float rate = 69.0;

    printf("\nYour box will cost $%f", box_cost(customer_box, rate));

}
