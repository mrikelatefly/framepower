#ifndef     __FUNCTIONLIST_H
#define     __FUNCTIONLIST_H
#include<stdlib.h>
#include "../../../common/StringOperation.h"



#define FunctionMaxValue 100
/************************
ʱ�䣺2018 10 4
���ߣ�latefly
���ܣ������ṩ��FunctionLittle�����ݽṹ�洢���ļ�
eiditVersion:0.0.01
ʹ�ù淶 10 8
ֻ��ʹ��int��char���͵������Լ��ַ���������
eiditVersion:0.0.02
ʹ�ù淶10 9
��Ҫע�����еļ�����õĺ�����Ҫ�ڼ����ʱ����һ����ת��
����

char Test(int a,double b)
{
    .
    .
    .
    .

}
ת��Ϊ
char Test(char *Inta,char *Doubleb)
{
    int a;
    doouble b;
    a=IntString(Inta);
    b=DoubleString(Doubleb);
    //����Ĵ�����һ����
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
    char FunctionName[30];//�˴��ǳ�ʼ���Ժ������ĺ�����
    void *FunctionPointer;
    int id;
    char arguments;//���������ܷ�ֱ�Ӳ��ò����ĸ�����ֱ��ת���õ�
}__FunctionFrame;


/*****************************************���Ժ���*********************************************/
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
/*******************************************���Ժ���******************************************/
//char functionBuff[300];
__FunctionList FunctionList[FunctionMaxValue]={
    "void charInt(char a,int b)",charInt,
    "void charChar(char a,char b)",charChar,
    "void IntInt(int a,int b)",IntInt,
    "void doubleInt(double a,int b)",doubleInt
    };
__FunctionFrame FunctionFrame[FunctionMaxValue]={0,0,0,0};//�������Ԥ��ʼ��





#endif // __FUNCTIONLIST_H
