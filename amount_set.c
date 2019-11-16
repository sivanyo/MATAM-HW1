//
// Created by sivan on 13/11/2019.
//

#include "amount_set.h"
#include <stdlib.h>

typedef struct element_node_t {
    ASElement element;
    double amount;
    struct element_node_t *next;
} *ElementNode;

struct AmountSet_t {
    ElementNode head;
    // adding possible iterator tracker
    ElementNode current;
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
    as->head = NULL;

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

void asDestroy(AmountSet set) {
    if (set == NULL) {
        return;
    }

    ElementNode current = set->head;
    ElementNode next;
    while(current != NULL) {
        next = current->next;
        set->free(current->element);
        free(current);
        current = next;
    }

    free(current);
    free(next);
    free(set);
}

AmountSet asCopy(AmountSet set) {
    /*
     * This function will create a copy of the current amount set using following actions:
     * 1. Create a new empty amount set with default values (using the same free, copy and compare functions of the original AS
     * 2. Iterate over all node head in the linked list and copy them in the same order to the new amount set
     * 3. return the new amount set
     */
}

int asGetSize(AmountSet set) {
    if (set == NULL) {
        return -1;
    }

    int count = 0;
    // Creating variable to iterate over linked list
    ElementNode current = set->head;
    while (current != NULL) {
        // Counting until the end of the linked list.
        count++;
        current = current->next;
    }

    return count;
}

bool asContains(AmountSet set, ASElement element) {
    if (set == NULL) {
        false;
    }

    ElementNode current = set->head;
    while (current != NULL) {
        if (!set->compareElements(current->element, element)) {
            return true;
        }
        current = current->next;
    }
    return false;
}


AmountSetResult asGetAmount(AmountSet set, ASElement element, double *outAmount) {
    if (set == NULL || element == NULL) {
        return AS_NULL_ARGUMENT;
    }

    ElementNode current = set->head;
    while (current != NULL) {
        // Going over the elements in the linked list until we find
        // the one we need the amount for.
        if (!set->compareElements(current->element, element)) {
            // The requested element was found, returning success code
            // and updating amount variable.
            *outAmount = current->amount;
            return AS_SUCCESS;
        }
        current = current->next;
    }

    // Item was not found.
    return AS_ITEM_DOES_NOT_EXIST;
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
    if (set->head == NULL) {
        elementNode->next = NULL;
        set->head = elementNode;
    } else {
        /*
         * need to check where to put the new element we're adding to the element nodes linked list
         * we need to iterate over th existing head and use the compare function to compare the element of a node
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

AmountSetResult asChangeAmount(AmountSet set, ASElement element, const double amount) {
    /*
     * This function use asContains to check if an element exists, if it does
     * it will search for it in the linked list and change it's amount.
     */
}

AmountSetResult asDelete(AmountSet set, ASElement element) {
    /*
     * This function will use asContains to check if an element exists, if it does it will search for it,
     * free it and set the element in the linked list pointing to it to point to the object after it in the list.
     */
}

AmountSetResult asClear(AmountSet set) {
    /*
     * This function will iterate over all items in the linked list and will free them.
     */
}

ASElement asGetFirst(AmountSet set) {
    /*
     * This function will set the "current" iterator object for the head of the linked list and return the ASElement it points to.
     */
}

ASElement asGetNext(AmountSet set) {
    /*
     * This function will set the "current" iterator object for the next item in the linked list and return the ASElement it points to.
     */
}