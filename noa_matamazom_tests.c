#include <stdbool.h>
#include <string.h>
#include "matamazom.h"
#include <stdlib.h>
#include <stdio.h>

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

MtmProductData copyStringData(MtmProductData data)
{
    const char *str_to_copy = (const char*)data;
    char *copy = (char*)malloc(strlen(str_to_copy));
    strcpy(copy, str_to_copy);
    return copy;
}

bool compare_inventory(Matamazom matamazom, const char *compare_to)
{
    char buffer[1024*10] = {0};
    FILE* output = fmemopen(buffer, 1024*10, "w");
    mtmPrintInventory(matamazom, output);
    fclose(output);
    return 0 == strcmp(buffer, compare_to);
}

bool compare_order(Matamazom matamazom, unsigned int order, const char *compare_to)
{
    char buffer[1024*10] = {0};
    FILE* output = fmemopen(buffer, 1024*10, "w");
    mtmPrintOrder(matamazom,order, output);
    fclose(output);
    return 0 == strcmp(buffer, compare_to);
}

bool compare_best(Matamazom matamazom, const char *compare_to)
{
    char buffer[1024*10] = {0};
    FILE* output = fmemopen(buffer, 1024*10, "w");
    mtmPrintBestSelling(matamazom, output);
    fclose(output);
    return 0 == strcmp(buffer, compare_to);
}

bool test_create_matamzom_should_fail(){
    Matamazom mtm = matamazomCreate();
    bool result = mtm != NULL;
    matamazomDestroy(mtm);
    return result;
}

bool test_destroy_null(){
    matamazomDestroy(NULL);
    return true;
}

bool test_add_new_product_null(Matamazom mtm){
    char* extra_data = "nonono";
    MatamazomResult result = mtmNewProduct(NULL, 1, "product", 1, MATAMAZOM_ANY_AMOUNT, extra_data,
                                           copyStringData, free, (MtmGetProductPrice) free);
    if (result != MATAMAZOM_NULL_ARGUMENT){
        return false;
    }

    result = mtmNewProduct(mtm, 1, "product", 1, MATAMAZOM_ANY_AMOUNT, NULL, copyStringData, free, (MtmGetProductPrice) free);
    if (result != MATAMAZOM_NULL_ARGUMENT){
        return false;
    }

    result = mtmNewProduct(mtm, 1, "product", 1, MATAMAZOM_ANY_AMOUNT, extra_data, NULL, free,
                           (MtmGetProductPrice) free);
    if (result != MATAMAZOM_NULL_ARGUMENT){
        return false;
    }

    result = mtmNewProduct(mtm, 1, "product", 1, MATAMAZOM_ANY_AMOUNT, extra_data, copyStringData, NULL,
                           (MtmGetProductPrice) free);
    if (result != MATAMAZOM_NULL_ARGUMENT){
        return false;
    }


    result = mtmNewProduct(mtm, 1, "product", 1, MATAMAZOM_ANY_AMOUNT, extra_data, copyStringData, free, NULL);
    if (result != MATAMAZOM_NULL_ARGUMENT){
        return false;
    }
    return true;
}

bool add_new_product_ahould_add(Matamazom matamazom, const unsigned int id, const char *name,
                                const double amount, const MatamazomAmountType amountType,
                                const MtmProductData customData, MtmCopyData copyData,
                                MtmFreeData freeData, MtmGetProductPrice prodPrice){
    MatamazomResult result = mtmNewProduct(matamazom, id, name, amount,amountType, customData, copyData, freeData, prodPrice);
    return MATAMAZOM_SUCCESS == result;
}


bool add_new_product_same_id(Matamazom matamazom, const unsigned int id, const char *name,
                                const double amount, const MatamazomAmountType amountType,
                                const MtmProductData customData, MtmCopyData copyData,
                                MtmFreeData freeData, MtmGetProductPrice prodPrice){
    MatamazomResult result = mtmNewProduct(matamazom, id, name, amount,amountType, customData, copyData, freeData, prodPrice);
    return MATAMAZOM_PRODUCT_ALREADY_EXIST == result;
}

bool add_new_product_invalid_name(Matamazom matamazom, const unsigned int id, const char *name,
                             const double amount, const MatamazomAmountType amountType,
                             const MtmProductData customData, MtmCopyData copyData,
                             MtmFreeData freeData, MtmGetProductPrice prodPrice){
    MatamazomResult result = mtmNewProduct(matamazom, id, "", amount,amountType, customData, copyData, freeData, prodPrice);
    if (MATAMAZOM_INVALID_NAME != result){
        return false;
    }
    result = mtmNewProduct(matamazom, id, "%vrev", amount,amountType, customData, copyData, freeData, prodPrice);
    if (MATAMAZOM_INVALID_NAME != result){
        return false;
    }
    return true;
}


bool add_new_product_invalid_amount(Matamazom matamazom, const unsigned int id, const char *name,
                                  const double amount, const MatamazomAmountType amountType,
                                  const MtmProductData customData, MtmCopyData copyData,
                                  MtmFreeData freeData, MtmGetProductPrice prodPrice){
    MatamazomResult result = mtmNewProduct(matamazom, id, name, -5,amountType, customData, copyData, freeData, prodPrice);
    if (MATAMAZOM_INVALID_AMOUNT != result){
        return false;
    }
    result = mtmNewProduct(matamazom, id, name, 5.4,MATAMAZOM_HALF_INTEGER_AMOUNT, customData, copyData, freeData, prodPrice);
    if (MATAMAZOM_INVALID_AMOUNT != result){
        return false;
    }
    return true;
}

bool change_product_amount_should_change(Matamazom matamazom, const unsigned int id, const double amount){
    MatamazomResult result = mtmChangeProductAmount(matamazom, id, amount);
    return MATAMAZOM_SUCCESS == result;
}


bool change_product_amount_MATAMAZOM_INVALID_AMOUNT(Matamazom matamazom, const unsigned int id, const double amount){
    MatamazomResult result = mtmChangeProductAmount(matamazom, id, amount);
    return MATAMAZOM_INVALID_AMOUNT == result;
}


bool change_product_amount_should_not_change(Matamazom matamazom, const unsigned int id, const double amount){
    MatamazomResult result = mtmChangeProductAmount(NULL, id, amount);
    if (MATAMAZOM_NULL_ARGUMENT != result){
        return false;
    }
    result = mtmChangeProductAmount(matamazom, -1, amount);
    if (MATAMAZOM_PRODUCT_NOT_EXIST != result){
        return false;
    }
    result = mtmChangeProductAmount(matamazom, -1, amount);
    if (MATAMAZOM_PRODUCT_NOT_EXIST != result){
        return false;
    }
    result = mtmChangeProductAmount(matamazom, -1, 0);
    if (MATAMAZOM_PRODUCT_NOT_EXIST != result){
        return false;
    }

    result = mtmChangeProductAmount(matamazom, id, 3.421);
    if (MATAMAZOM_INVALID_AMOUNT != result){
        return false;
    }


    result = mtmChangeProductAmount(matamazom, id, -4314);
    if (MATAMAZOM_INSUFFICIENT_AMOUNT != result){
        return false;
    }
    return true;
}

bool test_remove_product(Matamazom matamazom, const unsigned int id){
    MatamazomResult result = mtmClearProduct(matamazom, id);
    return MATAMAZOM_SUCCESS == result;
}


bool test_remove_should_not_remove(Matamazom matamazom, const unsigned int id){
    MatamazomResult result = mtmClearProduct(NULL, id);
    if (MATAMAZOM_NULL_ARGUMENT != result){
        return false;
    }
    result = mtmClearProduct(matamazom, id);
    if (MATAMAZOM_PRODUCT_NOT_EXIST != result){
        return false;
    }
    return true;
}

bool test_add_new_order_should_fail(){
    unsigned int result = mtmCreateNewOrder(NULL);
    return (result==0);
}

bool test_add_new_order_should_add(Matamazom matamazom){
    unsigned int result = mtmCreateNewOrder(matamazom);
    return (result>=1);
}


bool test_add_product_to_order(Matamazom matamazom, const unsigned int orderId,
                               const unsigned int productId, const double amount){
    MatamazomResult result = mtmChangeProductAmountInOrder(matamazom, orderId, productId, amount);
    return (result==MATAMAZOM_SUCCESS);
}

bool test_add_unknown_product_to_order(Matamazom matamazom, const unsigned int orderId,
                               const unsigned int productId, const double amount){
    MatamazomResult result = mtmChangeProductAmountInOrder(matamazom, orderId, productId, amount);
    return (result==MATAMAZOM_PRODUCT_NOT_EXIST);
}


bool test_add_product_to_unknown_order(Matamazom matamazom, const unsigned int orderId,
                                       const unsigned int productId, const double amount){
    MatamazomResult result = mtmChangeProductAmountInOrder(matamazom, orderId, productId, amount);
    return (result==MATAMAZOM_ORDER_NOT_EXIST);
}


bool test_add_product_to_order_null(Matamazom matamazom, const unsigned int orderId,
                                       const unsigned int productId, const double amount){
    MatamazomResult result = mtmChangeProductAmountInOrder(NULL, orderId, productId, amount);
    return (result==MATAMAZOM_NULL_ARGUMENT);
}


bool test_change_amount_to_product_in_order(Matamazom matamazom, const unsigned int orderId,
                                    const unsigned int productId, const double amount){
    MatamazomResult result = mtmChangeProductAmountInOrder(matamazom, orderId, productId, amount);
    return (result==MATAMAZOM_SUCCESS);
}


bool test_invalid_change_amount_to_product_in_order(Matamazom matamazom, const unsigned int orderId,
                                            const unsigned int productId){

    MatamazomResult result = mtmChangeProductAmountInOrder(matamazom, orderId, productId, 5.2);
    return  (result==MATAMAZOM_INVALID_AMOUNT);
}


bool test_cancel_order_should_cancel(Matamazom matamazom, const unsigned int orderId){

    MatamazomResult result = mtmCancelOrder(matamazom, orderId);
    return  (result==MATAMAZOM_SUCCESS);
}


bool test_cancel_order_null(Matamazom matamazom, const unsigned int orderId){

    MatamazomResult result = mtmCancelOrder(NULL, orderId);
    return  (result==MATAMAZOM_NULL_ARGUMENT);
}


bool test_cancel_order_order_not_exicte(Matamazom matamazom, const unsigned int orderId){

    MatamazomResult result = mtmCancelOrder(matamazom, orderId);
    return  (result==MATAMAZOM_ORDER_NOT_EXIST);
}

bool test_ship_order(Matamazom matamazom, const unsigned int orderId){

    MatamazomResult result = mtmShipOrder(matamazom, orderId);

    return  (result==MATAMAZOM_SUCCESS);
}

bool test_ship_order_cant_ship_MATAMAZOM_INSUFFICIENT_AMOUNT(Matamazom matamazom, const unsigned int orderId){

    MatamazomResult result = mtmShipOrder(matamazom, orderId);

    return  (result==MATAMAZOM_INSUFFICIENT_AMOUNT);
}

bool test_ship_order_cant_ship(Matamazom matamazom, const unsigned int orderId){

    MatamazomResult result = mtmShipOrder(NULL, orderId);
    if (result!=MATAMAZOM_NULL_ARGUMENT){
        return false;
    }
    result = mtmShipOrder(matamazom, 0);
    if (result!=MATAMAZOM_ORDER_NOT_EXIST){
        return false;
    }
    return true;

}


MtmProductData copy_data(MtmProductData data){
    size_t new_data = (size_t) data;
    return (MtmProductData) new_data;
}

void free_int(MtmProductData data){
    //do nothing
}

double get_product_price(MtmProductData data, const double amount){
    return (size_t)data * amount;

}

int main() {
    RUN_TEST_NO_ARGS(test_create_matamzom_should_fail);
    RUN_TEST_NO_ARGS(test_destroy_null);

    Matamazom mtmzom = matamazomCreate();

    RUN_TEST(test_add_new_product_null, mtmzom);

    char* first_ptoduct_name = "first";
    unsigned int first_product_id = 1;
    double first_product_amount = 1;
    MatamazomAmountType first_product_amount_type = MATAMAZOM_INTEGER_AMOUNT;
    MtmProductData first_product_price = (MtmProductData)1;

    RUN_TEST(add_new_product_ahould_add, mtmzom, first_product_id, first_ptoduct_name, first_product_amount,
             first_product_amount_type, first_product_price, copy_data, free_int, get_product_price);
    char* compare_string = "Inventory Status:\n"
                           "name: first, id: 1, amount: 1.000, price: 1.000\n";
    RUN_TEST(compare_inventory, mtmzom, compare_string);

    char* second_ptoduct_name = "2second";
    unsigned int second_product_id = 2;
    double second_product_amount = 2;
    MatamazomAmountType second_product_amount_type = MATAMAZOM_ANY_AMOUNT;
    MtmProductData second_product_price = (MtmProductData)2;

    RUN_TEST(add_new_product_same_id, mtmzom, first_product_id, second_ptoduct_name, second_product_amount,
             second_product_amount_type, second_product_price, copy_data, free_int, get_product_price);
    RUN_TEST(compare_inventory, mtmzom, compare_string);

    RUN_TEST(add_new_product_invalid_name, mtmzom, first_product_id, second_ptoduct_name, second_product_amount,
             second_product_amount_type, second_product_price, copy_data, free_int, get_product_price);
    RUN_TEST(compare_inventory, mtmzom, compare_string);

    RUN_TEST(add_new_product_invalid_amount, mtmzom, first_product_id, second_ptoduct_name, second_product_amount,
             second_product_amount_type, second_product_price, copy_data, free_int, get_product_price);
    RUN_TEST(compare_inventory, mtmzom, compare_string);

    RUN_TEST(change_product_amount_should_change, mtmzom, first_product_id, 10);
    compare_string = "Inventory Status:\n"
                           "name: first, id: 1, amount: 11.000, price: 1.000\n";
    RUN_TEST(compare_inventory, mtmzom, compare_string);

    RUN_TEST(change_product_amount_should_not_change, mtmzom, first_product_id, 3);
    RUN_TEST(compare_inventory, mtmzom, compare_string);

    RUN_TEST(test_remove_product, mtmzom, first_product_id);
    RUN_TEST(test_remove_should_not_remove, mtmzom, first_product_id);
    compare_string = "Inventory Status:\n";
    RUN_TEST(compare_inventory, mtmzom, compare_string);


    RUN_TEST(add_new_product_ahould_add, mtmzom, first_product_id, first_ptoduct_name, first_product_amount,
             first_product_amount_type, first_product_price, copy_data, free_int, get_product_price);

    RUN_TEST(add_new_product_ahould_add, mtmzom, second_product_id, second_ptoduct_name, second_product_amount,
             second_product_amount_type, second_product_price, copy_data, free_int, get_product_price);

    compare_string = "Inventory Status:\n"
                     "name: first, id: 1, amount: 1.000, price: 1.000\n"
                     "name: 2second, id: 2, amount: 2.000, price: 2.000\n";
    RUN_TEST(compare_inventory, mtmzom, compare_string);


    RUN_TEST_NO_ARGS(test_add_new_order_should_fail);
    RUN_TEST(test_add_new_order_should_add, mtmzom);


    int id_order_test = mtmCreateNewOrder(mtmzom);
    compare_string = "Order 2 Details:\n"
                     "----------\n"
                     "Total Price: 0.000\n";
    RUN_TEST(compare_order, mtmzom,id_order_test,compare_string);

    //  If 'amount' > 0 and the product doesn't exist inside
    // the order then you should add it to the order with the amount given as argument
    RUN_TEST(test_add_product_to_order, mtmzom, id_order_test,first_product_id, 0);
    RUN_TEST(compare_order, mtmzom,id_order_test,compare_string);

    RUN_TEST(test_add_unknown_product_to_order, mtmzom, id_order_test,3, 0);
    RUN_TEST(test_add_product_to_unknown_order, mtmzom, -1 ,first_product_id, 0);
    RUN_TEST(test_add_product_to_order_null, mtmzom, -1 ,first_product_id, 0);


    RUN_TEST(test_change_amount_to_product_in_order,mtmzom,id_order_test,first_product_id,5);
    RUN_TEST(test_invalid_change_amount_to_product_in_order,mtmzom,id_order_test,first_product_id);
    RUN_TEST(test_change_amount_to_product_in_order,mtmzom,id_order_test,first_product_id,-5);
    RUN_TEST(test_change_amount_to_product_in_order,mtmzom,id_order_test,first_product_id,5);
    RUN_TEST(test_cancel_order_should_cancel,mtmzom,id_order_test);
    RUN_TEST(test_cancel_order_null,mtmzom,id_order_test);
    RUN_TEST(test_cancel_order_order_not_exicte,mtmzom,id_order_test);

    id_order_test = mtmCreateNewOrder(mtmzom);
    compare_string = "Order 3 Details:\n"
                     "----------\n"
                     "Total Price: 0.000\n";
    RUN_TEST(compare_order, mtmzom,id_order_test,compare_string);

    int product_amount_in_order = 1;
    RUN_TEST(test_add_product_to_order, mtmzom, id_order_test,first_product_id, product_amount_in_order);
    compare_string = "Order 3 Details:\n"
                     "name: first, id: 1, amount: 1.000, price: 1.000\n"
                     "----------\n"
                     "Total Price: 1.000\n";
    RUN_TEST(compare_order, mtmzom,id_order_test,compare_string);

    RUN_TEST(test_add_product_to_order, mtmzom, id_order_test,second_product_id, product_amount_in_order);
    compare_string = "Order 3 Details:\n"
                     "name: first, id: 1, amount: 1.000, price: 1.000\n"
                     "name: 2second, id: 2, amount: 1.000, price: 2.000\n"
                     "----------\n"
                     "Total Price: 3.000\n";
    RUN_TEST(compare_order, mtmzom,id_order_test,compare_string);

    compare_string = "Inventory Status:\n"
                     "name: first, id: 1, amount: 1.000, price: 1.000\n"
                     "name: 2second, id: 2, amount: 2.000, price: 2.000\n";
    RUN_TEST(compare_inventory, mtmzom, compare_string);

    RUN_TEST(test_ship_order, mtmzom, id_order_test);
    compare_string = "Inventory Status:\n"
                     "name: first, id: 1, amount: 0.000, price: 1.000\n"
                     "name: 2second, id: 2, amount: 1.000, price: 2.000\n";
    RUN_TEST(compare_inventory, mtmzom, compare_string);
    RUN_TEST(test_cancel_order_order_not_exicte,mtmzom,id_order_test);

    id_order_test = mtmCreateNewOrder(mtmzom);
    RUN_TEST(test_ship_order_cant_ship, mtmzom, id_order_test);
    RUN_TEST(test_add_product_to_order, mtmzom, id_order_test,first_product_id, product_amount_in_order);
    RUN_TEST(test_ship_order_cant_ship_MATAMAZOM_INSUFFICIENT_AMOUNT, mtmzom, id_order_test);
    RUN_TEST(compare_inventory, mtmzom, compare_string);
    compare_string = "Order 4 Details:\n"
                     "name: first, id: 1, amount: 1.000, price: 1.000\n"
                     "----------\n"
                     "Total Price: 1.000\n";
    RUN_TEST(compare_order, mtmzom,id_order_test,compare_string);

    compare_string = "Best Selling Product:\n"
                     "name: 2second, id: 2, total income: 2.000\n";
    RUN_TEST(compare_best, mtmzom,compare_string);

    matamazomDestroy(mtmzom);

    // more 2 test 1 facebook 2 delete product and check if order axcicte. if i delete product and then add it back?
    mtmzom = matamazomCreate();
    RUN_TEST(test_cancel_order_order_not_exicte,mtmzom,1);
    first_product_amount = 1.001;
    RUN_TEST(add_new_product_ahould_add, mtmzom, first_product_id, first_ptoduct_name, first_product_amount,
             first_product_amount_type, first_product_price, copy_data, free_int, get_product_price);
    RUN_TEST(change_product_amount_should_change, mtmzom, first_product_id, first_product_amount);
    compare_string = "Inventory Status:\n"
                     "name: first, id: 1, amount: 2.002, price: 1.000\n";
    RUN_TEST(compare_inventory, mtmzom, compare_string);

    id_order_test = mtmCreateNewOrder(mtmzom);

    // רשום בתיעוד בקובץ matamazom.h כי אם אתה נדרש להוריד מהזמנה כמות של מוצר X הגדולה שווה מהכמות הקיימת שלו בהזמנה עליך להסיר את המוצר מההזמנה הזו.
    RUN_TEST(test_add_product_to_order, mtmzom, id_order_test,first_product_id, product_amount_in_order);
    compare_string = "Order 1 Details:\n"
                     "name: first, id: 1, amount: 1.000, price: 1.000\n"
                     "----------\n"
                     "Total Price: 1.000\n";
    RUN_TEST(compare_order, mtmzom,id_order_test, compare_string);
    RUN_TEST(test_add_product_to_order, mtmzom, id_order_test,first_product_id, -product_amount_in_order);
    mtmPrintOrder(mtmzom, id_order_test, stdout);
    compare_string = "Order 1 Details:\n"
                     "----------\n"
                     "Total Price: 0.000\n";
    RUN_TEST(compare_order, mtmzom,id_order_test, compare_string);

    RUN_TEST(test_add_product_to_order, mtmzom, id_order_test,first_product_id, product_amount_in_order);
    mtmPrintOrder(mtmzom, id_order_test, stdout);
    compare_string = "Order 1 Details:\n"
                     "name: first, id: 1, amount: 1.000, price: 1.000\n"
                     "----------\n"
                     "Total Price: 1.000\n";
    RUN_TEST(compare_order, mtmzom,id_order_test, compare_string);
    RUN_TEST(test_remove_product, mtmzom, first_product_id);
    compare_string = "Inventory Status:\n";
    RUN_TEST(compare_inventory, mtmzom, compare_string);
    compare_string = "Order 1 Details:\n"
                     "----------\n"
                     "Total Price: 0.000\n";
    RUN_TEST(compare_order, mtmzom,id_order_test, compare_string);

    matamazomDestroy(mtmzom);

}