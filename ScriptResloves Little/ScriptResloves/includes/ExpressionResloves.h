#ifndef __EXPRESSIONRESLOVES_H
#define __EXPRESSIONRESLOVES_H

#include "../../../common/StringOperation.h"
#include "NumSet.h"
#include "Order.h"
using namespace std;
//#include "Numset.h"
//#include<stdio.h>
//#include "../../common/HashMap.h"


/*
时间：2018 9 15
作者：latefly
功能：
普通的脚本解析基本的就是将一个中缀式转化为后缀式
例如
1+3*(12*3+2)
a+b*c*(d+e*add(c+a,d*e))
转化为
1 3 12 3*+2+

a b c* d e add(c+a,d*e)*++


注意这里的只是字符的转换而不是真正的运算式的转化
两个变量之间用空格符隔开，
所以所给出的表达式中不能含有空格，这可以去除空格
变量名可以是字符下划线

editVersion:0.0.01

*/

typedef struct
{
	char character;
	char priority;
}__Operator;
__Operator Operator[10] = {
'+',0,
'-',0,
'*',1,
'/',1,
'(',2,
')',-1,
'<',-2,
'>',-2,
'=',-3
};
char GetOperationPriority(char symbol)
{
	char priority;

	for (priority = 0; priority < 10; priority++)
	{
		if (Operator[priority].character == symbol)
		{
			priority = Operator[priority].priority;
			break;
		}

	}
	return priority;

}
char IsOperation(char symbol)
{
	char isOperation = 0, i;

	for (i = 0; i < 10; i++)
	{
		if (symbol == Operator[i].character)
		{
			isOperation = 1;
			break;
		}
	}
	return isOperation;
}
//此处的转换只能对正确的表达式进行的不包含检错机制
char ExpressionResloves(char *s)
{
	char Error = 1, *p = s, result;
	char Operation[20] = "\0";//储存临时的操做
	char temp[100] = "\0", *st = temp, *op = Operation, resultOp;
	while (*s != '\0')
	{
		if ((*s >= 'A'&&*s <= 'Z') || (*s >= 'a'&&*s <= 'z') || *s == '_')//匹配到变量的开头，可以是函数数字
		{
			/*************此处需要判别是否是函数或者是变量**************/

			while ((*s >= 'A'&&*s <= 'Z') || (*s >= 'a'&&*s <= 'z') || *s == '_' || IsNum(*s))
			{
				*st = *s;
				s++;
				st++;
				if (*s == '(')
				{
					while (*s != ')')//此处在输入出错的时候会越界
					{
						*st = *s;
						s++;
						st++;
					}
					*st = *s;
					st++;
					s++;
				}
			}
			s--;
			*st = ' ';
			st++;

		}
		else if (*s >= '0'&&*s <= '9')
		{

			while (*s >= '0'&&*s <= '9')
			{
				*st = *s;
				s++;
				st++;
			}
			*st = ' ';
			st++;
			s--;


		}
		else//此处判断是否有运算符
		{
			/*******************进行栈操作******************************/
			if (IsOperation(*s))
			{
				if (*op != '\0')
				{

					resultOp = GetOperationPriority(*op);
					if (resultOp == 2)
					{
						op++;
						*op = *s;
					}
					else if (GetOperationPriority(*s) == -1)
					{
						//printf("this %c\n",*s);
						//st--;
						while (*op != '(')
						{
							*st = *op;
							st++;
							*st = ' ';
							st++;
							op--;
						}
						//printf("op:%c\n",*op);
						op--;
					}
					else if (resultOp >= GetOperationPriority(*s))
					{
						*st = *op;
						st++;
						*st = ' ';
						st++;
						*op = *s;
					}
					else if (resultOp < GetOperationPriority(*s))
					{
						op++;
						*op = *s;
					}
				}
				else
				{
					op++;//跳过第一个转移栈底
					*op = *s;
				}
			}

		}
		if (*s == ' ')break;
		s++;
	}
	while (*op != '\0')
	{
		*st = *op;
		st++;
		*st = ' ';
		op--;
		st++;
	}
	StringCopy(temp, p);
	//printf("temp:%s\n",temp);
	//printf("Operation:%s\n",Operation);
	return Error;


}
//测试方法
void PrintStack(double *stack)
{
	int a = 0;
	printf("\n");
	while (a<10)
	{
		printf("\t%.2f",*stack);
		stack++;
		a++;
	}
	printf("\n");
}
double GetExpressionResult(char *s)
{
	double result = 0;


	return result;
}



#endif // !__EXPRESSIONRESLOVES_H


