//
// Created by Mor on 21/11/2019.
//

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matamazom.h"
#include "matamazom_print.h"
#include "amount_set.h"

#define FULL_OFFSET 0.001
#define HALF_OFFSET 0.5

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
    MtmCopyData copyFunction;
    MtmFreeData freeFunction;
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
    OrderNode ordersHead;
    unsigned int lastOrderId;
};

// custom functions for internal use
static bool warehouseContainsProduct(Matamazom matamazom,
                                     const unsigned int id) {
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

static bool warehouseContainsOrder(Matamazom matamazom,
                                   const unsigned int id) {
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
static bool removeProductFromOrders(Matamazom matamazom, unsigned int id) {
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

static bool checkLegalName(const char *name) {
    if (strlen(name) == 0) {
        return false;
    } else if ((name[0] >= 'a' && name[0] <= 'z') ||
               (name[0] >= 'A' && name[0] <= 'Z')) {
        return true;
    } else if (name[0] >= '0' && name[0] <= '9') {
        return true;
    } else {
        return false;
    }
}

/**
 * Function to check if the provided amount matches the AmountType specified.
 */
static bool checkAmountType(double amount,
                            const MatamazomAmountType amountType) {
    // Checking if number is a legal full int
    if (amountType == MATAMAZOM_INTEGER_AMOUNT) {
        if (floor(amount) == amount || floor(amount) + FULL_OFFSET == amount ||
            ceil(amount) - FULL_OFFSET == amount) {
            return true;
        }
        return false;
    } else if (amountType == MATAMAZOM_HALF_INTEGER_AMOUNT) {
        if (floor(amount) == amount || floor(amount) + FULL_OFFSET == amount ||
            ceil(amount) - FULL_OFFSET == amount ||
            floor(amount) + HALF_OFFSET == amount ||
            floor(amount) + HALF_OFFSET + FULL_OFFSET == amount ||
            floor(amount) + HALF_OFFSET - FULL_OFFSET == amount) {
            return true;
        }
        return false;
    } else {
        return true;
    }
}

static MatamazomResult validProductCheck(Matamazom matamazom,
                                         const unsigned int id,
                                         const char *name, const double amount,
                                         const MatamazomAmountType amountType,
                                         const MtmProductData customData,
                                         MtmCopyData copyData,
                                         MtmFreeData freeData,
                                         MtmGetProductPrice prodPrice) {
    if (matamazom == NULL || name == NULL || customData == NULL ||
        copyData == NULL || freeData == NULL || prodPrice == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }

    if (!checkLegalName(name)) {
        return MATAMAZOM_INVALID_NAME;
    }

    if (amount < 0 || !checkAmountType(amount, amountType)) {
        return MATAMAZOM_INVALID_AMOUNT;
    }

    if (warehouseContainsProduct(matamazom, id)) {
        return MATAMAZOM_PRODUCT_ALREADY_EXIST;
    }

    return MATAMAZOM_SUCCESS;
}

static ProductNode getProductById(Matamazom matamazom, const unsigned int id) {
    if (matamazom == NULL) {
        return NULL;
    }
    ProductNode current = matamazom->productsHead;
    while (current->id != id) {
        current = current->next;
    }
    return current;
}


/**
 * This function receives an amount set and an ID and amount, and changes
 * the amount of the correct productId in the amount set accordingly,
 * the logic is identical to the one in mtmChangeProductAmountInOrder.
 */
MatamazomResult changeAmountOfProductInSet(AmountSet set,
                                           unsigned int productId,
                                           const double amount) {
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
            return MATAMAZOM_SUCCESS;
        } else {
            asRegister(set, &productId);
            asChangeAmount(set, &productId, amount);
            return MATAMAZOM_SUCCESS;
        }
    }
}

/**
 * This function recieves a matamazon warehouse and product ID and
 * returns the amount of that product in the warehouse.
 */
static double getProductAmount(Matamazom matamazom, const unsigned int id) {
    if (matamazom == NULL) {
        return -1;
    }
    ProductNode current = matamazom->productsHead;
    while (current != NULL) {
        if (current->id == id) {
            return current->amount;
        }
        current = current->next;
    }

    // Shouldn't get here.
    return -1;
}

static MatamazomResult changeProductIncome(Matamazom matamazom,
                                           const unsigned int id,
                                           double income) {
    if (matamazom == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }

    ProductNode current = matamazom->productsHead;
    while (current != NULL) {
        if (current->id == id) {
            current->income += income;
            return MATAMAZOM_SUCCESS;
        }
        current = current->next;
    }
    return MATAMAZOM_PRODUCT_NOT_EXIST;
}

static double calculateProductPrice(Matamazom matamazom, const unsigned int id,
                                    double amount) {
    if (matamazom == NULL) {
        return -1;
    }
    ProductNode current = matamazom->productsHead;
    while (current != NULL) {
        if (current->id == id) {
            return current->price(current->product, amount);
        }
        current = current->next;
    }
    return -1;
}

static MatamazomResult removeShippedOrder(Matamazom matamazom,
                                          const unsigned int id) {
    if (matamazom == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }

    if (matamazom->ordersHead->id == id) {
        OrderNode temp = matamazom->ordersHead;
        matamazom->ordersHead = temp->next;
        free(temp);

        return MATAMAZOM_SUCCESS;
    }
    OrderNode previous = matamazom->ordersHead;
    OrderNode nextProduct = previous->next;
    while (previous != NULL) {
        if (nextProduct->id == id) {
            previous->next = nextProduct->next;
            free(nextProduct);
            return MATAMAZOM_SUCCESS;
        }
        previous = previous->next;
        nextProduct = nextProduct->next;
    }
    // Shouldn't get here.
    return MATAMAZOM_ORDER_NOT_EXIST;
}

static ProductNode fillProductFields(ProductNode product,
                                     const unsigned int id,
                                     const char *copyName, const double amount,
                                     const MatamazomAmountType amountType,
                                     MtmFreeData freeData,
                                     MtmGetProductPrice prodPrice
) {
    product->id = id;
    product->name = copyName;
    product->amount = amount;
    product->amountType = amountType;
    product->freeFunction = freeData;
    product->price = prodPrice;
    product->income = 0;
    product->next = NULL;

    return product;
}

static MatamazomResult addProductToLinkedList(Matamazom matamazom,
                                              ProductNode newProduct) {
    if (matamazom->productsHead == NULL) {
        matamazom->productsHead = newProduct;
        return MATAMAZOM_SUCCESS;
    }

    if (newProduct->id < matamazom->productsHead->id) {
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

Matamazom matamazomCreate() {
    Matamazom matamazom = malloc(sizeof(*matamazom));
    if (matamazom == NULL) {
        return NULL;
    }
    matamazom->productsHead = NULL;
    matamazom->ordersHead = NULL;
    matamazom->lastOrderId = 0;

    return matamazom;
}

void matamazomDestroy(Matamazom matamazom) {
    if (matamazom == NULL) {
        return;
    }

    ProductNode tempProduct = matamazom->productsHead;
    ProductNode productToDelete = matamazom->productsHead;
    while (tempProduct != NULL) {
        tempProduct = tempProduct->next;
        productToDelete->freeFunction(productToDelete->product);
        free((char *) productToDelete->name);
        free(productToDelete);
        productToDelete = tempProduct;
    }

    OrderNode tempOrder = matamazom->ordersHead;
    OrderNode orderToDelete = matamazom->ordersHead;
    while (tempOrder != NULL) {
        tempOrder = tempOrder->next;
        asDestroy(orderToDelete->orderProducts);
        free(orderToDelete);
        orderToDelete = tempOrder;

    }
    free(matamazom);
    matamazom = NULL;
}

MatamazomResult mtmNewProduct(Matamazom matamazom, const unsigned int id,
                              const char *name, const double amount,
                              const MatamazomAmountType amountType,
                              const MtmProductData customData,
                              MtmCopyData copyData,
                              MtmFreeData freeData,
                              MtmGetProductPrice prodPrice) {
    MatamazomResult checksResult = validProductCheck(matamazom, id, name,
                                                     amount, amountType,
                                                     customData, copyData,
                                                     freeData, prodPrice);

    if (checksResult != MATAMAZOM_SUCCESS) {
        return checksResult;
    }

    ProductNode newProduct = malloc(sizeof(*newProduct));
    if (newProduct == NULL) {
        return MATAMAZOM_OUT_OF_MEMORY;
    }

    newProduct->copyFunction = copyData;
    newProduct->product = newProduct->copyFunction(customData);
    if (newProduct->product == NULL) {
        free(newProduct);
        return MATAMAZOM_OUT_OF_MEMORY;
    }
    char *copyName = (char *) malloc(sizeof(char) * strlen(name) + 1);
    if (copyName == NULL) {
        return MATAMAZOM_OUT_OF_MEMORY;
    }
    strcpy(copyName, name);
    newProduct = fillProductFields(newProduct, id, copyName, amount,
                                   amountType, freeData, prodPrice);

    return addProductToLinkedList(matamazom, newProduct);
}

MatamazomResult mtmChangeProductAmount(Matamazom matamazom,
                                       const unsigned int id,
                                       const double amount) {
    if (matamazom == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }

    ProductNode current = matamazom->productsHead;
    while (current != NULL) {
        if (current->id == id) {
            if (!checkAmountType(amount, current->amountType)) {
                return MATAMAZOM_INVALID_AMOUNT;
            }
            if (amount >= 0) {
                current->amount += amount;
                return MATAMAZOM_SUCCESS;
            } else if ((current->amount + amount) >= 0) {
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
        temp->freeFunction(temp->product);
        free((char *) temp->name);
        matamazom->productsHead = temp->next;
        free(temp);
        return MATAMAZOM_SUCCESS;
    }

    ProductNode previous = matamazom->productsHead;
    ProductNode current = previous->next;

    while (current != NULL) {
        if (current->id == id) {
            previous->next = current->next;
            current->freeFunction(current->product);
            free((char *) current->name);
            free(current);
            return MATAMAZOM_SUCCESS;
        }
        previous = previous->next;
        current = current->next;
    }

    // Shouldn't get here
    return MATAMAZOM_PRODUCT_NOT_EXIST;
}

unsigned int mtmCreateNewOrder(Matamazom matamazom) {
    if (matamazom == NULL) {
        return 0;
    }
    OrderNode newOrder = malloc(sizeof(*newOrder));
    if (newOrder == NULL) {
        return 0;
    }

    newOrder->orderProducts = asCreate(copyInt, freeInt, compareInts);
    newOrder->next = NULL;

    if (matamazom->ordersHead == NULL) {
        newOrder->id = 1;
        matamazom->ordersHead = newOrder;
        matamazom->lastOrderId = newOrder->id;
        return newOrder->id;
    }
    OrderNode temp = matamazom->ordersHead;
    while (temp != NULL) {
        if (temp->next == NULL) {
            newOrder->id = matamazom->lastOrderId + 1;
            matamazom->lastOrderId += 1;
            temp->next = newOrder;
            return newOrder->id;
        }
        temp = temp->next;
    }

    // Shouldn't get here.
    return 0;
}

MatamazomResult mtmChangeProductAmountInOrder(Matamazom matamazom,
                                              const unsigned int orderId,
                                              const unsigned int productId,
                                              const double amount) {
    if (matamazom == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }

    if (!warehouseContainsOrder(matamazom, orderId)) {
        return MATAMAZOM_ORDER_NOT_EXIST;
    }

    if (!warehouseContainsProduct(matamazom, productId)) {
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }
    OrderNode order = matamazom->ordersHead;
    while (order != NULL) {
        if (order->id == orderId) {
            // We found the order we need to update.
            MatamazomAmountType type = MATAMAZOM_ANY_AMOUNT;
            ProductNode product = matamazom->productsHead;
            while (product != NULL) {
                if (product->id == productId) {
                    type = product->amountType;
                }
                product = product->next;
            }
            if (!checkAmountType(amount, type)) {
                return MATAMAZOM_INVALID_AMOUNT;
            }
            AmountSet productsSet = order->orderProducts;
            return changeAmountOfProductInSet(productsSet, productId, amount);
        }
        order = order->next;
    }

    return MATAMAZOM_PRODUCT_NOT_EXIST;
}

MatamazomResult mtmShipOrder(Matamazom matamazom, const unsigned int orderId) {
    if (matamazom == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if (!warehouseContainsOrder(matamazom, orderId)) {
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    OrderNode current = matamazom->ordersHead;
    while (current->id != orderId) {
        current = current->next;
    }

    double tempAmount = 0;
    unsigned int *productId =
            (unsigned int *) asGetFirst(current->orderProducts);
    while (productId != (unsigned int *) NULL) {
        asGetAmount(current->orderProducts, productId, &tempAmount);
        if (getProductAmount(matamazom, *productId) - tempAmount < 0) {
            return MATAMAZOM_INSUFFICIENT_AMOUNT;
        }
        productId = (unsigned int *) asGetNext(current->orderProducts);
    }

    productId = (unsigned int *) asGetFirst(current->orderProducts);
    while (productId != (unsigned int *) NULL) {
        asGetAmount(current->orderProducts, productId, &tempAmount);
        mtmChangeProductAmount(matamazom, *productId, -(tempAmount));
        double income =
                calculateProductPrice(matamazom, *productId, tempAmount);
        changeProductIncome(matamazom, *productId, income);
        productId = (unsigned int *) asGetNext(current->orderProducts);
    }
    asDestroy(current->orderProducts);
    removeShippedOrder(matamazom, orderId);
    return MATAMAZOM_SUCCESS;
}

MatamazomResult mtmCancelOrder(Matamazom matamazom,
                               const unsigned int orderId) {
    if (matamazom == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }

    if (!warehouseContainsOrder(matamazom, orderId)) {
        return MATAMAZOM_ORDER_NOT_EXIST;
    }

    if (matamazom->ordersHead->id == orderId) {
        OrderNode order = matamazom->ordersHead;
        matamazom->ordersHead = order->next;
        asDestroy(order->orderProducts);
        //int check = asGetSize(order->orderProducts);
        order->orderProducts = NULL;
        free(order);
        return MATAMAZOM_SUCCESS;
    }
    OrderNode previous = matamazom->ordersHead;
    OrderNode nextProduct = previous->next;
    while (previous != NULL) {
        if (nextProduct->id == orderId) {
            previous->next = nextProduct->next;
            asDestroy(nextProduct->orderProducts);
            free(nextProduct);
            return MATAMAZOM_SUCCESS;
        }
        previous = previous->next;
        nextProduct = nextProduct->next;
    }
    // Shouldn't get here.
    return MATAMAZOM_ORDER_NOT_EXIST;
}

MatamazomResult mtmPrintInventory(Matamazom matamazom, FILE *output) {
    if (matamazom == NULL || output == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    ProductNode current = matamazom->productsHead;
    double price = 0;
    fprintf(output, "Inventory Status:\n");
    while (current != NULL) {
        price = calculateProductPrice(matamazom, current->id, 1);
        mtmPrintProductDetails(current->name, current->id,
                               current->amount, price, output);
        current = current->next;
    }
    return MATAMAZOM_SUCCESS;
}

MatamazomResult mtmPrintOrder(Matamazom matamazom,
                              const unsigned int orderId, FILE *output) {
    if (matamazom == NULL || output == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if (!warehouseContainsOrder(matamazom, orderId)) {
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    mtmPrintOrderHeading(orderId, output);
    OrderNode order = matamazom->ordersHead;
    while (order->id != orderId) {
        order = order->next;
    }
    double tempAmount = 0;
    double total = 0;
    unsigned int *productId =
            (unsigned int *) asGetFirst(order->orderProducts);
    while (productId != (unsigned int *) NULL) {
        asGetAmount(order->orderProducts, productId, &tempAmount);
        ProductNode product = getProductById(matamazom, *productId);
        double productTotalPrice =
                product->price(product->product, tempAmount);
        total += productTotalPrice;
        mtmPrintProductDetails(product->name, product->id,
                               tempAmount, productTotalPrice, output);
        productId = (unsigned int *) asGetNext(order->orderProducts);
    }
    mtmPrintOrderSummary(total, output);
    return MATAMAZOM_SUCCESS;
}

MatamazomResult mtmPrintBestSelling(Matamazom matamazom, FILE *output) {
    if (matamazom == NULL || output == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    fprintf(output, "Best Selling Product:\n");
    ProductNode current = matamazom->productsHead;
    const char *name = "";
    double maxIncome = 0;
    unsigned int maxProductId = 0;
    while (current != NULL) {
        if (current->income > maxIncome) {
            maxIncome = current->income;
            maxProductId = current->id;
            name = current->name;
        }
        current = current->next;
    }

    if (maxProductId == 0) {
        fprintf(output, "none\n");
    } else {
        mtmPrintIncomeLine(name, maxProductId, maxIncome, output);
    }

    return MATAMAZOM_SUCCESS;
}

MatamazomResult mtmPrintFiltered(Matamazom matamazom,
                                 MtmFilterProduct customFilter, FILE *output) {
    if (matamazom == NULL || output == NULL || customFilter == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    ProductNode current = matamazom->productsHead;
    double price = 0;
    while (current != NULL) {
        if (customFilter(current->id, current->name,
                         current->amount, current->product)) {
            price = calculateProductPrice(matamazom, current->id, 1);
            mtmPrintProductDetails(current->name, current->id,
                                   current->amount, price, output);
        }
        current = current->next;
    }

    return MATAMAZOM_SUCCESS;
}