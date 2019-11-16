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
        return AS_OUT_OF_MEMORY;
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
    while (current != NULL) {
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
    if (set == NULL) {
        return NULL;
    }

    AmountSet as = malloc(sizeof(as));
    if (as == NULL) {
        return NULL;
    }

    ElementNode current = set->head;
    ElementNode newNode = malloc(sizeof(newNode));
    newNode->element = set->copy(current->element);
    newNode->amount = current->amount;
    as->head = newNode;
    while(current != NULL) {

    }
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
    if (set == NULL || element == NULL) {
        return AS_NULL_ARGUMENT;
    }

    if (asContains(set, element)) {
        return AS_ITEM_ALREADY_EXISTS;
    }

    ElementNode newNode = malloc(sizeof(newNode));
    if (newNode == NULL) {
        return AS_OUT_OF_MEMORY;
    }
    newNode->element = set->copy(element);
    newNode->amount = 0;
    if (set->head == NULL) {
        // We are at the head of the linked list, adding as first item.
        newNode->next = NULL;
        set->head = newNode;
    } else {
        // We are adding an item to a linked list which already has items,
        // finding the correct position to add the new item.
        ElementNode current = set->head;
        ElementNode previous = current;
        while (current != NULL) {
            if (set->compareElements(current->element, element) > 0) {
                // we found an item in the set that is larger than the
                // item we want to insert, so we will insert the new item
                // before it.
                previous->next = newNode;
                newNode->next = current;
                return AS_SUCCESS;
            }
            previous = current;
            current = current->next;
        }
        // If we reach the end of the loop that means we didn't find a place
        // in the middle of the list for the new element, that means it needs
        // to be placed at the end of the linked list.
        current->next = newNode;
        return AS_SUCCESS;
    }
}

AmountSetResult asChangeAmount(AmountSet set, ASElement element, const double amount) {
    if (set == NULL || element == NULL) {
        return AS_NULL_ARGUMENT;
    }

    ElementNode current = set->head;
    while (current != NULL) {
        if (!set->compareElements(current->element, element)) {
            current->amount += amount;
            return AS_SUCCESS;
        }
        current = current->next;
    }

    return AS_ITEM_DOES_NOT_EXIST;
}

AmountSetResult asDelete(AmountSet set, ASElement element) {
    if (set == NULL || element == NULL) {
        return AS_NULL_ARGUMENT;
    }

    ElementNode current = set->head;
    ElementNode previous = current;
    while (current != NULL) {
        if (!set->compareElements(current->element, element)) {
            previous->next = current->next;
            set->free(current->element);
            free(current);
            return AS_SUCCESS;
        }
        previous = current;
        current = current->next;
    }

    return AS_ITEM_DOES_NOT_EXIST;
}

AmountSetResult asClear(AmountSet set) {
    if (set == NULL) {
        return AS_NULL_ARGUMENT;
    }

    ElementNode current = set->head;
    ElementNode next;
    while (current != NULL) {
        next = current->next;
        set->free(current->element);
        free(current);
        current = next;
    }

    return AS_SUCCESS;
}

ASElement asGetFirst(AmountSet set) {
    if (set == NULL) {
        return NULL;
    }

    if (set->head != NULL) {
        set->current = set->head;
        return set->current->element;
    }

    return NULL;
}

ASElement asGetNext(AmountSet set) {
    if (set == NULL) {
        return NULL;
    }

    if (set->current->next != NULL) {
        set->current = set->current->next;
        return set->current->element;
    }

    return NULL;
}