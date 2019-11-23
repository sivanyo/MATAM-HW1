//
// Created by Mor on 21/11/2019.
//

#include <stdlib.h>
#include "matamazom.h"
#include "amount_set.h"

#define OFFSET 0.01

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

static bool warehouseContainsOrder(Matamazom matamazom, const unsigned int id) {
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

/**
 * This function will remove products from all orders which contain them,
 * in order to make sure that no orders contain products that were removed from
 * warehouse.
 */
static bool removeProductFromOrders(Matamazom matamazom, const unsigned int id) {
    if (matamazom == NULL) {
        return false;
    }

    OrderNode currentOrder = matamazom->ordersHead;
    while (currentOrder != NULL) {
        AmountSet currentProduct = currentOrder->orderProducts;
        // Checking if the product we want to remove is contained in the list
        // of products in this order.
        if (asContains(currentProduct, &id)) {
            if (asDelete(currentProduct, &id) != AS_SUCCESS) {
                return false;
            }
        }
        currentOrder = currentOrder->next;
    }

    return true;
}

/**
 * This function receives an amount set and an ID and amount, and changes the amount of the correct productId
 * in the amount set accordingly, the logic is identical to the one in mtmChangeProductAmountInOrder.
 */
MatamazomResult changeAmountOfProductInSet(AmountSet set, const unsigned int productId, const double amount) {
    double currentAmount = 0;
    if (asContains(set, &productId)) {
        // We found the product we need to change, checking
        // if we need to remove it or just update the amount.
        if (amount < 0) {
            asGetAmount(set, &productId, &currentAmount);
            if (-amount >= currentAmount) {
                asDelete(set, &productId);
                return MATAMAZOM_SUCCESS;
            } else {
                asChangeAmount(set, &productId, amount);
                return MATAMAZOM_SUCCESS;
            }
        } else {
            asChangeAmount(set, &productId, amount);
            return MATAMAZOM_SUCCESS;
        }
    } else {
        // Product does not exist in order, checking if amount is valid
        // to add it to the order.
        if (amount <= 0) {
            return MATAMAZOM_PRODUCT_NOT_EXIST;
        } else {
            asRegister(set, &productId);
            asChangeAmount(set, &productId, amount);
            return MATAMAZOM_SUCCESS;
        }
    }
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
    free(matamazom->productsCurrent);
    ProductNode tempProduct = matamazom->productsHead;
    ProductNode ProductToDelete = matamazom->productsHead;
    while (tempProduct != NULL) {
        tempProduct = tempProduct->next;
        free(ProductToDelete);
        ProductNode toDelete = tempProduct;
    }

    free(matamazom->ordersCurrent);
    OrderNode tempOrder = matamazom->ordersHead;
    OrderNode OrderToDelete = matamazom->ordersHead;
    while (tempOrder != NULL) {
        tempOrder = tempOrder->next;
        free(OrderToDelete);
        ProductNode toDelete = tempOrder;
    }
}

// FIXME: add checks for legal product name
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

MatamazomResult mtmChangeProductAmount(Matamazom matamazom, const unsigned int id, const double amount) {
    if (matamazom == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }

    ProductNode current = matamazom->productsHead;
    while (current != NULL) {
        if (current->id == id) {
            if (amount >= 0) {
                current->amount += amount;
                return MATAMAZOM_SUCCESS;
            } else if ((current->amount - amount) >= 0) {
                current->amount += amount;
                return MATAMAZOM_SUCCESS;
            } else {
                return MATAMAZOM_INSUFFICIENT_AMOUNT;
            }
        }
        current = current->next;
    }
    return MATAMAZOM_PRODUCT_NOT_EXIST;
}

MatamazomResult mtmClearProduct(Matamazom matamazom, const unsigned int id) {
    if (matamazom == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }

    if (!warehouseContainsProduct(matamazom, id)) {
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }

    // Removing the product from all orders.
    removeProductFromOrders(matamazom, id);

    // Checking if the product we need to remove is the first stored product.
    if (matamazom->productsHead->id == id) {
        ProductNode temp = matamazom->productsHead;
        matamazom->productsHead = temp->next;
        free(temp);
    }

    ProductNode previous = matamazom->productsHead;
    ProductNode current = previous->next;

    while (current != NULL) {
        if (current->id == id) {
            previous->next = current->next;
            free(current);
            return MATAMAZOM_SUCCESS;
        }
        previous = previous->next;
        current = current->next;
    }
}

unsigned int mtmCreateNewOrder(Matamazom matamazom) {
    if (matamazom == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    OrderNode newOrder = malloc(sizeof(newOrder));
    if (newOrder == NULL) {
        return 0;
    }

    newOrder->orderProducts = asCreate(copyInt, freeInt,compareInts);

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

    // Shouldn't get here.
    return 0;
}

// TODO: this function will require testing
// FIXME: we still didn't address whether the amount is legal according to amount type.
MatamazomResult mtmChangeProductAmountInOrder(Matamazom matamazom, const unsigned int orderId,
                                              const unsigned int productId, const double amount) {
    if (matamazom == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }

    OrderNode current = matamazom->ordersHead;
    while (current != NULL) {
        if (current->id == orderId) {
            // We found the order we need to update.
            AmountSet productsSet = current->orderProducts;
            return changeAmountOfProductInSet(productsSet, productId, amount);
        }
        current = current->next;
    }

    return MATAMAZOM_ORDER_NOT_EXIST;
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
    if (matamazom->ordersHead->id = orderId) {
        OrderNode temp = matamazom->ordersHead;
        matamazom->ordersHead = temp->next;
        free(temp);
        return MATAMAZOM_SUCCESS;
    }
    OrderNode previous = matamazom->ordersHead;
    OrderNode nextProduct = previous->next;
    while (previous != NULL) {
        if (nextProduct->id == orderId) {
            previous->next = nextProduct->next;
            free(nextProduct);
            return MATAMAZOM_SUCCESS;
        }
        previous = previous->next;
        nextProduct = nextProduct->next;
    }

    return MATAMAZOM_PRODUCT_NOT_EXIST;
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