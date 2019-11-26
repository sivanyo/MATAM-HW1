//
// Created by Mor on 21/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include "amount_set.h"
#include "matamazom.h"

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
//    Matamazom mtz = matamazomCreate();
//    unsigned int orderId = mtmCreateNewOrder(mtz);
//    MatamazomResult check = mtmCancelOrder(mtz, orderId);
//    FILE* test = fopen("./test.txt", "w");
//    fprintf(test, "hello");
//    fclose(test);
    return 0;
}