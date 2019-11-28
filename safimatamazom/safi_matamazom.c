#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "matamazom.h"

#define INVENTORY_OUT_FILE "C:/mtmhw119/printed_inventory_safi_test.txt"
#define INVENTORY_OUT_FILE3 "C:/mtmhw119/printed_inventory_safi_test3.txt"
#define INVENTORY_OUT_FILE4 "C:/mtmhw119/printed_inventory_safi_test4.txt"
#define ORDER_OUT_FILE "C:/mtmhw119/printed_order_safi_test.txt"
#define ORDER_OUT_FILE3 "C:/mtmhw119/printed_order_safi_test3.txt"
#define BEST_SELLING_OUT_FILE "C:/mtmhw119/printed_best_selling_safitest.txt"
#define BEST_SELLING_OUT_FILE3  "C:/mtmhw119/printed_best_selling_safitest3.txt"
#define FILTERED_OUT_FILE "C:/mtmhw119/printed_filtered_safitest.txt"
#define FILTERED_OUT_FILE3 "C:/mtmhw119/printed_filtered_safitest3.txt"
#define FILTERED_OUT_FILE4 "C:/mtmhw119/printed_filtered_safitest4.txt"

#define TEST_END(test)                   \
        printf( #test " working YAHOOOOoooo\n "); \


#define RUN_TEST(test)                   \
        printf("Running " #test "...\n "); \


static MtmProductData copyDouble(MtmProductData number) {
    double *copy = malloc(sizeof(*copy));
    if (copy) {
        *copy = *(double*)number;
    }
    return copy;
}

static void freeDouble(MtmProductData number) {
    free(number);
}

static double simplePrice(MtmProductData basePrice, const double amount) {
    return (*(double*)basePrice) * amount;
}

static double buy10Get10ForFree(MtmProductData basePrice, const double
amount) {
    double realAmount = amount;
    if (amount >= 20) {
        realAmount -= 10;
    } else if (amount >= 10) {
        realAmount = 10;
    }
    return simplePrice(basePrice, realAmount);
}

static bool isAmountLessThan10(const unsigned int id, const char *name,
                               const double amount, MtmProductData customData) {
    return amount < 10;
}

static bool isNamebiggerThan7(const unsigned int id, const char *name,
                               const double amount, MtmProductData customData) {
    return strlen(name)>7;
}

static bool isAmountModId(const unsigned int id, const char *name,
                              const double amount, MtmProductData customData) {
    return (int)amount%id==0;
}


int main(){

    RUN_TEST(matamazomCreate);
    Matamazom mtm = matamazomCreate();
    assert(mtm);
    TEST_END(matamazomCreate);

    RUN_TEST(matamazomDestroy);
    matamazomDestroy(mtm);
    TEST_END(matamazomDestroy);

    RUN_TEST(mtmNewProduct);
    mtm = matamazomCreate();
    assert(mtm);
    double basePrice=5;

    //checking the MATAMAZOM_NULL_ARGUMENT case:
    assert(MATAMAZOM_NULL_ARGUMENT==mtmNewProduct(NULL,1,"F",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmNewProduct(mtm,1,NULL,1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmNewProduct(mtm,1,"F",1,
            MATAMAZOM_INTEGER_AMOUNT,NULL,copyDouble, freeDouble,simplePrice));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmNewProduct(mtm,1,"F",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,NULL,freeDouble,simplePrice));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmNewProduct(mtm,1,"F",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,NULL,simplePrice));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmNewProduct(mtm,1,"F",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,NULL));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmNewProduct(mtm,1,"F",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,NULL,NULL,NULL));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmNewProduct(NULL,1,"F",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,NULL));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmNewProduct(mtm,1,NULL,1,
            MATAMAZOM_INTEGER_AMOUNT,NULL,copyDouble,freeDouble,simplePrice));

    //checking the MATAMAZOM_INVALID_NAME case:
    assert(MATAMAZOM_INVALID_NAME==mtmNewProduct(mtm,1,"",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_NAME==mtmNewProduct(mtm,1,".bla",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_NAME==mtmNewProduct(mtm,1,":bla",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_NAME==mtmNewProduct(mtm,1,"@bla",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_NAME==mtmNewProduct(mtm,1,"[bla",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_NAME==mtmNewProduct(mtm,1,"^bla",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_NAME==mtmNewProduct(mtm,1,"{bla",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_NAME==mtmNewProduct(mtm,1,"|bla",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));

    //checking the MATAMAZOM_INVALID_AMOUNT case:
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",-12222.001,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",5.0012,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",5.01,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",5.005,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",-3,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",0.990,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",100.991,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",10.997999,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",10.998999999999999,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",2.0010000001,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",0.990,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",5.502,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",5.5010000000000001,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",5.4989999999999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",5.0010000000000001,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmNewProduct(mtm,1,"F",5.9989999999999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));

    //checking the MATAMAZOM_INVALID_AMOUNT case if you got the right limits
    // of amount that can be send :
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,1,"First",5.000999999999,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,2,"Second",3.999000000000000000001,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,5,"cat",5.00099999999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,4,"dog",7.49999999999999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,8,"bed",9.50099999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,9,"shite",9.99900000000000001,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,7,"bed",9.999999999999999999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));

    //checking the MATAMAZOM_PRODUCT_ALREADY_EXIST case:
    assert(MATAMAZOM_PRODUCT_ALREADY_EXIST==mtmNewProduct(mtm,1,"First",5.0009999999999,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_PRODUCT_ALREADY_EXIST==mtmNewProduct(mtm,2,"Second",3.999000000000000000001,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_PRODUCT_ALREADY_EXIST==mtmNewProduct(mtm,5,"cat",5.00099999999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_PRODUCT_ALREADY_EXIST==mtmNewProduct(mtm,4,"dog",7.49999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_PRODUCT_ALREADY_EXIST==mtmNewProduct(mtm,8,"bed",9.5009999999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_PRODUCT_ALREADY_EXIST==mtmNewProduct(mtm,9,"shite",9.99900000000000001,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_PRODUCT_ALREADY_EXIST==mtmNewProduct(mtm,7,"bed",9.999999999999999999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));

    matamazomDestroy(mtm);
    TEST_END(mtmNewProduct);

    RUN_TEST(mtmChangeProductAmount);
    mtm = matamazomCreate();
    assert(mtm);

    //checking the MATAMAZOM_NULL_ARGUMENT case:
    assert(MATAMAZOM_NULL_ARGUMENT==mtmChangeProductAmount(NULL,1,3));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmChangeProductAmount(NULL,1,0));

    //checking the MATAMAZOM_PRODUCT_NOT_EXIST case:
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmount(mtm,1,0));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmount(mtm,1,3));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmount(mtm,7,0));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmount(mtm,5,3));
            //insert new products to play with:
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,1,"First",5.0009999999999,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,2,"Second",3.999000000000000000001,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,5,"cat",5.000999999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,4,"dog",7.49999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,8,"bed",9.5009999999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,9,"shite",9.99900000000000001,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,7,"bed",9.999999999999999999999,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,19,"Nar",25.245,
            MATAMAZOM_ANY_AMOUNT,&basePrice,copyDouble, freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,17,"Ya3me",77.045699999,
            MATAMAZOM_ANY_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,18,"Narrrr",567.709999999,
            MATAMAZOM_ANY_AMOUNT,&basePrice,copyDouble,freeDouble,simplePrice));

    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmount(mtm,0,3));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmount(mtm,3,3));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmount(mtm,6,3));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmount(mtm,10,3));

    //checking the MATAMAZOM_INSUFFICIENT_AMOUNT case:
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmChangeProductAmount(mtm,1,-6));
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmChangeProductAmount(mtm,2,-4));
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmChangeProductAmount(mtm,5,-10));
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmChangeProductAmount(mtm,8,-80));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmount(mtm,1,-3));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmount(mtm,1,-2));
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmChangeProductAmount(mtm,1,-1));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmount(mtm,8,-6));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmount(mtm,8,-2));
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmChangeProductAmount(mtm,8,-2));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmount(mtm,4,-7.4999999));   //amount=0
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmChangeProductAmount(mtm,4,-7.4999999));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmount(mtm,4,0));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmount(mtm,4,100));

    //checking the MATAMAZOM_INVALID_AMOUNT case:
    //MATAMAZOM_INTEGER_AMOUNT:
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,1,-6.001000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,1,6.0010000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,1,6.998999999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,1,-6.998999999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,1,5.5));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,1,5.888889999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,1,5.34));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,1,0.00100000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,1,-0.001000000001));
    //MATAMAZOM_HALF_INTEGER_AMOUNT:
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,5,-6.001000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,5,6.0010000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,5,6.998999999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,5,-6.998999999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,5,5.501999999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,5,5.888889999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,5,5.34));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,5,0.00100000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,5,-0.001000000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,5,5.4980001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,5,5.49899999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmount(mtm,5,5.501009999));

    TEST_END(mtmChangeProductAmount);

    RUN_TEST(mtmClearProduct);
    //checking the MATAMAZOM_NULL_ARGUMENT case:
    assert(MATAMAZOM_NULL_ARGUMENT==mtmClearProduct(NULL,1));

    //checking the MATAMAZOM_PRODUCT_NOT_EXIST case:
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmClearProduct(mtm,12));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmClearProduct(mtm,13));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmClearProduct(mtm,14));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmClearProduct(mtm,15));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmClearProduct(mtm,16));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmClearProduct(mtm,3));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmClearProduct(mtm,6));

    //checking the MATAMAZOM_SUCCESS case:
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,8));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmClearProduct(mtm,8));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmount(mtm,8,3));
    unsigned int order=mtmCreateNewOrder(mtm);
    assert(0!=order);
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,1,3));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,1));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmount(mtm,1,3));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmountInOrder(mtm,order,1,3));
    assert(MATAMAZOM_SUCCESS==mtmCancelOrder(mtm,order));
    //products:{2,4,5,7,9,17,18,19} in the matamazom
    TEST_END(mtmClearProduct);

    RUN_TEST(mtmCreateNewOrder);
    //checking the 0==NULL case:
    unsigned int order2=mtmCreateNewOrder(NULL);
    assert(order2==0);

    //create orders to have some fun:
    order2=mtmCreateNewOrder(mtm);
    unsigned int order3=mtmCreateNewOrder(mtm);
    unsigned int order4=mtmCreateNewOrder(mtm);
    unsigned int order5=mtmCreateNewOrder(mtm);
    unsigned int order6=mtmCreateNewOrder(mtm);
    unsigned int order7=mtmCreateNewOrder(mtm);
    unsigned int order8=mtmCreateNewOrder(mtm);
    unsigned int orderX=mtmCreateNewOrder(mtm);
    assert(order2!=0);
    assert(order3!=0);
    assert(order4!=0);
    assert(order5!=0);
    assert(order6!=0);
    assert(order7!=0);
    assert(order8!=0);
    assert(orderX!=0);

    TEST_END(mtmCreateNewOrder);
    //products:{2(int),4(half),5,7,9(half),17(any),18,19} in the matamazom
    RUN_TEST(mtmChangeProductAmountInOrder);
    //checking the MATAMAZOM_NULL_ARGUMENT case:
    assert(MATAMAZOM_NULL_ARGUMENT==mtmChangeProductAmountInOrder(NULL,order2,1,1211));

    //checking the MATAMAZOM_ORDER_NOT_EXIST case:
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmChangeProductAmountInOrder(mtm, order,2,1211));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,2,1211));
    assert(MATAMAZOM_SUCCESS==mtmCancelOrder(mtm,order2));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmChangeProductAmountInOrder(mtm,order2,2,12));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm, order3,2,1211));
    assert(MATAMAZOM_SUCCESS==mtmCancelOrder(mtm,order3));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmChangeProductAmountInOrder(mtm, order3,2,1211));

    //checking the MATAMAZOM_PRODUCT_NOT_EXIST case:
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmountInOrder(mtm,order4,1,1211));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmountInOrder(mtm,order4,8,1211));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmountInOrder(mtm,order4,10,1211));
    assert(MATAMAZOM_PRODUCT_NOT_EXIST==mtmChangeProductAmountInOrder(mtm,order4,11,1211));

    //checking the MATAMAZOM_INVALID_AMOUNT case:
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,2,1211.00123));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,2,-1211.00123));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,2,1.001000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,2,-1.001000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,2,1.99899999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,2,-1.99899999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,1.99899999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,1211.00123));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,-1211.00123));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,1.001000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,-1.001000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,1.99899999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,-1.99899999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,-1.498999999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,1.498999999));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,1.4981101));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,-1.4981101));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,1.5010000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,-1.5010000001));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,1.501234));
    assert(MATAMAZOM_INVALID_AMOUNT==mtmChangeProductAmountInOrder(mtm,order4,5,-1.501234));

    //checking the MATAMAZOM_SUCCESS case:
    //half-int
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,-1.501));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,-1.501));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,-20));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,-30));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,3.5));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,-1.501));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,3.501));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,3.001));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,3.50022));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,3.499));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,3.4992344));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,3.999));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,3.9991234));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,-3.501));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,-3.001));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,-3.50022));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,-3.499));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,-3.4992344));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,-3.999));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,5,-3.9991234));
    //any
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-1.501));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-1.501));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-20));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-30));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,3.5));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-1.501));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,3.501));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,3.001));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,3.50022));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,3.499));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,3.4992344));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,3.999));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,3.9991234));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-3.501));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-3.001));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-3.50022));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-3.499));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-3.4992344));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-3.999));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-3.9991234));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-3.485443));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,3.485443));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,3485443));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,-30000.485443));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,355.11111111));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,17,355.77778));
    //int
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,2,355));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,2,355.001));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,2,-355.001));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,2,-355));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,2,355.000123));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,2,355.999));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,2,-35.999));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,2,-35.000123));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,2,355.999123));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order4,2,-35.999123));

    TEST_END(mtmChangeProductAmountInOrder);

    RUN_TEST(mtmShipOrder);
    //checking the MATAMAZOM_NULL_ARGUMENT case:
    assert(MATAMAZOM_NULL_ARGUMENT==mtmShipOrder(NULL,order4));

    //checking the MATAMAZOM_ORDER_NOT_EXIST case:
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmShipOrder(mtm,order));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmShipOrder(mtm,order2));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmShipOrder(mtm,order3));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmShipOrder(mtm,11));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmShipOrder(mtm,10));
    assert(MATAMAZOM_SUCCESS==mtmCancelOrder(mtm,order8));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmShipOrder(mtm,8));

    //checking the MATAMAZOM_INSUFFICIENT_AMOUNT case:
    //int:
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order5,2,100));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order6,2,200));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order7,2,300));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmount(mtm,2,500));
    assert(MATAMAZOM_SUCCESS==mtmShipOrder(mtm,order5));  //pro amount 403.999000000000000000001
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmShipOrder(mtm,order5));  //pro amount 403.999000000000000000001
    assert(MATAMAZOM_SUCCESS==mtmShipOrder(mtm,order6));  //proamount 203.999000000000000000001
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmShipOrder(mtm,order6));
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmShipOrder(mtm,order7));  //proamount 203.999000000000000000001
    //half-int :
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order7,7,300));
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmShipOrder(mtm,order7));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmount(mtm,2,500));//proamount    503.99900..1
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmShipOrder(mtm,order7));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmount(mtm,7,500));//509.99999...
    assert(MATAMAZOM_SUCCESS==mtmShipOrder(mtm,order7));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmShipOrder(mtm,order7));
    //any :25.245 567.709999999
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,orderX,19,3000));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,orderX,18,3000));
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmShipOrder(mtm,orderX));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,orderX,18,-2700));
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmShipOrder(mtm,orderX));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,orderX,19,-2700));
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmShipOrder(mtm,orderX));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmount(mtm,19,500));
    assert(MATAMAZOM_SUCCESS==mtmShipOrder(mtm,orderX));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmShipOrder(mtm,orderX));

    TEST_END(mtmShipOrder);

    RUN_TEST(mtmCancelOrder);
    //checking the MATAMAZOM_NULL_ARGUMENT case:
    assert(MATAMAZOM_NULL_ARGUMENT==mtmCancelOrder(NULL,2));

    //checking the MATAMAZOM_ORDER_NOT_EXIST case:
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmCancelOrder(mtm,order));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmCancelOrder(mtm,order2));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmCancelOrder(mtm,order3));
    assert(MATAMAZOM_SUCCESS==mtmCancelOrder(mtm,order4));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmCancelOrder(mtm,order4));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmCancelOrder(mtm,order5));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmCancelOrder(mtm,order6));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmCancelOrder(mtm,order7));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmCancelOrder(mtm,order8));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmCancelOrder(mtm,orderX));

    //checking the MATAMAZOM_SUCCESS case:
    //if you get here then the function works so no need to test it already
    // you have done it :-)

    TEST_END(mtmCancelOrder);

    matamazomDestroy(mtm);

    RUN_TEST(mtmPrintInventory);
    mtm=matamazomCreate();
    assert(mtm);

    basePrice = 8.9;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm, 4, "Tomato", 2019.11,
            MATAMAZOM_ANY_AMOUNT,&basePrice, copyDouble, freeDouble, simplePrice));
    basePrice = 5.8;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm, 6, "Onion", 1789.75,
            MATAMAZOM_ANY_AMOUNT,&basePrice, copyDouble, freeDouble, buy10Get10ForFree));
    basePrice = 2000;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm, 10, "Television", 15,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 5000;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm, 11, "Smart TV", 4,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 18.5;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm, 7, "Watermelon", 24.5,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 100;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm, 1, "Glass",30,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 1500;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm, 8, "Xbox",5,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 1000;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm, 13, "Playstation",9,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 5.35;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm, 19, "Carrot",4.356,
            MATAMAZOM_ANY_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 7.35;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm, 17, "eggplant",2.345,
            MATAMAZOM_ANY_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 20;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,3, "olives",55.7889,
            MATAMAZOM_ANY_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 20;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,9, "beer",1.5,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 40;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,12,"Pack of cigarettes",8,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 110;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,14,"Weed",29.5,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 120;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,15,"Vodka",60,
            MATAMAZOM_HALF_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 45;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,21,"Tickets",40,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 45000.000000001;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,28,"car",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 45000;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,25,"home",1,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));
    basePrice = 350;
    assert(MATAMAZOM_SUCCESS==mtmNewProduct(mtm,24,"Surf Board",5,
            MATAMAZOM_INTEGER_AMOUNT,&basePrice,copyDouble, freeDouble, simplePrice));


    FILE *outputFile = fopen(INVENTORY_OUT_FILE, "w");
    assert(outputFile);
    //checking the MATAMAZOM_NULL_ARGUMENT case:
    assert(MATAMAZOM_NULL_ARGUMENT==mtmPrintInventory(NULL,outputFile));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmPrintInventory(mtm,NULL));
    assert(MATAMAZOM_SUCCESS==mtmPrintInventory(mtm,outputFile));
    fclose(outputFile);



    TEST_END(mtmPrintInventory);

    RUN_TEST(mtmPrintOrder);

    order=mtmCreateNewOrder(mtm);
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,1,400));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,1,400));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,3,400));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,3,-400));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,3,14.56));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,4,142.56));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,4,-120.5));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,6,20.5));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,6,0.5));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,6,0));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,7,0));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,7,34.501));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,8,0));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,8,34.001));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,9,0));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,9,34.001));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,9,-4));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,10,34.001));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,10,34.999));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,11,91.0009));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,11,-3));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,11,-0.001));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,11,0));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,12,66));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order,12,-5));

    //checking the MATAMAZOM_NULL_ARGUMENT case:
    outputFile = fopen(ORDER_OUT_FILE, "w");
    assert(outputFile);
    assert(MATAMAZOM_NULL_ARGUMENT==mtmPrintOrder(mtm,order,NULL));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmPrintOrder(NULL,order,outputFile));

    //checking the MATAMAZOM_ORDER_NOT_EXIST case:
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmPrintOrder(mtm,order2,outputFile));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmPrintOrder(mtm,order3,outputFile));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmPrintOrder(mtm,order4,outputFile));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmPrintOrder(mtm,order5,outputFile));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmPrintOrder(mtm,order6,outputFile));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmPrintOrder(mtm,order7,outputFile));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmPrintOrder(mtm,order8,outputFile));
    assert(MATAMAZOM_ORDER_NOT_EXIST==mtmPrintOrder(mtm,orderX,outputFile));

    //printe the order:
    assert(MATAMAZOM_SUCCESS==mtmPrintOrder(mtm,order,outputFile));
    fclose(outputFile);




    TEST_END(mtmPrintOrder);

    RUN_TEST(mtmPrintBestSelling);
    outputFile = fopen(BEST_SELLING_OUT_FILE, "w");
    FILE* outputFile3 = fopen(BEST_SELLING_OUT_FILE3, "w");
    assert(outputFile);
    assert(outputFile3);
    order2=mtmCreateNewOrder(mtm);
    assert(order2!=0);
    assert(MATAMAZOM_INSUFFICIENT_AMOUNT==mtmShipOrder(mtm,order));
    //checking the MATAMAZOM_NULL_ARGUMENT case:
    assert(MATAMAZOM_NULL_ARGUMENT==mtmPrintBestSelling(mtm,NULL));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmPrintBestSelling(NULL,NULL));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmPrintBestSelling(NULL,outputFile));

    //print:
    assert(MATAMAZOM_SUCCESS==mtmPrintBestSelling(mtm,outputFile));
    fclose(outputFile);

    //number 3:
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,1,20));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,3,40));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,4,400));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,6,400));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,7,24));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,8,2));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,9,0));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,10,5));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,11,3));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,12,6));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,13,9));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,14,25));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,15,55));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,17,2));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,19,3));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,21,30));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,24,3));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,25,1));
    assert(MATAMAZOM_SUCCESS==mtmChangeProductAmountInOrder(mtm,order2,28,1));
    assert(MATAMAZOM_SUCCESS==mtmShipOrder(mtm,order2));
    assert(MATAMAZOM_SUCCESS==mtmPrintBestSelling(mtm,outputFile3));
    //updated inventory:
    outputFile3 = fopen(INVENTORY_OUT_FILE3, "w");
    assert(outputFile3);
    assert(MATAMAZOM_SUCCESS==mtmPrintInventory(mtm,outputFile3));
    fclose(outputFile3);

    TEST_END(mtmPrintBestSelling);

    //checking the MATAMAZOM_NULL_ARGUMENT case:
    RUN_TEST(mtmPrintFiltered);
    outputFile = fopen(FILTERED_OUT_FILE, "w");
    outputFile3 = fopen(FILTERED_OUT_FILE3, "w");
    assert(outputFile);
    assert(outputFile3);
    //checking the MATAMAZOM_NULL_ARGUMENT case:
    assert(MATAMAZOM_NULL_ARGUMENT==mtmPrintFiltered(mtm,NULL,NULL));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmPrintFiltered(mtm,isAmountLessThan10,NULL));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmPrintFiltered(NULL,isAmountLessThan10,outputFile));
    assert(MATAMAZOM_NULL_ARGUMENT==mtmPrintFiltered(mtm,NULL,outputFile));

    //print:
    assert(MATAMAZOM_SUCCESS==mtmPrintFiltered(mtm,isAmountLessThan10,outputFile));
    assert(MATAMAZOM_SUCCESS==mtmPrintFiltered(mtm,isNamebiggerThan7,outputFile3));
    fclose(outputFile);
    fclose(outputFile3);
    outputFile3 = fopen(FILTERED_OUT_FILE4, "w");
    assert(MATAMAZOM_SUCCESS==mtmPrintFiltered(mtm,isAmountModId,outputFile3));
    fclose(outputFile3);

    TEST_END(mtmPrintFiltered);

    //last test for print inventory and order:
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,1));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,3));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,4));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,6));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,7));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,8));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,9));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,10));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,11));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,12));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,13));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,14));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,15));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,17));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,19));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,21));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,24));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,25));
    assert(MATAMAZOM_SUCCESS==mtmClearProduct(mtm,28));
    outputFile3 = fopen(INVENTORY_OUT_FILE4, "w");
    assert(outputFile3);
    assert(MATAMAZOM_SUCCESS==mtmPrintInventory(mtm,outputFile3));
    fclose(outputFile3);
    order=mtmCreateNewOrder(mtm);
    assert(order!=0);
    outputFile3 = fopen(ORDER_OUT_FILE3, "w");
    assert(outputFile3);
    assert(MATAMAZOM_SUCCESS==mtmPrintOrder(mtm,order,outputFile3));
    fclose(outputFile3);

    matamazomDestroy(mtm);
    printf("\n");
    printf("\n");
    printf("\n");
    printf("############################################################################\n");
    printf("############################################################################\n");
    printf("############################################################################\n");
    printf("##############################congratulations!##############################\n");
    printf("############################################################################\n");
    printf("############################################################################\n");

    return 0;
}
