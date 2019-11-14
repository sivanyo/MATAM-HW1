//
// Created by sivan on 13/11/2019.
//

#include "amount_set.h"
#include <stdlib.h>

#define INITIAL_SIZE 10

struct AmountSet_t {
    ASElement* elements;
    int size;
    int maxSize;
    CopyASElement copy;
    FreeASElement free;
    CompareASElements compareElements;
};

AmountSet asCreate(CopyASElement copyElement,
                   FreeASElement freeElement,
                   CompareASElements compareElements) {
    AmountSet as = malloc(sizeof(as));
    if (as == NULL) {
        return NULL;
    }

    as->elements = malloc(INITIAL_SIZE * sizeof(ASElement));
    if (as->elements == NULL) {
        free(as);
        return NULL;
    }

    as->size = 0;
    as->maxSize = INITIAL_SIZE;
    as->copy = copyElement;
    as->free = freeElement;
    as->compareElements = compareElements;

    return as;
}

typedef struct element_t {
    ASElement element;
    double amount;
    struct element_t* next;
} *Element;