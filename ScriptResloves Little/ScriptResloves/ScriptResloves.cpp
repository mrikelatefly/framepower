#include "pch.h"
#include <iostream>
#include "includes/Order.h"
#include "../../common/StringOperation.h"
#include "includes/Numset.h"
#include "includes/ExpressionResloves.h"
#include "includes/FunctionList.h"
#include "includes/FunctionLittle.h"
#include<stdio.h>
using namespace std;

typedef struct
{
	char order[20];
	void *funp;
}__OrderList;
#define OrderList_Size	50

__OrderList OrderList[OrderList_Size] = {
"break",0,
"showall",0
};
//输入类型的区别
#define Orders 0x20
#define Function 0x21
#define ResultOfFunction 0x22
#define CreateOrSetVaryable 0x23
#define Expression 0x24


char ScriptResloves(char *s)
{
	char ScriptType = 0,Tacken[20];
	int couter = 0;
	double	Stack[20],*sp,temp;
	char Varyname[30][50] = {"\0"},vn=0;
	sp = Stack;
	/*
	1.首先判断是否是命令
	2.若不是命令则只是将其作为一般的等式来处理此时只需要进行系列的栈操作就可以得到结果
	*/
	SplitString(' ',s);
	for (couter=0; couter <OrderList_Size; couter++)
	{
		if (StringEqual(StringOperation_Buff[0], OrderList[couter].order))
		{
			ScriptType = Orders;
			cout << "this is  an ordder\n";
			return 0;//执行并返回
			break;
		}
	}
	//由于输入的数可以由NumSet获取或者是用函数得到其结果所以下面是数据操作
	ExpressionResloves(s);
/****************************此时处理数据************************/
	cout << "this has convertted!\n";
	cout << s<<endl;
	//由于是表达式这里的类型只有三种变量 数字 函数
	while (*s!='\0')
	{
		if (IsCharacter(*s))//函数或者是变量
		{
			for (couter = 0; *s != ' '; couter++)
			{
				Tacken[couter] = *s;
				if(*s!='\0')
				s++;
			}
			Tacken[couter] = '\0';
			if (HasSymbol('(', Tacken))
			{
				sp++;
				vn++;
				*sp = FunctionLittle(Tacken);
				cout << "Function:" << Tacken <<*sp<< endl;
			}
			else
			{
				sp++;
				*sp = NumSetGetValue(Tacken);
				StringCopy(Tacken, Varyname[vn]);
				vn++;
				cout << "Varyable:" << Tacken <<"  value:"<<*sp<< endl;
			}
		}
		else if(IsNum(*s))//数字
		{
			for (couter = 0; *s != ' '; couter++)
			{
				Tacken[couter] = *s;
				if (*s != '\0')
					s++;
			}
			Tacken[couter] = '\0';
			sp++;
			vn++;
			*sp = DoubleString(Tacken);
			cout << "Number:"<<*sp<< endl;
		}
		else if (IsOperator(*s))
		{

				cout << "This to start operation!" << endl;
				switch (*s)
				{
					/***********此处需要同时操作两个栈*********/
					case('+'):; break;
					case('-'):; break;
					case('*'):; break;
					case('/'):; break;
					default:
						break;
				}
				if (*s != '\0')
					s++;
		}
		else if (*s == '=')
		{
			temp = *sp;
			sp--;
			vn--;
			*sp = temp;
			StringDouble(*sp);
			NumSetCreateOrSet(Varyname[vn], 0, StringOperation_Buff[0]);
			cout << (int)vn<<Varyname[vn] <<":"<< *sp<<endl;
			if (*s != '\0')
				s++;
		}
		if (*s != '\0')
			s++;
	}

	return ScriptType;
}
int main()
{
	char input[50] = "\0";
	double result = 0;
	cout << "Input to start this test!" << endl;
	while (true)
	{
		cin >> input;
		if (StringEqual(Order[exit], input))break;
		ScriptResloves(input);
		/*
		NumSet(input);
		result = GetExpressionResult(input);
		cout << "result:" << result << endl;
		*/
	}
	return 0;
}