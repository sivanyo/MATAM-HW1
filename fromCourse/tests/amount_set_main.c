#include "amount_set_tests.h"
#include "test_utilities.h"

int main() {
    RUN_TEST(testCreate);
    RUN_TEST(testModify);
    RUN_TEST(testModify2);
    RUN_TEST(testDestroy);
    RUN_TEST(testCopy);
    RUN_TEST(testGetSize);
    RUN_TEST(testContains);
    RUN_TEST(testGetAmount);
    RUN_TEST(testIteration);
    return 0;
}
