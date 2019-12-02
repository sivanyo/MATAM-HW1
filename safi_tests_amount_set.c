#include <stdio.h>
#include "amount_set.h"
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

typedef struct point{

    int x;

}Point;

int GetX(Point *p){

    return p->x;

}

void SetX(Point *p, int n){

    p->x=n;

}



Point* copy(Point* p) {

    Point *tmp = malloc(sizeof(*p));
    tmp->x = p->x;
    return tmp;
}

void freeElement(Point *p){

    free(p);

}


int compare(Point* p1, Point* p2){

    if(p1->x==p2->x){
        return 0;
    }
    if(p1->x>p2->x){
        return 1;
    }
    return -1;
}


Point* create(int x){

    Point* p = malloc(sizeof(*p));
    p->x=x;
    return p;


}

ASElement copyInt(ASElement number) {
    Point* tmp=(Point*)number;
    return copy(tmp);
}

void freeInt(ASElement number) {
    Point* tmp=(Point*)number;
    freeElement(tmp);
}

int compareInts(ASElement lhs, ASElement rhs) {
    Point* tmp1=(Point*)lhs;
    Point* tmp2=(Point*)rhs;
    return compare(tmp1,tmp2);
}

int main() {

    AmountSet alist1 = asCreate(copyInt,freeInt,compareInts);
    AmountSet alist2 = asCreate(copyInt,freeInt,compareInts);
    AmountSet alist4 = asCreate(copyInt,freeInt,compareInts);
    AmountSet alist5 = asCreate(copyInt,freeInt,compareInts);
    AmountSet alist6 = asCreate(copyInt,freeInt,compareInts);
    AmountSet alist7 = asCreate(copyInt,freeInt,compareInts);
    AmountSet alist8 = asCreate(copyInt,freeInt,compareInts);
    AmountSet alist9 = asCreate(copyInt,freeInt,compareInts);
    AmountSet alist10 = asCreate(copyInt,freeInt,compareInts);


    Point *ap1 = create(5);//1
    Point *ap2 = create(6);//2
    Point *ap3 = create(6);
    Point *ap4 = create(12);//3
    Point *ap5 = create(21);//4
    Point *ap6 = create(42);//5
    Point *ap7 = create(12);
    Point *ap8 = create(211);//6
    Point *ap9 = create(1000);//7
    Point *ap10 = create(13);//8
    Point *ap11 = create(1152);//9
    Point *ap12 = create(2);//10
    Point *ap13 = create(2);
    Point *ap14 = create(1);//11
    Point *ap15 = create(1);
    Point *ap16 = create(62);//12
    Point *ap17 = create(27);//13
    Point *ap18 = create(11);//14
    Point *ap19 = create(1);
    Point *ap20 = create(25);//15
    Point *ap21 = create(2);
    Point *ap22 = create(209);//16
    Point *ap23 = create(-20);//17
    Point *ap24 = create(-21);//18
    Point *ap25 = create(-23);//19
    Point *ap26 = create(-24);//20
    Point *ap27 = create(-25);//21
    Point *ap28 = create(-26);//22
    Point *ap29 = create(-27);//23
    Point *ap30 = create(-28);//24
    Point *ap31 = create(-29);//25
    Point *ap32 = create(1);
    Point *ap33 = create(2);
    Point *ap34 = create(4);//26
    Point *ap35 = create(4);
    Point *ap36 = create(4);

    asRegister(alist1, ap2);//6
    assert(asRegister(alist1, ap3)==AS_ITEM_ALREADY_EXISTS);//re 6
    asRegister(alist1, ap4);//12
    asRegister(alist1, ap5);//21
    asRegister(alist1, ap6);//42
    assert(asRegister(alist1, ap7)==AS_ITEM_ALREADY_EXISTS);//re 12
    asRegister(alist1, ap8);//211
    asRegister(alist1, ap9);//1000


    AmountSet acopiedlist = asCopy(alist1);





    asRegister(alist2, ap10);//13




    asRegister(alist5, ap11);//1152
    asRegister(alist5, ap12);//2
    assert(asRegister(alist5, ap13)==AS_ITEM_ALREADY_EXISTS);//re 2
    asRegister(alist5, ap14);//1
    assert(asRegister(alist5, ap15)==AS_ITEM_ALREADY_EXISTS);//re 1
    asRegister(alist5, ap16);//62
    asRegister(alist5, ap17);//27

    asClear(alist5); //p11-p17 freed



    asRegister(alist6, ap19);//1
    asRegister(alist6, ap20);//25
    asRegister(alist6, ap21);//2
    asRegister(alist6, ap22);//209
    asRegister(alist6, ap23);//-20
    asRegister(alist6, ap24);//-21
    asRegister(alist6, ap25);//-23
    asRegister(alist6, ap26);//-24

    asDelete(alist6, ap19); //p19 freed 1
    asDelete(alist6, ap20); //p20 freed 25
    asDelete(alist6, ap21); //p21 freed 2
    assert(asDelete(alist6, ap21)==AS_ITEM_DOES_NOT_EXIST); //p21 refreed 2
    asDelete(alist6, ap22);//p22 freed 209
    asDelete(alist6, ap23);//p23 freed -20
    asDelete(alist6, ap24);//p24 freed -21
    assert(asDelete(alist6, ap24)==AS_ITEM_DOES_NOT_EXIST); //p24 refreed -21
    asDelete(alist6, ap25);//p25 freed -23
    asDelete(alist6, ap26);//p26 freed -24


    asRegister(alist7, ap27);//-25
    asRegister(alist7, ap28);//-26
    asRegister(alist7, ap29);//-27
    asRegister(alist7, ap30);//-28
    asRegister(alist7, ap31);//-29

    Point* tmp=(Point*)asGetFirst(alist7);//=-29
    assert(GetX(tmp)==-29);
    tmp=asGetNext(alist7);//-28
    assert(GetX(tmp)==-28);
    tmp=asGetNext(alist7);//-27
    assert(GetX(tmp)==-27);
    tmp=asGetNext(alist7);//-26
    assert(GetX(tmp)==-26);
    asDelete(alist7, ap29); //p29 freed -27



    asRegister(alist8, ap32);//1
    assert(asDelete(alist8, ap32)==AS_SUCCESS); //p32 freed 1



    asRegister(alist9, ap33);//2

    AmountSet acopied1 = asCopy(alist9);

    asDelete(acopied1, ap33);  //copy of p33 freed 2



    asRegister(alist10, ap34);//4
    assert(asRegister(alist10, ap35)==AS_ITEM_ALREADY_EXISTS);//re 4
    assert(asRegister(alist10, ap36)==AS_ITEM_ALREADY_EXISTS);//re 4


    asDestroy(alist10);
    asDestroy(alist9); //list9 && p33 freed
    asDestroy(acopied1);//copied1 freed
    asDestroy(alist6); //list6 freed
    asDestroy(alist1);    //p2-p9 & list1 freed
    asDestroy(acopiedlist); //copies of p2-p9 & copiedlist freed
    asDestroy(alist7); //27,28,30,31 freed
    asDestroy(alist8); //list8 freed
    asDestroy(alist5); //list5 freed
    asDestroy(alist2); // list2 & p10 freed
    asDestroy(alist4); //list4 freed
    //p34 && list10 freed
    free(ap1);
    free(ap2);
    free(ap3);
    free(ap4);
    free(ap5);
    free(ap6);
    free(ap7);
    free(ap8);
    free(ap9);
    free(ap10);
    free(ap11);
    free(ap12);
    free(ap13);
    free(ap14);
    free(ap15);
    free(ap16);
    free(ap17);
    free(ap18);
    free(ap19);
    free(ap20);
    free(ap21);
    free(ap22);
    free(ap23);
    free(ap24);
    free(ap25);
    free(ap26);
    free(ap27);
    free(ap28);
    free(ap29);
    free(ap30);
    free(ap31);
    free(ap32);
    free(ap33);
    free(ap34);
    free(ap35);
    free(ap36);

    AmountSet list1 = asCreate(copyInt,freeInt,compareInts);


    Point* p1 = create(5);//1
    Point* p2 = create(6);//2
    Point* p3 = create(6);
    Point* p4 = create(12);//3
    Point* p5 = create(21);//4
    Point* p6 = create(42);//5
    Point* p7 = create(12);
    Point* p8 = create(211);//6
    Point* p9 = create(1000);//7
    Point* p10 = create(13);//8
    Point* p11 = create(1152);//9
    Point* p12 = create(2);//10
    Point* p13 = create(2);
    Point* p14 = create(1);//11
    Point* p15 = create(1);
    Point* p16 = create(62);//12
    Point* p17 = create(27);//13
    Point* p18 = create(11);//14
    Point* p19 = create(1);
    Point* p20 = create(25);//15
    Point* p21 = create(2);
    Point* p22 = create(209);//16

    //insert

    //TESTING INSERT FUNCTION
    assert(asRegister(list1,NULL)==AS_NULL_ARGUMENT);
    assert(asRegister(list1,NULL)==AS_NULL_ARGUMENT);
    assert(asRegister(list1,p1)==AS_SUCCESS);//5
    assert(asRegister(list1,NULL)==AS_NULL_ARGUMENT);
    assert(asRegister(list1,p2)==AS_SUCCESS);//6
    assert(asRegister(list1,p3)==AS_ITEM_ALREADY_EXISTS);//re 6
    assert(asRegister(list1,p4)==AS_SUCCESS);//12
    assert(asRegister(list1,p5)==AS_SUCCESS);//21
    assert(asRegister(list1,p6)==AS_SUCCESS);//42
    assert(asRegister(list1,p7)==AS_ITEM_ALREADY_EXISTS);//re 12
    assert(asRegister(list1,p8)==AS_SUCCESS);//211
    assert(asRegister(list1,p9)==AS_SUCCESS);//1000
    assert(asRegister(list1,p10)==AS_SUCCESS);//13
    assert(asRegister(list1,p11)==AS_SUCCESS);//1152
    assert(asRegister(list1,NULL)==AS_NULL_ARGUMENT);
    assert(asRegister(list1,NULL)==AS_NULL_ARGUMENT);
    assert(asRegister(list1,p12)==AS_SUCCESS);//2
    assert(asRegister(list1,p13)==AS_ITEM_ALREADY_EXISTS);//re 2
    assert(asRegister(list1,p14)==AS_SUCCESS);//1
    assert(asRegister(list1,p15)==AS_ITEM_ALREADY_EXISTS);//re 1
    assert(asRegister(list1,p16)==AS_SUCCESS);//62
    assert(asRegister(list1,p17)==AS_SUCCESS);//27
    assert(asRegister(list1,p18)==AS_SUCCESS);//11
    assert(asRegister(list1,p19)==AS_ITEM_ALREADY_EXISTS);//re 1
    assert(asRegister(list1,p20)==AS_SUCCESS);//25
    assert(asRegister(list1,p21)==AS_ITEM_ALREADY_EXISTS);//re 2
    assert(asRegister(list1,p22)==AS_SUCCESS);//209
    assert(asRegister(NULL,NULL)==AS_NULL_ARGUMENT);
    assert(asRegister(NULL,p22)==AS_NULL_ARGUMENT);
    //the asmount{1,2,5,6,11,12,13,21,25,27,42,62,209,211,1000,1152}
    printf("INSERT FUNCTION WORKING\n");




    //TESTING SIZE & REMOVE
    assert(asGetSize(list1)==16);
    AmountSet list2 = asCopy(list1);
    assert(asGetSize(list2)==16);
    assert(asDelete(NULL, p1)==AS_NULL_ARGUMENT);
    assert(asDelete(list1, p1)==AS_SUCCESS);//delete 5
    assert(asGetSize(list1)==15);
    assert(asDelete(list1, p1)==AS_ITEM_DOES_NOT_EXIST);//no 5
    assert(asDelete(list1, p2)==AS_SUCCESS);//delete 6
    assert(asDelete(list1,p22)==AS_SUCCESS);//delete 209
    assert(asDelete(NULL,p22)==AS_NULL_ARGUMENT);
    assert(asDelete(list1,NULL)==AS_NULL_ARGUMENT);
    assert(asGetSize(NULL)==-1);
    assert(asGetSize(list1)==13);
    assert(asGetSize(list2)==16);

    //the list1{1,2,11,12,13,21,25,27,42,62,211,1000,1152}
    //the list2{1,2,5,6,11,12,13,21,25,27,42,62,209,211,1000,1152}
    printf("REMOVE FUNCTION WORKING\n");

    //TESTING GETAMOUNT FUNCTION
    double tmp_amount =0;
    assert(asGetAmount(NULL,p14,&tmp_amount)==AS_NULL_ARGUMENT);
    assert(asGetAmount(list1,NULL,&tmp_amount)==AS_NULL_ARGUMENT);
    assert(asGetAmount(list1,p14,NULL)==AS_NULL_ARGUMENT);
    asGetAmount(list1,p14,&tmp_amount);//1
    assert(tmp_amount==0);
    asGetAmount(list1,p12,&tmp_amount);//2
    assert(tmp_amount==0);
    asGetAmount(list1,p18,&tmp_amount);//11
    assert(tmp_amount==0);
    asGetAmount(list1,p20,&tmp_amount);//25
    assert(tmp_amount==0);
    asGetAmount(list1,p17,&tmp_amount);//27
    assert(tmp_amount==0);
    asGetAmount(list1,p8,&tmp_amount);//211
    assert(tmp_amount==0);
    asGetAmount(list1,p5,&tmp_amount);//21
    assert(tmp_amount==0);
    asGetAmount(list1,p16,&tmp_amount);//62
    assert(tmp_amount==0);
    asDelete(list1,p5);//delete 21
    assert(asGetAmount(list1,p5,&tmp_amount)==AS_ITEM_DOES_NOT_EXIST);
    asDelete(list1,p18);//delete 11
    assert(asGetAmount(list1,p18,&tmp_amount)==AS_ITEM_DOES_NOT_EXIST);

    printf("GETAMOUNT FUNCTION WORKING\n");
    //the list1{1,2,12,13,25,27,42,62,211,1000,1152}

    //TESTING CHANGEAMOUNT FUNCTION

    assert(asChangeAmount(list1,NULL,1)==AS_NULL_ARGUMENT);
    assert(asChangeAmount(NULL,p17,1)==AS_NULL_ARGUMENT);
    assert(asChangeAmount(NULL,NULL,1)==AS_NULL_ARGUMENT);
    assert(asChangeAmount(list1,p17,-1)==AS_INSUFFICIENT_AMOUNT);//27
    asGetAmount(list1,p17,&tmp_amount);//27
    assert(tmp_amount==0);
    assert(asChangeAmount(list1,p17,7)==AS_SUCCESS);
    asGetAmount(list1,p17,&tmp_amount);//27
    assert(tmp_amount==7);
    assert(asChangeAmount(list1,p17,0)==AS_SUCCESS);
    asGetAmount(list1,p17,&tmp_amount);//27
    assert(tmp_amount==7);
    assert(asChangeAmount(list1,p17,-4)==AS_SUCCESS);
    asGetAmount(list1,p17,&tmp_amount);//27
    assert(tmp_amount==3);
    assert(asChangeAmount(list1,p17,-4)==AS_INSUFFICIENT_AMOUNT);
    asGetAmount(list1,p17,&tmp_amount);//27
    assert(tmp_amount==3);
    assert(asChangeAmount(list1,p5,-4)==AS_ITEM_DOES_NOT_EXIST);
    assert(asChangeAmount(list1,p18,-4)==AS_ITEM_DOES_NOT_EXIST);
    AmountSet tmpcopy=asCopy(list1);
    tmp_amount=0;
    asGetAmount(tmpcopy,p17,&tmp_amount);//27
    assert(tmp_amount==3);
    asGetAmount(tmpcopy,p8,&tmp_amount);//211
    assert(tmp_amount==0);
    asDestroy(tmpcopy);

    printf("CHANGEAMOUNT FUNCTION WORKING\n");

    //TESTING CLEAR
    asClear(list1);
    assert(asClear(NULL)==AS_NULL_ARGUMENT);
    assert(asGetSize(list1)==0);
    asDestroy(list2);
    asDestroy(list1);


    //TESTING IF Set IS SORTED, AND GETNEXT & GETFIRST FUNCTIONS
    Point* p30 = create(1);
    Point* p31 = create(2);
    Point* p32 = create(3);
    Point* p33 = create(4);
    Point* p34 = create(5);
    Point* p35 = create(6);
    Point* p36 = create(7);
    Point* p37 = create(8);
    Point* p38 = create(9);
    Point* p40 = create(10);
    Point* p41 = create(11);
    Point* p42 = create(12);
    Point* p43 = create(13);
    Point* p44 = create(14);
    Point* p45 = create(15);
    Point* p46 = create(16);

    AmountSet newlist = asCreate(copyInt,freeInt,compareInts);
    assert(asGetFirst(newlist)==NULL);
    assert(asGetFirst(NULL)==NULL);
    assert(asGetNext(newlist)==NULL);
    assert(asGetNext(NULL)==NULL);
    asRegister(newlist,p46);//16
    asRegister(newlist,p30);//1
    asRegister(newlist,p32);//3
    asRegister(newlist,p31);//2
    asRegister(newlist,p45);//15
    asRegister(newlist,p44);//14
    asRegister(newlist,p41);//11
    asRegister(newlist,p38);//9
    asRegister(newlist,p36);//7
    asRegister(newlist,p37);//8
    asRegister(newlist,p33);//4
    asRegister(newlist,p34);//5
    asRegister(newlist,p40);//10
    asRegister(newlist,p42);//12
    asRegister(newlist,p43);//13
    asRegister(newlist,p35);//6
    assert(asGetSize(newlist)==16);
    assert(GetX((Point*)asGetFirst(newlist))==1);
    assert(GetX((Point*)asGetNext(newlist))==2);
    assert(GetX((Point*)asGetNext(newlist))==3);
    assert(GetX((Point*)asGetNext(newlist))==4);
    assert(GetX((Point*)asGetNext(newlist))==5);
    assert(GetX((Point*)asGetNext(newlist))==6);
    assert(GetX((Point*)asGetNext(newlist))==7);
    assert(GetX((Point*)asGetNext(newlist))==8);
    assert(GetX((Point*)asGetNext(newlist))==9);
    assert(GetX((Point*)asGetNext(newlist))==10);
    assert(GetX((Point*)asGetNext(newlist))==11);
    assert(GetX((Point*)asGetNext(newlist))==12);
    assert(GetX((Point*)asGetNext(newlist))==13);
    assert(GetX((Point*)asGetNext(newlist))==14);
    assert(GetX((Point*)asGetNext(newlist))==15);
    assert(GetX((Point*)asGetNext(newlist))==16); ///
    //newlist{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}

    printf("Set IS SORTED! \n");


    //CHECKING COPY FUNCTION(checking if the copied set elements points to the
    //place in memory as the original one

    assert(asCopy(NULL)==NULL);
    AmountSet copied = asCopy(newlist);

    SetX((Point*)asGetFirst(newlist), 11);
    SetX((Point*)asGetNext(newlist), 12);
    SetX((Point*)asGetNext(newlist), 13);
    SetX((Point*)asGetNext(newlist), 14);
    SetX((Point*)asGetNext(newlist), 15);
    SetX((Point*)asGetNext(newlist), 16);
    SetX((Point*)asGetNext(newlist), 17);
    SetX((Point*)asGetNext(newlist), 18);
    SetX((Point*)asGetNext(newlist), 19);
    SetX((Point*)asGetNext(newlist), 20);
    SetX((Point*)asGetNext(newlist), 21);
    SetX((Point*)asGetNext(newlist), 22);
    SetX((Point*)asGetNext(newlist), 23);
    SetX((Point*)asGetNext(newlist), 24);
    SetX((Point*)asGetNext(newlist), 25);
    SetX((Point*)asGetNext(newlist), 26);

    assert(GetX((Point*)asGetFirst(copied))==1);
    assert(GetX((Point*)asGetNext(copied))==2);
    assert(GetX((Point*)asGetNext(copied))==3);
    assert(GetX((Point*)asGetNext(copied))==4);
    assert(GetX((Point*)asGetNext(copied))==5);
    assert(GetX((Point*)asGetNext(copied))==6);
    assert(GetX((Point*)asGetNext(copied))==7);
    assert(GetX((Point*)asGetNext(copied))==8);
    assert(GetX((Point*)asGetNext(copied))==9);
    assert(GetX((Point*)asGetNext(copied))==10);
    assert(GetX((Point*)asGetNext(copied))==11);
    assert(GetX((Point*)asGetNext(copied))==12);
    assert(GetX((Point*)asGetNext(copied))==13);
    assert(GetX((Point*)asGetNext(copied))==14);
    assert(GetX((Point*)asGetNext(copied))==15);
    assert(GetX((Point*)asGetNext(copied))==16);

    asDestroy(copied);
    printf ("COPY FUNCTION WORKING!\n");

    //newlist{11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26}

    //Checking contains function


    assert(asContains(newlist,p44)==true);//14
    assert(asContains(newlist,p45)==true);//15
    assert(asContains(newlist,p42)==true);//12
    assert(asContains(newlist,p44)==true);//14
    assert(asContains(newlist,p41)==true);//11
    assert(asContains(newlist,p40)==false);//no 10
    assert(asContains(newlist,p38)==false);//no 9
    assert(asContains(newlist,p37)==false);//no 8
    assert(asContains(newlist,p36)==false);//no 7
    assert(asContains(newlist,p35)==false);//no 6
    assert(asContains(newlist,p34)==false);//no 5
    assert(asContains(newlist,p33)==false);//no 4
    assert(asContains(newlist,p32)==false);//no 3
    assert(asContains(newlist,p31)==false);//no 2
    assert(asContains(newlist,p30)==false);//no 1
    assert(asContains(newlist,NULL)==false);
    assert(asContains(NULL,p43)==false);
    assert(asContains(NULL,NULL)==false);
    asDelete(newlist, p44);//delete 14
    asDelete(newlist, p45);//delete 15
    asDelete(newlist, p42);//delete 12
    asDelete(newlist, p41);//delete 11
    assert(asDelete(newlist, p40)==AS_ITEM_DOES_NOT_EXIST);//delete no 10
    assert(asContains(newlist,p44)==false);
    assert(asContains(newlist,p45)==false);
    assert(asContains(newlist,p42)==false);

    printf("CONTAINS FUNCTION WORKING!\n");

    asClear(newlist);
    assert(asGetSize(newlist)==0);
    asDestroy(newlist);

//more tests

    printf("checking some more edge cases\n");

    AmountSet lis = asCreate(NULL,freeInt,compareInts);
    assert(lis==NULL);
    lis = asCreate(copyInt,NULL,compareInts);
    assert(lis==NULL);
    lis = asCreate(copyInt,freeInt,NULL);
    assert(lis==NULL);

    lis = asCreate(copyInt,freeInt,compareInts);
    freeElement(p1);
    p1 = create(-7);
    asRegister(lis,p1);//-7
    assert(asGetSize(lis)==1);
    asDelete(lis, p1);//delete -7
    assert(asGetSize(lis)==0);

    freeElement(p1);
    freeElement(p2);
    freeElement(p3);
    p1=create(8);
    p2=create(8);
    p3=create(8);

    asRegister(lis,p1);//8
    asRegister(lis,p2);//re 8
    asRegister(lis,p3);//re 8

    assert(asGetSize(lis)==1);

    freeElement(p4);
    freeElement(p5);
    freeElement(p6);
    freeElement(p7);
    freeElement(p8);
    freeElement(p9);
    freeElement(p10);
    freeElement(p11);

    p4=create(4);
    p5=create(2);
    p6=create(6);
    p7=create(7);
    p8=create(1);
    p9=create(3);
    p10=create(10);
    p11=create(13);

    asRegister(lis,p4);//4
    asRegister(lis,p5);//2
    asRegister(lis,p6);//6
    asRegister(lis,p7);//7
    asRegister(lis,p8);//1
    asRegister(lis,p9);//3
    asRegister(lis,p11);//13
    //lis{1,2,3,4,6,7,8,13}
    assert(asGetSize(lis)==8);
    assert((GetX(asGetFirst(lis))==1));


    asClear(lis);
    assert(asDelete(lis, p9)==AS_ITEM_DOES_NOT_EXIST);
    assert(asGetFirst(lis)==NULL);
    assert(asGetNext(lis)==NULL);

    asDestroy(lis);

    freeElement(p1);
    freeElement(p2);
    freeElement(p3);
    freeElement(p4);
    freeElement(p5);
    freeElement(p6);
    freeElement(p7);
    freeElement(p8);
    freeElement(p9);
    freeElement(p10);
    freeElement(p11);
    freeElement(p12);
    freeElement(p13);
    freeElement(p14);
    freeElement(p15);
    freeElement(p16);
    freeElement(p17);
    freeElement(p18);
    freeElement(p19);
    freeElement(p20);
    freeElement(p21);
    freeElement(p22);
    freeElement(p30);
    freeElement(p31);
    freeElement(p32);
    freeElement(p33);
    freeElement(p34);
    freeElement(p35);
    freeElement(p36);
    freeElement(p37);
    freeElement(p38);
    freeElement(p40);
    freeElement(p41);
    freeElement(p42);
    freeElement(p43);
    freeElement(p44);
    freeElement(p45);
    freeElement(p46);

    printf("\n");
    printf("\n");
    printf("\n");
    printf("############################################################################\n");
    printf("############################################################################\n");
    printf("############################################################################\n");
    printf("##############################congratulations!##############################\n");
    printf("############################################################################\n");
    printf("############################################################################\n");







}
