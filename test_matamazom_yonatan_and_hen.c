//
// Created by yonatan on 11/18/19.
//
#include <stdio.h>
#include <stdlib.h>
#include "matamazom.h"
#include <string.h>
#include <assert.h>
#include "tests/test_utilities.h"
#define SHIP_OUT_FILE "../yonatan_tests/test_ship_out.txt"
#define SHIP_TEST_FILE "../yonatan_tests/test_ship.txt"
#define CLEAR_TEST_FILE "../yonatan_tests/test_clear_product.txt"
#define CLEAR_OUT_FILE "../yonatan_tests/test_clear_product_out.txt"
#define NEW_PRODUCT_OUT_FILE "../yonatan_tests/test_new_product_out.txt"
#define NEW_PRODUCT_TEST_FILE "../yonatan_tests/test_new_product.txt"
#define CHANGE_PRODUCT_AMOUNT_TEST_FILE "../yonatan_tests/test_change_product_amount.txt"
#define CHANGE_PRODUCT_AMOUNT_OUT_FILE "../yonatan_tests/test_change_product_amount_out.txt"
#define CHANGE_PRODUCT_AMOUNT_IN_ORDER_TEST_FILE "../yonatan_tests/test_change_product_amount_in_order.txt"
#define CHANGE_PRODUCT_AMOUNT_IN_ORDER_OUT_FILE "../yonatan_tests/test_change_product_amount_in_order_out.txt"

#define BOOLEAN_TEST(test) passed = passed && test;
#define START_TEST bool passed = true;
#define END_TEST return passed;
static bool fileEqual(FILE *file1, FILE *file2) {
    int c1, c2;
    do {
        c1 = fgetc(file1);
        c2 = fgetc(file2);
    } while (c1 != EOF && c2 != EOF && c1 == c2);
    return (c1 == EOF && c2 == EOF);
}

bool wholeFileEqualGeneral(const char *filename1, const char *filename2) {
    FILE *file1 = fopen(filename1, "r");

    FILE *file2 = fopen(filename2, "r");

    assert(file1);
    assert(file2);
    bool result = fileEqual(file1, file2);
    fclose(file1);
    fclose(file2);
    return result;
}

MtmProductData copyInt(MtmProductData intPtr){
    MtmProductData elm = malloc(sizeof(int));
    if(!elm){
        return NULL;
    }
    *(int*)elm = *(int*)intPtr;
    return elm;
}
void freeInt(MtmProductData intPtr){
    free(intPtr);
}

double getPrice(MtmProductData data ,double amount){
    return *(int*)data *amount;
}


//bool filterIdModulo2(const unsigned int id, const char *name, const double amount, MtmProductData customData){
//    return id%2==0;
//}
bool testCreateDestroy(){
    START_TEST
    Matamazom  matamazom = matamazomCreate();
    BOOLEAN_TEST(matamazom!=NULL)
    matamazomDestroy(matamazom);
    matamazomDestroy(NULL);
    END_TEST
}



bool testMtmNewProduct(){
    int a  =3;
    unsigned int id =1;

    MtmProductData data =&a;
    START_TEST
    Matamazom  matamazom = matamazomCreate();
    FILE * outfile = fopen(NEW_PRODUCT_OUT_FILE,"w");

    mtmPrintInventory(matamazom,outfile);

    MatamazomResult result = mtmNewProduct(matamazom,id,"prod",4,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_SUCCESS)
    result = mtmNewProduct(matamazom,id,"",4,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_INVALID_NAME)
    result = mtmNewProduct(matamazom,id,"-",4,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_INVALID_NAME)
    result = mtmNewProduct(matamazom,id,"-",4.5,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_INVALID_NAME)
    result = mtmNewProduct(matamazom,id,"A",4.5,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_INVALID_AMOUNT)
    result = mtmNewProduct(NULL,id,"prod",4,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_NULL_ARGUMENT);
    result = mtmNewProduct(matamazom,id,NULL,4,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_NULL_ARGUMENT);
    result = mtmNewProduct(matamazom,id,"prod",4,MATAMAZOM_INTEGER_AMOUNT,NULL,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_NULL_ARGUMENT);
    result = mtmNewProduct(matamazom,id,"prod",4.5,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_INVALID_AMOUNT);
    result = mtmNewProduct(matamazom,id,"prod",4.5,MATAMAZOM_HALF_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_PRODUCT_ALREADY_EXIST);
    result = mtmNewProduct(matamazom,id,"prod",4.6,MATAMAZOM_HALF_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_INVALID_AMOUNT);
    result = mtmNewProduct(matamazom,id,"prod",4.6,MATAMAZOM_ANY_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_PRODUCT_ALREADY_EXIST);
    result = mtmNewProduct(matamazom,2,"prod",4.6,MATAMAZOM_ANY_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_SUCCESS);

    mtmPrintInventory(matamazom,outfile);
    fclose(outfile);
    BOOLEAN_TEST(wholeFileEqualGeneral(NEW_PRODUCT_OUT_FILE,NEW_PRODUCT_TEST_FILE));
    result = mtmNewProduct(matamazom,2,"prod",4.6,MATAMAZOM_HALF_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    BOOLEAN_TEST(result==MATAMAZOM_INVALID_AMOUNT);
    matamazomDestroy(matamazom);
    END_TEST
}
static void addProducts(Matamazom matamazom){
    int a = 3;
    MtmProductData data =&a;

    mtmNewProduct(matamazom,1,"prod",4,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,2,"prod2",4.5,MATAMAZOM_HALF_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,3,"prod3",4.6,MATAMAZOM_ANY_AMOUNT,data,copyInt,freeInt,getPrice);

}
bool test_mtmCancelOrder(){
    START_TEST
    Matamazom matamazom = matamazomCreate();
    unsigned int order_id1 = mtmCreateNewOrder(matamazom);
//    unsigned int order_id2 = mtmCreateNewOrder(matamazom);
//    unsigned int order_id3 = mtmCreateNewOrder(matamazom);
    BOOLEAN_TEST(mtmCancelOrder(matamazom,order_id1)==MATAMAZOM_SUCCESS);
    BOOLEAN_TEST(mtmCancelOrder(matamazom,order_id1)==MATAMAZOM_ORDER_NOT_EXIST);
    BOOLEAN_TEST(mtmCancelOrder(matamazom,5)==MATAMAZOM_ORDER_NOT_EXIST);
    BOOLEAN_TEST(mtmCancelOrder(NULL,5)==MATAMAZOM_NULL_ARGUMENT);

    BOOLEAN_TEST(mtmPrintOrder(matamazom,order_id1,stdout)==MATAMAZOM_ORDER_NOT_EXIST)
    addProducts(matamazom);
    BOOLEAN_TEST(mtmChangeProductAmountInOrder(matamazom,order_id1,1,1)==MATAMAZOM_ORDER_NOT_EXIST)
    BOOLEAN_TEST(mtmChangeProductAmountInOrder(matamazom,order_id1,55,1)==MATAMAZOM_ORDER_NOT_EXIST)
    matamazomDestroy(matamazom);


    END_TEST
}
bool testChangeProductAmount(){
    int a  =3;
    MtmProductData data =&a;
    bool passed = true;
    Matamazom  matamazom = matamazomCreate();
    FILE * outfile = fopen(CHANGE_PRODUCT_AMOUNT_OUT_FILE,"w");
    mtmNewProduct(matamazom,2,"prod",4.6,MATAMAZOM_ANY_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,3,"prod",4.5,MATAMAZOM_HALF_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,4,"prod",4,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmPrintInventory(matamazom,outfile);

    MatamazomResult result = mtmChangeProductAmount(matamazom,2,0.3);
    passed = passed&& (result==MATAMAZOM_SUCCESS);
    passed = passed && mtmChangeProductAmount(matamazom,3,0.3)==MATAMAZOM_INVALID_AMOUNT;
    passed = passed && mtmChangeProductAmount(matamazom,3,-5)==MATAMAZOM_INSUFFICIENT_AMOUNT;
    passed = passed && mtmChangeProductAmount(matamazom,3,0.501)==MATAMAZOM_SUCCESS;
    passed = passed && mtmChangeProductAmount(matamazom,3,0.50101)==MATAMAZOM_INVALID_AMOUNT;
    passed = passed && mtmChangeProductAmount(matamazom,3,0.499)==MATAMAZOM_SUCCESS;
    passed = passed && mtmChangeProductAmount(matamazom,3,0.498)==MATAMAZOM_INVALID_AMOUNT;
    mtmPrintInventory(matamazom,outfile);

    passed = passed && mtmChangeProductAmount(matamazom,3,4.5)==MATAMAZOM_SUCCESS;
    passed = passed && mtmChangeProductAmount(matamazom,4,0.5)==MATAMAZOM_INVALID_AMOUNT;
    passed = passed && mtmChangeProductAmount(matamazom,4,0.2)==MATAMAZOM_INVALID_AMOUNT;
    passed = passed && mtmChangeProductAmount(matamazom,4,1.001)==MATAMAZOM_SUCCESS;
    passed = passed && mtmChangeProductAmount(matamazom,4,0.999)==MATAMAZOM_SUCCESS;
    passed = passed && mtmChangeProductAmount(matamazom,4,1.00101)==MATAMAZOM_INVALID_AMOUNT;
    passed = passed && mtmChangeProductAmount(matamazom,4,0.998)==MATAMAZOM_INVALID_AMOUNT;
    passed = passed && mtmChangeProductAmount(matamazom,4,-10)==MATAMAZOM_INSUFFICIENT_AMOUNT;
    passed = passed && mtmChangeProductAmount(matamazom,4,-1)==MATAMAZOM_SUCCESS;
    mtmPrintInventory(matamazom,outfile);
    fclose(outfile);
    passed = passed && wholeFileEqualGeneral(CHANGE_PRODUCT_AMOUNT_OUT_FILE,CHANGE_PRODUCT_AMOUNT_TEST_FILE);
    matamazomDestroy(matamazom);
    return passed;
}
bool testClearProduct(){
    int a  =3;
    MtmProductData data =&a;
    int b  =1;
    MtmProductData datab =&b;
    bool passed = true;
    FILE * outputFile = fopen(CLEAR_OUT_FILE, "w");

    Matamazom  matamazom = matamazomCreate();
    mtmNewProduct(matamazom,2,"prod",4.6,MATAMAZOM_ANY_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,1,"p2",4,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,56,"p3",3.5,MATAMAZOM_HALF_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,43,"p4",4.5,MATAMAZOM_HALF_INTEGER_AMOUNT,datab,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,3,"p5",4.5,MATAMAZOM_HALF_INTEGER_AMOUNT,datab,copyInt,freeInt,getPrice);
    unsigned int order_id1 = mtmCreateNewOrder(matamazom);
    unsigned int order_id2 = mtmCreateNewOrder(matamazom);
    unsigned int order_id3 = mtmCreateNewOrder(matamazom);
    unsigned int order_id4 = mtmCreateNewOrder(matamazom);
    mtmPrintOrder(matamazom,order_id1,outputFile);
    mtmPrintOrder(matamazom,order_id2,outputFile);
    mtmPrintOrder(matamazom,order_id3,outputFile);
    mtmPrintOrder(matamazom,order_id4,outputFile);
    mtmChangeProductAmountInOrder(matamazom,order_id1,1,20);
    mtmChangeProductAmountInOrder(matamazom,order_id1,56,2.5);
    mtmChangeProductAmountInOrder(matamazom,order_id1,2,2.4);
    mtmChangeProductAmountInOrder(matamazom,order_id2,1,20);
    mtmChangeProductAmountInOrder(matamazom,order_id2,2,2.4);
    mtmChangeProductAmountInOrder(matamazom,order_id2,56,2.5);
    mtmChangeProductAmountInOrder(matamazom,order_id2,3,0);
    mtmChangeProductAmountInOrder(matamazom,order_id3,1,20);
    mtmChangeProductAmountInOrder(matamazom,order_id3,43,2.5);
    mtmChangeProductAmountInOrder(matamazom,order_id3,56,2.5);
    mtmChangeProductAmountInOrder(matamazom,order_id4,3,20);
    mtmChangeProductAmountInOrder(matamazom,order_id4,43,2.5);
    mtmChangeProductAmountInOrder(matamazom,order_id4,56,2.5);



    mtmPrintInventory(matamazom,outputFile);
    mtmPrintOrder(matamazom,order_id1,outputFile);
    mtmPrintOrder(matamazom,order_id2,outputFile);
    mtmPrintOrder(matamazom,order_id3,outputFile);
    mtmPrintOrder(matamazom,order_id4,outputFile);

    mtmClearProduct(matamazom,3);
    mtmClearProduct(matamazom,43);
    mtmClearProduct(matamazom,56);
    mtmPrintOrder(matamazom,order_id1,outputFile);
    mtmPrintOrder(matamazom,order_id2,outputFile);
    mtmPrintOrder(matamazom,order_id3,outputFile);
    mtmPrintOrder(matamazom,order_id4,outputFile);
    mtmPrintInventory(matamazom,outputFile);

    passed = passed && (mtmClearProduct(matamazom,33)==MATAMAZOM_PRODUCT_NOT_EXIST);
    passed = passed && (mtmClearProduct(matamazom,3)==MATAMAZOM_PRODUCT_NOT_EXIST);
    passed = passed && (mtmClearProduct(matamazom,46)==MATAMAZOM_PRODUCT_NOT_EXIST);
    passed = passed && (mtmClearProduct(matamazom,56)==MATAMAZOM_PRODUCT_NOT_EXIST);
    passed = passed && (mtmClearProduct(NULL,56)==MATAMAZOM_NULL_ARGUMENT);

    mtmPrintInventory(matamazom,outputFile);
    fclose(outputFile);
    passed = passed && wholeFileEqualGeneral(CLEAR_OUT_FILE,CLEAR_TEST_FILE);
    matamazomDestroy(matamazom);
    return  passed;



}
bool testOrderShip(){
    int a  =3;
    MtmProductData data =&a;
    bool passed  = true;
    Matamazom  matamazom = matamazomCreate();
    mtmNewProduct(matamazom,2,"prod",4.6,MATAMAZOM_ANY_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,6,"p2",4,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,56,"p3",4.5,MATAMAZOM_HALF_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,43,"p4",4.5,MATAMAZOM_HALF_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,3,"p5",4.5,MATAMAZOM_HALF_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    unsigned int order_id = mtmCreateNewOrder(matamazom);
    passed = passed&&mtmChangeProductAmountInOrder(matamazom,order_id,6,1.5)==MATAMAZOM_INVALID_AMOUNT;
    passed = passed&&mtmChangeProductAmountInOrder(matamazom,order_id,6,1)==MATAMAZOM_SUCCESS;
    FILE * outputFile = fopen(SHIP_OUT_FILE, "w");
    mtmPrintInventory(matamazom,outputFile);

    mtmPrintBestSelling(matamazom,outputFile);

    passed = passed&& mtmShipOrder(matamazom,order_id)==MATAMAZOM_SUCCESS;


    mtmPrintInventory(matamazom,outputFile);
    mtmPrintBestSelling(matamazom,outputFile);


    order_id = mtmCreateNewOrder(matamazom);
    passed = passed&&mtmChangeProductAmountInOrder(matamazom,order_id,6,4)==MATAMAZOM_SUCCESS;

    passed = passed&& mtmShipOrder(matamazom,order_id)==MATAMAZOM_INSUFFICIENT_AMOUNT;
    passed = passed&&mtmChangeProductAmountInOrder(matamazom,order_id,6,-1)==MATAMAZOM_SUCCESS;

    passed = passed&& mtmShipOrder(matamazom,order_id)==MATAMAZOM_SUCCESS;
    mtmPrintInventory(matamazom,outputFile);

    mtmPrintBestSelling(matamazom,outputFile);

    order_id = mtmCreateNewOrder(matamazom);
    passed = passed&&mtmChangeProductAmountInOrder(matamazom,order_id,2,4.3)==MATAMAZOM_SUCCESS;
    passed = passed&&mtmChangeProductAmountInOrder(matamazom,order_id,6,4)==MATAMAZOM_SUCCESS;

    passed = passed&& mtmShipOrder(matamazom,order_id)==MATAMAZOM_INSUFFICIENT_AMOUNT;
    mtmPrintInventory(matamazom,outputFile);


    fclose(outputFile);


    passed = passed&&wholeFileEqualGeneral(SHIP_TEST_FILE,SHIP_OUT_FILE);

    matamazomDestroy(matamazom);

    return passed;
}

bool testNewOrder(){
    bool passed = true;

    Matamazom  matamazom = matamazomCreate();
    passed = passed && mtmCreateNewOrder(matamazom)==1;
    passed = passed && mtmCreateNewOrder(matamazom)==2;
    passed = passed && mtmCreateNewOrder(matamazom)==3;
    matamazomDestroy(matamazom);
    return  passed;
}
bool testMtmChangeOrderAmount(){

    int a  =3;
    MtmProductData data =&a;
    bool passed  = true;
    FILE * outfile = fopen(CHANGE_PRODUCT_AMOUNT_IN_ORDER_OUT_FILE,"w");
    //Creating MATAMAZOM
    Matamazom  matamazom = matamazomCreate();
    //Creating products
    mtmNewProduct(matamazom,111,"Banana",308.5,MATAMAZOM_ANY_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,89,"Television",25,MATAMAZOM_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,126,"Fish",95.7,MATAMAZOM_ANY_AMOUNT,data,copyInt,freeInt,getPrice);
    mtmNewProduct(matamazom,43,"Black Eyed Pea",55.5,MATAMAZOM_HALF_INTEGER_AMOUNT,data,copyInt,freeInt,getPrice);

    //Creating Orders
    unsigned int order1 = mtmCreateNewOrder(matamazom);
    unsigned int order2 = mtmCreateNewOrder(matamazom);
    unsigned int order3 = mtmCreateNewOrder(matamazom);

    passed = passed && mtmChangeProductAmountInOrder(matamazom,order1,111,14.3)            == MATAMAZOM_SUCCESS;
//    after this operation product should be removed form the order//
    mtmPrintOrder(matamazom,order1,outfile);

    passed = passed && mtmChangeProductAmountInOrder(matamazom,order1,111,-15)             == MATAMAZOM_SUCCESS;
    mtmPrintOrder(matamazom,order1,outfile);

    passed = passed && mtmChangeProductAmountInOrder(matamazom,order1,111,14.3)            == MATAMAZOM_SUCCESS;
    mtmPrintOrder(matamazom,order1,outfile);
    passed = passed && mtmChangeProductAmountInOrder(matamazom,order1,111,-14.3)            == MATAMAZOM_SUCCESS;
    mtmPrintOrder(matamazom,order1,outfile);
    passed = passed && mtmChangeProductAmountInOrder(matamazom,order1,111,14.3)            == MATAMAZOM_SUCCESS;
    passed = passed && mtmChangeProductAmountInOrder(matamazom,order1,89,5)                == MATAMAZOM_SUCCESS;
    passed = passed && mtmChangeProductAmountInOrder(matamazom,order1,89,1)                == MATAMAZOM_SUCCESS;
    passed = passed && mtmChangeProductAmountInOrder(matamazom,order1,126,7.7)             == MATAMAZOM_SUCCESS;
    mtmPrintOrder(matamazom,order1,outfile);
    passed = passed && mtmChangeProductAmountInOrder(matamazom,order1,89,-3)    == MATAMAZOM_SUCCESS;
    passed = passed && mtmChangeProductAmountInOrder(matamazom,order1,126,-7.7)                == MATAMAZOM_SUCCESS;
    passed = passed && mtmChangeProductAmountInOrder(matamazom,order1,111,-7.7)                == MATAMAZOM_SUCCESS;
    mtmPrintOrder(matamazom,order1,outfile);

    passed = passed && mtmChangeProductAmountInOrder(matamazom,4,89,2)             == MATAMAZOM_ORDER_NOT_EXIST;
    passed = passed && mtmChangeProductAmountInOrder(matamazom,order2,43,2.1)  == MATAMAZOM_INVALID_AMOUNT;
    passed = passed && mtmChangeProductAmountInOrder(matamazom,order3,43,-1.1) == MATAMAZOM_INVALID_AMOUNT;
    passed = passed && mtmChangeProductAmountInOrder(matamazom,order3,15,0)      == MATAMAZOM_PRODUCT_NOT_EXIST;

    mtmPrintOrder(matamazom,order2,outfile);
    mtmPrintOrder(matamazom,order3,outfile);

    passed = passed && mtmChangeProductAmountInOrder(matamazom,order3,111,0)      == MATAMAZOM_SUCCESS;
    mtmPrintOrder(matamazom,order3,outfile);
    passed = passed && mtmChangeProductAmountInOrder(matamazom,order3,43,0.5)      == MATAMAZOM_SUCCESS;
    mtmPrintOrder(matamazom,order3,outfile);


    fclose(outfile);
    BOOLEAN_TEST(wholeFileEqualGeneral(CHANGE_PRODUCT_AMOUNT_IN_ORDER_OUT_FILE,CHANGE_PRODUCT_AMOUNT_IN_ORDER_TEST_FILE))
    matamazomDestroy(matamazom);
    return  passed;
}

int main() {
    RUN_TEST(testNewOrder);
    RUN_TEST(testMtmNewProduct);
    RUN_TEST(testChangeProductAmount);
    RUN_TEST(testClearProduct);
    RUN_TEST(testOrderShip);
    RUN_TEST(testMtmChangeOrderAmount);
    RUN_TEST(testCreateDestroy);
    RUN_TEST(test_mtmCancelOrder);
}




