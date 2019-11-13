#include "../amount_set.h"
#include "amount_set_tests.h"
#include "test_utilities.h"
#include <stdlib.h>

#define ASSERT_OR_DESTROY(expr) ASSERT_TEST_WITH_FREE((expr), asDestroy(set))

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

bool testCreate() {
    AmountSet set = asCreate(copyInt, freeInt, compareInts);
    ASSERT_OR_DESTROY(set);
    asDestroy(set);
    return true;
}

bool testDestroy() {
    AmountSet set = asCreate(copyInt, freeInt, compareInts);
    asDestroy(set);
    /* try to elicit undefined behavior in case asDestroy has bugs */
    AmountSet set2 = asCreate(copyInt, freeInt, compareInts);
    ASSERT_TEST(set2 != NULL);
    asDestroy(set2);
    return true;
}

bool testModify() {
    AmountSet set = asCreate(copyInt, freeInt, compareInts);
    int ids[7] = {0, 4, 2, 5, 7, 3, 1};
    for (int i = 0; i < 7; ++i) {
        ASSERT_OR_DESTROY(asRegister(set, ids + i) == AS_SUCCESS);
    }

    ASSERT_OR_DESTROY(asChangeAmount(set, ids + 2, 10.5) == AS_SUCCESS);
    ASSERT_OR_DESTROY(asChangeAmount(set, ids + 1, 3) == AS_SUCCESS);
    ASSERT_OR_DESTROY(asChangeAmount(set, ids + 1, -17) == AS_INSUFFICIENT_AMOUNT);
    ASSERT_OR_DESTROY(asDelete(set, ids + 3) == AS_SUCCESS);

    asDestroy(set);
    return true;
}

static void addElements(AmountSet set) {
    int ids[7] = {0, 4, 2, 5, 7, 3, 1};
    for (int i = 0; i < 7; ++i) {
        asRegister(set, ids + i);
    }
    asChangeAmount(set, ids + 2, 10.5);
    asChangeAmount(set, ids + 1, 3);
}

bool testModify2() {
    AmountSet set = asCreate(copyInt, freeInt, compareInts);
    addElements(set);
    ASSERT_OR_DESTROY(asClear(set) == AS_SUCCESS);
    int x = 8, y = 3;
    ASSERT_OR_DESTROY(asRegister(set, &x) == AS_SUCCESS);
    ASSERT_OR_DESTROY(asRegister(set, &y) == AS_SUCCESS);
    ASSERT_OR_DESTROY(asClear(set) == AS_SUCCESS);
    asDestroy(set);
    return true;
}

static void destroyAmountSets(AmountSet set1, AmountSet set2) {
    asDestroy(set1);
    asDestroy(set2);
}

bool testCopy() {
    AmountSet set = asCreate(copyInt, freeInt, compareInts);
    addElements(set);
    AmountSet copy = asCopy(set);
    ASSERT_OR_DESTROY(copy != NULL);
    int x = 2;
    ASSERT_TEST_WITH_FREE(asDelete(copy, &x) == AS_SUCCESS, destroyAmountSets(set, copy));
    ASSERT_TEST_WITH_FREE(asContains(set, &x), destroyAmountSets(set, copy));
    asDestroy(copy);
    asDestroy(set);
    return true;
}

bool testGetSize() {
    AmountSet set = asCreate(copyInt, freeInt, compareInts);
    addElements(set);
    ASSERT_OR_DESTROY(asGetSize(set) == 7);
    ASSERT_OR_DESTROY(asGetSize(NULL) == -1);
    asDestroy(set);
    return true;
}

bool testContains() {
    AmountSet set = asCreate(copyInt, freeInt, compareInts);
    addElements(set);
    int x = 3;
    ASSERT_OR_DESTROY(asContains(set, &x));
    x = 30;
    ASSERT_OR_DESTROY(!asContains(set, &x));
    asDestroy(set);
    return true;
}

bool testGetAmount() {
    AmountSet set = asCreate(copyInt, freeInt, compareInts);
    addElements(set);
    double amount = -1.0;
    int x = 2;
    ASSERT_OR_DESTROY(asGetAmount(set, &x, &amount) == AS_SUCCESS);
    ASSERT_OR_DESTROY(-0.001 < amount - 10.5 && amount - 10.5 < 0.001);
    x = 30;
    ASSERT_OR_DESTROY(asGetAmount(set, &x, &amount) == AS_ITEM_DOES_NOT_EXIST);
    asDestroy(set);
    return true;
}

bool testIteration() {
    AmountSet set = asCreate(copyInt, freeInt, compareInts);
    addElements(set);
    int lastId = -1;
    AS_FOREACH(int*, currId, set) {
        ASSERT_OR_DESTROY(lastId < (*currId));
    }
    asDestroy(set);
    return true;
}
