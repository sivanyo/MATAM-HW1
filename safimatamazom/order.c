//
// Created by safi azmi on 17/11/2019.
//

#include "order.h"
#include <stdio.h>
#include <stdlib.h>

/**---------------------------------------------------------------------------------**/
/**--------------copy,free,compare function for setamount---------------------------**/
/**---------------------------------------------------------------------------------**/
static ASElement copyInt(ASElement number) {
    unsigned int *copy = malloc(sizeof(*copy));
    if (copy != NULL) {
        *copy = *(unsigned int *)number;
    }
    return copy;
}

static void freeInt(ASElement number) { free(number); }

static int compareInts(ASElement lhs, ASElement rhs) {
    return (*(unsigned int *)lhs) - (*(unsigned int *)rhs);
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------Order struct ---------------------------------**/
/**----------------------------------------------------------------------------------**/
struct order_t{
    unsigned int id;
    AmountSet order_set;
};

/**----------------------------------------------------------------------------------**/
/**-----------------------------------createOrder---------------------------------**/
/**----------------------------------------------------------------------------------**/
Order createOrder(unsigned int id){
    if(id<0){
        return NULL;
    }
    Order new=malloc(sizeof(*new));
    if(!new){ return NULL;}
    new->id=id;
    new->order_set=asCreate(copyInt,freeInt,compareInts);
    if(!new->order_set){
        free(new);
        return NULL;
    }
    return new;
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------destroyOrder---------------------------------**/
/**----------------------------------------------------------------------------------**/

void destroyOrder(Order e){
    if(!e){ return;}
    asDestroy(e->order_set);
    free(e);
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------copyOrder---------------------------------**/
/**----------------------------------------------------------------------------------**/

Order copyOrder(Order e){
    if(!e){ return NULL;}
    Order copy=malloc(sizeof(*copy));
    if(!copy){
        return NULL;
    }
    copy->id=e->id;
    copy->order_set=asCopy(e->order_set);
    if(!copy->order_set){
        free(copy);
        return NULL;
    }
    return copy;
}

/**----------------------------------------------------------------------------------**/
/**-----------------------------------getOrderId---------------------------------**/
/**----------------------------------------------------------------------------------**/


unsigned int getOrderId(Order e){
    if(!e){ return 0;}
    return e->id;
}
/**----------------------------------------------------------------------------------**/
/**-----------------------------------getSetOrder---------------------------------**/
/**----------------------------------------------------------------------------------**/

AmountSet getSetOrder(Order e){
    if(!e){ return NULL;}
    return e->order_set;
}


/**----------------------------------------------------------------------------------**/
/**----------------------------------getProAmInOrder---------------------------------**/
/**----------------------------------------------------------------------------------**/

double getProAmInOrder(Order e, unsigned int id){
    AmountSet tmp=e->order_set;
    double tmp_amount=0;
    asGetAmount(tmp,&id,&tmp_amount);
    return tmp_amount;
}
