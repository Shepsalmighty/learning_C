#ifndef COST_H
#define COST_H

typedef struct box
    {
        float height; 
        float width;
        float depth;
    } box; 

float box_cost(box customer_box, float rate);

#endif 

