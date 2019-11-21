//
// Created by Mor on 21/11/2019.
//

#include <stdlib.h>
#include "matamazom.h"
#include "amount_set.h"

/** Static functions to handle ids inside product linked list */
static ASElement copyInt(ASElement number) {
    int *copy = malloc(sizeof(*copy));
    if (copy != NULL) {
        *copy = *(int *)number;
    }
    return copy;
}

static void freeInt(ASElement number) { free(number); }

static int compareInts(ASElement lhs, ASElement rhs) {
    return (*(int *)lhs) - (*(int *)rhs);
}

typedef struct product_t {
    char *name;
    unsigned int id;
    MtmProductData product;
    MtmCopyData copy;
    MtmFreeData free;
    MtmGetProductPrice price;
    double income;
    double amount;
    MatamazomAmountType amountType;
    struct product_t *next;
} *ProductNode;

typedef struct order_t {
    unsigned int id;
    AmountSet orderProducts;
    struct order_t *next;
} *OrderNode;

struct Matamazom_t {
    ProductNode productsHead;
    ProductNode productsCurrent;
    OrderNode ordersHead;
    OrderNode ordersCurrent;
};

// TODO: sivan :) :-) :] :-] :D
Matamazom matamazomCreate() {
    // This function will allocate memory for a new Matamazon object and return it to the user.
    struct Matamzom_t new = malloc(sizeof(struct Matamazom_t));
    struct product_t *newProduct = malloc(sizeof(struct product_t));
    struct order_t *newOrder = malloc(sizeof(struct order_t));
    if (new == NULL || newProduct == NULL || newOrder == NULL) {
        return NULL;
    }
    new->productHead->name =NULL;
    new->productCurrent = NULL; //should i put something in the fields?
    new->orderHead = NULL;
    new->orderCurrent = NULL;
    return new;
}


void matamazomDestroy(Matamazom matamazom) {
    // This function will iterate over the linked lists for products and orders and free them
    // and then do the same for the matamazom object
}

// TODO: sivan :) :-) :] :-] :D
MatamazomResult mtmNewProduct(Matamazom matamazom, const unsigned int id, const char *name,
                              const double amount, const MatamazomAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice) {
    // This function will create a new productNode and add it to the products linked list.
    if (matamazom == NULL || copyData == NULL || freeData ==NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    Matamazom newProduct = malloc(sizeof(Matamazom);
    if (newProduct == NULL) {
        return NULL;
    }
    newProduct->productsCurrent->name = *name;
    newProduct->productsCurrent->id= id;
    newProduct->productsCurrent->product= customData;
    newProduct->productsCurrent->amount = amount;
    newProduct->productsCurrent->amountType = amountType;
    newProduct->productsCurrent->copy = copyData;
    newProduct->productsCurrent->free = freeData;
    newProduct->productsCurrent->price = prodPrice;
    newProduct->productsCurrent->next=NULL;

    //how to continue???

    return newProduct;

}

MatamazomResult mtmChangeProductAmount(Matamazom matamazom, const unsigned int id, const double amount) {
    // This function will find the product in the linked list and upate it's amount accordingly.
}

MatamazomResult mtmClearProduct(Matamazom matamazom, const unsigned int id) {
    // This function will remove a productNode from the linked list
    // and also remove the product from all active orders which include it.
}

// TODO: sivan :) :-) :] :-] :D
unsigned int mtmCreateNewOrder(Matamazom matamazom) {
    if(matamazom ==NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    Matamazom newOrder = malloc(sizeof(Matamazom));
    if (newOrder = =NULL){
        return NULL;
    }
    newOrder->ordersCurrent->next =NULL;
    newOrder->ordersCurrent->id= matamazom->ordersCurrent->id;
    if(  newOrder->ordersCurrent->id < 0 ){
        return 0;
    }
    return newOrder->ordersCurrent->id ;

}

// TODO: sivan :) :-) :] :-] :D
MatamazomResult mtmChangeProductAmountInOrder(Matamazom matamazom, const unsigned int orderId,
                                              const unsigned int productId, const double amount) {
    // This function will change the amount of a produt in the order,
    // if the product doesn't exist, a correct error code will be returned,
    // if the amount of the product decreases to 0 or less, the product will be removed
    // from the order.
}

MatamazomResult mtmShipOrder(Matamazom matamazom, const unsigned int orderId) {
    // This function will check that the inventory in the products linked list
    // is able to ship the required order, will update the income and the inventory
    // for each product and then delete the order
}

// TODO: sivan :) :-) :] :-] :D
MatamazomResult mtmCancelOrder(Matamazom matamazom, const unsigned int orderId) {
    if ( matamazom == NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    //we need to replace the head of the list
    if (matamazom->ordersHead->id = orderId){
        matamazom->ordersCurrent= matamazom->ordersHead->next;
        free(matamazom->ordersHead);
        matamazom->ordersHead = matamazom->ordersHead;
    }
    matamazom->ordersCurrent= matamazom->ordersHead->next;
    while (matamazom->ordersCurrent->id != orderId){
        if (matamazom->ordersCurrent->next = NULL) {
            return MATAMAZOM_ORDER_NOT_EXIST;
        }
        matamazom->ordersCurrent=matamazom->ordersCurrent->next;
    }
    // dont understand of I need to free the struct..

}

MatamazomResult mtmPrintInventory(Matamazom matamazom, FILE *output) {
    // This fucntion will recieve a mtmzon and ouput file and will use the
    // provided print function to print the contents of the products inventory
    // to the file.
}

MatamazomResult mtmPrintOrder(Matamazom matamazom, const unsigned int orderId, FILE *output) {
    // This function will receive a mtmzon and output file and will use
    // the built in print functions to print the contents of the orders
    // list to the file
}

MatamazomResult mtmPrintBestSelling(Matamazom matamazom, FILE *output) {
    // This function will receive a mtmzon and output file and will use
    // the built in print function to print the best selling product.
}

MatamazomResult mtmPrintFiltered(Matamazom matamazom, MtmFilterProduct customFilter, FILE *output) {
    // This function will receive a mtmzon and output file and will use
    // the provided filter function to filter the products inventory,
    // and then use the built in print function to print each product to a file.
}