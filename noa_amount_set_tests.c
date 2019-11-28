#include <stdbool.h>
#include <string.h>
#include "amount_set.h"
#include <stdlib.h>

typedef struct TestElement_t *TestElement;

struct TestElement_t {
    int x;
    int y;
};

TestElement copyTestElement(TestElement testElement) {
    TestElement new = malloc(sizeof(*new));
    if (new == NULL) {
        return NULL;
    }
    new->x = testElement->x;
    new->y = testElement->y;
    return new;
}

void freeTestElement(TestElement testElement) {
    free(testElement);
}

int compareTestElement(TestElement testElement1, TestElement testElement2) {
    //printf("%d", (testElement1->x - testElement2->x));
    return (testElement1->x - testElement2->x);
}

bool test_create_should_return_null() {
    if (asCreate(NULL, NULL, (CompareASElements) compareTestElement) == NULL) {
        return true;
    }
    return false;
}

bool test_create_should_create() {
    AmountSet new = asCreate((CopyASElement) copyTestElement, (FreeASElement) freeTestElement,
                             (CompareASElements) compareTestElement);
    if (new) {
        free((new));
        return true;
    }
    return false;
}

bool test_getSize(AmountSet as, int size) {
    if (asGetSize(as) == size) {
        return true;
    }
    return false;
}

bool test_asContains_should_not_have(AmountSet as, TestElement element) {
    if (asContains(as, element) == false) {
        return true;
    }
    return false;
}

bool test_asRegister_should_add(AmountSet as, TestElement element) {
    asRegister(as, element);
    return asContains(as, element);
}


bool test_asChangeAmount_should_change(AmountSet as, TestElement element, double amount) {
    double am;
    double am2;
    asGetAmount(as, element, &am);
    if (asChangeAmount(as, element, amount) == AS_SUCCESS) {
        asGetAmount(as, element, &am2);
        return (am2 - am == amount);
    }
    return false;
}

bool test_asChangeAmount_should_not_change(AmountSet as, TestElement element, double amount) {
    double am;
    double am2;
    asGetAmount(as, element, &am);
    if (asChangeAmount(as, element, amount) == AS_INSUFFICIENT_AMOUNT) {
        asGetAmount(as, element, &am2);
        return (am == am2);
    }
    return false;
}

bool test_asDelete_should_delete(AmountSet as, TestElement element) {
    asDelete(as, element);
    if (asContains(as, element)) {
        return false;
    }
    return true;
}

bool test_asDelete_should_AS_ITEM_DOES_NOT_EXIST(AmountSet as, TestElement element) {
    return (asDelete(as, element) == AS_ITEM_DOES_NOT_EXIST);
}


bool test_set_order(AmountSet as) {
    int prev;
    TestElement element = asGetFirst(as);
    if(element == NULL) {
        return true;
    }
    prev = element->x;
    while (element != NULL) {
        element = asGetNext(as);
        if (element == NULL) {
            return true;
        }
        if (element->x < prev) {
            return false;
        }
    }
    return true;
}

bool test_set_copy_should_work(AmountSet as) {
    AmountSet as_copy = asCopy(as);
    if (as_copy == NULL) {
        return false;
    }
    if (asGetSize(as) != (asGetSize(as_copy))) {
        asDestroy(as_copy);
        return false;
    }

    if (asGetFirst(as) == NULL && asGetFirst(as_copy) == NULL) {
        asDestroy(as_copy);
        return true;
    }

    if (!compareTestElement(asGetFirst(as), asGetFirst(as_copy))) {
        asDestroy(as_copy);
        return false;
    }

    TestElement as_element = asGetNext(as);
    TestElement as_copy_element = asGetNext(as_copy);

    while (as_copy_element != NULL && as_element != NULL) {
        if (!compareTestElement(as_copy_element, as_element)) {
            asDestroy(as_copy);
            return false;
        }
        as_element = asGetNext(as);
        as_copy_element = asGetNext(as_copy);
    }
    asDestroy(as_copy);
    return true;
}

#define RUN_TEST(x, ...) do {\
 printf(#x "(" #__VA_ARGS__ ")");\
 printf(x(__VA_ARGS__) ? " [OK]" : "[FAIL]"); \
 printf("\n\n");\
} while(0)

#define RUN_TEST_NO_ARGS(x) do {\
 printf(#x "()");\
 printf(x() ? " [OK]" : "[FAIL]"); \
 printf("\n\n");\
} while(0)

// Run tests here
int main() {
    RUN_TEST_NO_ARGS(test_create_should_return_null);
    RUN_TEST_NO_ARGS(test_create_should_create);

    AmountSet as_test = asCreate((CopyASElement) copyTestElement, (FreeASElement) freeTestElement,
                                 (CompareASElements) compareTestElement);

    RUN_TEST(test_getSize, NULL, -1);
    RUN_TEST(test_getSize, as_test, 0);
    RUN_TEST(test_set_copy_should_work, as_test);

    TestElement element1 = malloc(sizeof(*element1));
    element1->x = 3;
    element1->y = 1;

    TestElement element2 = malloc(sizeof(*element2));
    element2->x = 1;
    element2->y = 1;

    TestElement element3 = malloc(sizeof(*element3));
    element3->x = 8;
    element3->y = 1;

    RUN_TEST(test_asContains_should_not_have, as_test, (ASElement) element1);
    RUN_TEST(test_asRegister_should_add, as_test, (ASElement) element1);
    RUN_TEST(test_getSize, as_test, 1);

    RUN_TEST(test_asRegister_should_add, as_test, (ASElement) element2);
    RUN_TEST(test_getSize, as_test, 2);
    RUN_TEST(test_asRegister_should_add, as_test, (ASElement) element3);
    RUN_TEST(test_getSize, as_test, 3);

    RUN_TEST(test_asChangeAmount_should_change, as_test, element1, 5);
    RUN_TEST(test_asChangeAmount_should_change, as_test, element1, 0);
    RUN_TEST(test_asChangeAmount_should_not_change, as_test, element1, -54);

    RUN_TEST(test_set_order, as_test);

    RUN_TEST(test_asDelete_should_delete, as_test, element1);
    RUN_TEST(test_getSize, as_test, 2);
    RUN_TEST(test_set_order, as_test);
    RUN_TEST(test_asDelete_should_delete, as_test, element2);
    RUN_TEST(test_getSize, as_test, 1);
    RUN_TEST(test_set_order, as_test);
    RUN_TEST(test_asDelete_should_delete, as_test, element3);
    RUN_TEST(test_getSize, as_test, 0);
    RUN_TEST(test_set_order, as_test);
    RUN_TEST(test_asDelete_should_AS_ITEM_DOES_NOT_EXIST, as_test,element1);

    asDestroy(as_test);
    free(element1);
    free(element2);
    free(element3);

    return 0;
}
