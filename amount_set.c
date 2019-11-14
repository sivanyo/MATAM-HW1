//
// Created by sivan on 13/11/2019.
//

#include "amount_set.h"
#include <stdlib.h>

typedef struct element_node_t {
    ASElement element;
    double amount;
    struct element_node_t* next;
} *ElementNode;

struct AmountSet_t {
    ElementNode elements;
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
    as-> elements = NULL;

    if (copyElement == NULL) {
        return NULL;
    }
    as->copy = copyElement;

    if (freeElement == NULL) {
        return NULL;
    }
    as->free = freeElement;

    if (compareElements == NULL) {
        return NULL;
    }
    as->compareElements = compareElements;

    return as;
}

AmountSetResult asRegister(AmountSet set, ASElement element) {
    if (set == NULL) {
        return AS_NULL_ARGUMENT;
    }

    if (asContains(set, element)) {
        return AS_SUCCESS;
    }

    ElementNode elementNode = malloc(sizeof(element));
    elementNode->element = element;
    elementNode->amount = 0;
    if (set->elements == NULL) {
        elementNode-> next = NULL;
        set-> elements = elementNode;
    } else {
        /*
         * need to check where to put the new element we're adding to the element nodes linked list
         * we need to iterate over th existing elements and use the compare function to compare the element of a node
         * to the element we received with the function, if the value of the current node we're checking as smaller than the value
         * of our supplied element, and the value of the next supplied node is larger, then that is where we need to add the item
         * if the value is lower on both, our element needs to be further up in the linked note (for example our value is 4 and we're checking
         * the number 1 and 2, so it should be after them
         *
         * use a boolean to check inside the while loop that we haven't added the item to the linked list yet,
         * also need to check that we haven't reached the end of the list, if we have reached the end of the list we add the new element at the end of it
         */
    }
}