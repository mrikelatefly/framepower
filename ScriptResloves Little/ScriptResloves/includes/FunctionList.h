#ifndef     __FUNCTIONLIST_H
#define     __FUNCTIONLIST_H
#include<stdlib.h>
#include "../../../common/StringOperation.h"



#define FunctionMaxValue 100
/************************
时间：2018 10 4
作者：latefly
功能：这是提供给FunctionLittle的数据结构存储的文件
eiditVersion:0.0.01
使用规范 10 8
只能使用int，char类型的数据以及字符串的内容
eiditVersion:0.0.02
使用规范10 9
需要注意所有的加入调用的函数需要在加入的时候做一定的转换
如下

char Test(int a,double b)
{
    .
    .
    .
    .

}
转化为
char Test(char *Inta,char *Doubleb)
{
    int a;
    doouble b;
    a=IntString(Inta);
    b=DoubleString(Doubleb);
    //下面的代码是一样的
    .
    .
    .
    .

}
************************/


typedef struct
{
   char FunctionName[50];
   void *FunctionPointer;
}__FunctionList;

typedef struct
{
    char FunctionName[30];//此处是初始化以后分离出的函数名
    void *FunctionPointer;
    int id;
    char arguments;//这里讨论能否直接不用参数的个数而直接转化得到
}__FunctionFrame;


/*****************************************测试函数*********************************************/
void charInt(char *Chara,char *Intb)
{
    char a=*Chara;
    int b=IntString(Intb);
	printf("a=%c,b=%d\n",a,b);
}
void charChar(char *Chara,char *Charb)
{
    char a=*Chara;
    char b=*Charb;
	printf("a=%c,b=%c\n",a,b);
}
void IntInt(char *Inta,char *Intb)
{
    int a=IntString(Inta);
    int b=IntString(Intb);
	printf("a=%d,b=%d\n",a,b);
}
void doubleInt(char *Floata,char *Intb)
{
    float a=FloatString(Floata);
    int b=IntString(Intb);
	printf("a=%lf,b=%d\n",a,b);
}
/*******************************************测试函数******************************************/
//char functionBuff[300];
__FunctionList FunctionList[FunctionMaxValue]={
    "void charInt(char a,int b)",charInt,
    "void charChar(char a,char b)",charChar,
    "void IntInt(int a,int b)",IntInt,
    "void doubleInt(double a,int b)",doubleInt
    };
__FunctionFrame FunctionFrame[FunctionMaxValue]={0,0,0,0};//这里进行预初始化





#endif // __FUNCTIONLIST_H
