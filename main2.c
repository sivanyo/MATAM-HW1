//
// Created by Mor on 21/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include "amount_set.h"
#include "matamazom.h"

static MtmProductData copyDouble(MtmProductData number) {
    double *copy = malloc(sizeof(*copy));
    if (copy) {
        *copy = *(double *) number;
    }
    return copy;
}

static void freeDouble(MtmProductData number) {
    free(number);
}

static double simplePrice(MtmProductData basePrice, const double amount) {
    return (*(double *) basePrice) * amount;
}

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

static void addElements(AmountSet set) {
    int ids[7] = {0, 4, 2, 5, 7, 3, 1};
    for (int i = 0; i < 7; ++i) {
        asRegister(set, ids + i);
    }
    asChangeAmount(set, ids, 50);
    asChangeAmount(set, ids + 2, 10.5);
    asChangeAmount(set, ids + 1, 3);
}

int main() {
    Matamazom mtz = matamazomCreate();
    unsigned int orderId = mtmCreateNewOrder(mtz);
    const char *prodName = "Mor";
    double basePrice = 3;
    mtmNewProduct(mtz, 1, prodName, 5, MATAMAZOM_INTEGER_AMOUNT, &basePrice, copyDouble, freeDouble, simplePrice);
    prodName = "Sivan";
//    MatamazomResult check = mtmCancelOrder(mtz, orderId);
    FILE* test = fopen("./test.txt", "w");
    mtmPrintInventory(mtz, test);
    fprintf(test, prodName);
//    fprintf(test, "hello");
//    fclose(test);
    return 0;
}