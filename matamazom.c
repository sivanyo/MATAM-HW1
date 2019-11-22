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
        *copy = *(int *) number;
    }
    return copy;
}

static void freeInt(ASElement number) { free(number); }

static int compareInts(ASElement lhs, ASElement rhs) {
    return (*(int *) lhs) - (*(int *) rhs);
}

typedef struct product_t {
    const char *name;
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

// custom functions for internal use
static bool warehouseContainsProduct(Matamazom matamazom, const unsigned int id) {
    ProductNode tempProduct;
    tempProduct = matamazom->productsHead;
    while (tempProduct != NULL) {
        if (tempProduct->id == id) {
            return true;
        }
        tempProduct = tempProduct->next;
    }
    return false;
}

static bool warehouseContainsOrder(Matamazom matamazom,const unsigned int id) {
    OrderNode tempOrder;
    tempOrder = matamazom->ordersHead;
    while (tempOrder != NULL) {
        if (tempOrder->id == id) {
            return true;
        }
        tempOrder = tempOrder->next;
    }
    return false;
}

Matamazom matamazomCreate() {
    Matamazom matamazom = malloc(sizeof(*matamazom));
//    OrderNode newOrder = malloc(sizeof(*newOrder));
    if (matamazom == NULL) {
        return NULL;
    }

    return matamazom;
}

// TODO: sivan :) :-) :] :-] :D
void matamazomDestroy(Matamazom matamazom) {
    // This function will iterate over the linked lists for products and orders and free them
    // and then do the same for the matamazom object
}

MatamazomResult mtmNewProduct(Matamazom matamazom, const unsigned int id, const char *name,
                              const double amount, const MatamazomAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice) {
    if (matamazom == NULL || name == NULL || customData == NULL ||
        copyData == NULL || freeData == NULL || prodPrice == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }

    ProductNode newProduct = malloc(sizeof(*newProduct));
    if (newProduct == NULL) {
        return MATAMAZOM_OUT_OF_MEMORY;
    }

    newProduct->product = copyData(customData);
    if (newProduct->product == NULL) {
        free(newProduct);
        return MATAMAZOM_OUT_OF_MEMORY;
    }
    newProduct->name = name;
    newProduct->id = id;
    newProduct->amount = amount;
    newProduct->amountType = amountType;
    newProduct->copy = copyData;
    newProduct->free = freeData;
    newProduct->price = prodPrice;
    newProduct->next = NULL;

    if (matamazom->productsHead == NULL) {
        matamazom->productsHead = newProduct;
        return MATAMAZOM_SUCCESS;
    }

    if (id < matamazom->productsHead->id) {
        newProduct->next = matamazom->productsHead;
        matamazom->productsHead = newProduct;
        return MATAMAZOM_SUCCESS;
    }

    ProductNode current = matamazom->productsHead;
    ProductNode previous = current;

    while (current != NULL) {
        if (newProduct->id < current->id) {
            previous->next = newProduct;
            newProduct->next = current;
            return MATAMAZOM_SUCCESS;
        }
        previous = current;
        current = current->next;
    }
    previous->next = newProduct;
    return MATAMAZOM_SUCCESS;
}

// TODO: sivan :) :-) :] :-] :D
MatamazomResult mtmChangeProductAmount(Matamazom matamazom, const unsigned int id, const double amount) {
    if(matamazom==NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if (amount==0){
        return MATAMAZOM_SUCCESS;
    }
    ProductNode temp = matamazom->productsHead;
    while(temp != NULL){
        if(temp->id==id){
            if (amount > 0){
                temp->amount += amount;
                return MATAMAZOM_SUCCESS;
            }
            else if((temp->amount - amount) >= 0 ) {
                temp->amount += amount;
                return MATAMAZOM_SUCCESS;
            }
            else{
                return MATAMAZOM_INSUFFICIENT_AMOUNT;
            }
        }
        temp = temp->next;
    }
    return MATAMAZOM_PRODUCT_NOT_EXIST;
}

// TODO: sivan :) :-) :] :-] :D
MatamazomResult mtmClearProduct(Matamazom matamazom, const unsigned int id) {
    if (matamazom == NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }

    if (!warehouseContainsProduct(id)){
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }

    if (matamazom->productsHead->id == id){
        ProductNode temp = matamazom->productsHead;
        matamazom->productsHead=temp->next;
        free(temp);
    }

    ProductNode previous = matamazom->productsHead;
    ProductNode nextProduct = previous->next;

    while (nextProduct != NULL){
        if(nextProduct->id == id){
            previous->next = nextProduct->next;
            free(nextProduct);
            return MATAMAZOM_SUCCESS;
        }
        previous=previous->next;
        nextProduct=nextProduct->next;
    }

    //product has been deleted, how should I change the income and the orders?

}

unsigned int mtmCreateNewOrder(Matamazom matamazom) {
    if (matamazom == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    OrderNode newOrder = malloc(sizeof(newOrder));
    if (newOrder == NULL) {
        return 0;
    }
    if (matamazom->ordersHead == NULL) {
        newOrder->id = 1;
        matamazom->ordersHead = newOrder;
        return newOrder->id;
    }
    OrderNode temp = matamazom->ordersHead;
    while (temp != NULL) {
        if (temp->next == NULL) {
            newOrder->id = temp->id + 1;
            temp->next = newOrder;
            return newOrder->id;
        }
        temp = temp->next;
    }
}
//
// TODO: sivan :) :-) :] :-] :D

    MatamazomResult mtmChangeProductAmountInOrder(Matamazom matamazom, const unsigned int orderId,
                                                  const unsigned int productId, const double amount) {
        if (matamazom==NULL){
            return MATAMAZOM_NULL_ARGUMENT;
        }
        OrderNode temp= matamazom->ordersHead;
        while(temp!=NULL){
            if(temp->id==orderId){

            }
        }

    }

// TODO: sivan :) :-) :] :-] :D
    MatamazomResult mtmShipOrder(Matamazom matamazom, const unsigned int orderId) {
        // This function will check that the inventory in the products linked list
        // is able to ship the required order, will update the income and the inventory
        // for each product and then delete the order
    }

// TODO: sivan :) :-) :] :-] :D
    MatamazomResult mtmCancelOrder(Matamazom matamazom, const unsigned int orderId) {
        if (matamazom == NULL) {
            return MATAMAZOM_NULL_ARGUMENT;
        }
        //we need to replace the head of the list
        // FIXME: #8 bad practice, you changed the external iterator of the object for now reason. (line 163).
        if (matamazom->ordersHead->id = orderId) {
            matamazom->ordersCurrent = matamazom->ordersHead->next;
            free(matamazom->ordersHead);
            // FIXME: #9 this line will assign a freed object to ordersHead - not what we want.
            matamazom->ordersHead = matamazom->ordersHead;
        }
        // FIXME: #9 if we already made a change, we shouldn't arrive at this piece of code, need to add a return call in previous block.
        matamazom->ordersCurrent = matamazom->ordersHead->next;
        // FIXME: #10 pretty sure this isn't the correct loop logic we need to use.
        while (matamazom->ordersCurrent->id != orderId) {
            if (matamazom->ordersCurrent->next = NULL) {
                return MATAMAZOM_ORDER_NOT_EXIST;
            }
            matamazom->ordersCurrent = matamazom->ordersCurrent->next;
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