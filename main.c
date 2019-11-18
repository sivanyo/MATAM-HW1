#include <stdio.h>
#include <stdlib.h>
#include "amount_set.h"

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
    AmountSet set = asCreate(copyInt, freeInt, compareInts);
    addElements(set);
    asClear(set);
    int x = 8, y = 3;
    asRegister(set, &x);
    asChangeAmount(set, &x, x);
    asRegister(set, &y);
    asChangeAmount(set, &y, y);
    asClear(set);
//    int x=1,y=2,z=4,h=3;
//    asRegister(set, &x);
//    asChangeAmount(set, &x, 1);
//    asRegister(set, &y);
//    asChangeAmount(set, &y, 2);
//    asRegister(set, &z);
//    asChangeAmount(set, &z, 4);
//    asRegister(set, &x);
//    asChangeAmount(set, &x, 1);
//    asRegister(set, &h);
//    asChangeAmount(set, &h, 3);
//    asDelete(set, &x);
//    asDelete(set, &h);
//    asDelete(set, &z);
    return 0;
}