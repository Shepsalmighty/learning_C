#include "cost.h"

float box_cost( box customer_box, float rate )
{

    return (customer_box.depth * customer_box.height * customer_box.width) * rate;

}




