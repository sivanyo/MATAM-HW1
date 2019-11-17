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

int main() {
    AmountSet set = asCreate(copyInt, freeInt, compareInts);
    asDestroy(set);
    return 0;
}