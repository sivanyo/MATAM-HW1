//
// Created by sivan on 29/11/2019.
//

#include <stdlib.h>
#include <stdbool.h>
#include "amount_set.h"

typedef struct TestElement_t {
    int number;
    char letter;
} *testElement;

testElement copyFunction(testElement Element) {
    if (Element == NULL) {
        return NULL;
    }
    testElement newElement = malloc(sizeof(*newElement));
    if (newElement == NULL) {
        return NULL;
    }
    newElement->letter = Element->letter;
    newElement->number = Element->number;
    return newElement;
}

void freeFunction(testElement Element) {
    free(Element);
}

// this function return a negative number if element2 is bigger than element1
// and zero if they equal
int compareFunction(testElement element1, testElement element2) {
    return (element1->number - element2->number);
}

bool asCreateCheck1() {
    if (asCreate(NULL, NULL,
                 (CompareASElements) compareFunction) == NULL) {
        return true;
    }
    return false;
}

bool asCreateCheck2() {
    AmountSet newSet = asCreate((CopyASElement) copyFunction,
                                (FreeASElement) freeFunction,
                                (CompareASElements) compareFunction);
    if (newSet == NULL) {
        return false;
    }
    free(newSet);
    return true;
}

bool asGetSizeCheck(AmountSet set, int size) {
    if (asGetSize(set) != size) {
        return false;
    }
    return true;
}

bool asContainsCheck(AmountSet set, ASElement element) {
    if (asContains(set, element) == true) {
        return true;
    }
    return false;
}

bool asGetAmountCheck(AmountSet set, ASElement element, double *outAmount) {
    double amount1, amount2;
    asGetAmount(set, element, &amount1);
    if (asChangeAmount(set, element, amount1) == AS_SUCCESS) {
        asGetAmount(set, element, &amount2);
        return (amount2 - amount1);
    }
    return false;
}

bool asRegisterCheck(AmountSet set, ASElement element) {
    asRegister(set, element);
    return asContains(set, element);

}

bool asChangeAmountCheck(AmountSet set,
                         ASElement element, const double amount) {
    if (asChangeAmount(set, element, amount) != AS_SUCCESS) {
        return false;
    }
    return true;
}

bool asDeleteCheck(AmountSet set, ASElement element) {
    if (asDelete(set, element) != AS_SUCCESS) {
        return false;
    }
    return true;
}

bool asClearCheck(AmountSet set) {
    if (asClear(set) != AS_SUCCESS) {
        return false;
    }
    return true;
}

#define RUN_TEST(x, ...) do {\
 printf(#x "(" #__VA_ARGS__ ")");\
 printf(x(__VA_ARGS__) ? " [Excellent!!!]" : "[FAIL]"); \
 printf("\n\n");\
} while(0)

#define RUN_TEST_NO_ARGS(x) do {\
 printf(#x "()");\
 printf(x() ? " [Good Job]" : "[FAIL]"); \
 printf("\n\n");\
} while(0)


int main() {

    // Trying to create empty set with NULL arguments.
    RUN_TEST_NO_ARGS(asCreateCheck1);
    // Creating empty set and then freeing.
    RUN_TEST_NO_ARGS(asCreateCheck2);

    // Creating set to use in multiple test scenarios.
    AmountSet set_test = asCreate((CopyASElement) copyFunction,
                                  (FreeASElement) freeFunction,
                                  (CompareASElements) compareFunction);

    // Checking that created set is empty.
    RUN_TEST(asGetSizeCheck, set_test, 0);
    testElement element = malloc(sizeof(*element));

    if (element == NULL) {
        free(element);
    } else {
        element->letter = 'a';
        element->number = 1;
        RUN_TEST(!asContainsCheck, set_test, element);
        RUN_TEST(asRegisterCheck, set_test, element);
        double out_amount = 5;
        RUN_TEST(asChangeAmountCheck, set_test, element, out_amount);
        out_amount = 1;
        RUN_TEST(asGetAmountCheck, set_test, element, &out_amount);
        RUN_TEST(asDeleteCheck, set_test, element);
    }
    RUN_TEST(asClearCheck, set_test);
    free(element);
    free(set_test);

}