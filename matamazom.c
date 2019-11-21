//
// Created by Mor on 21/11/2019.
//

#include "matamazom.h"
#include <stdlib.h>

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
    unsigned int productId;
    double amount;
    struct order_t *next;
} *OrderNode;

struct Matamazom_t {
    ProductNode productsHead;
    ProductNode productsCurrent;
    OrderNode ordersHead;
    OrderNode ordersCurrent;
};
