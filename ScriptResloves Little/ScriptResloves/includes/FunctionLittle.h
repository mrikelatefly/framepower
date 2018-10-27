#ifndef __FUNCTIONLITTLE_H
#define	__FUNCTIONLITTLE_H
//#include<stdio.h>
#include "FunctionList.h"
#include "Order.h"
#include "../../../common/StringOperation.h"
#include "Numset.h"
#include "ExpressionResloves.h"
#include "FunctionList.h"
//#include "pack.h"
//#include<stdio.h>


#define FunCBuff(x) StringOperation_Buff[x]
/*
时间：2018 9 23
作者：latefly
功能：一个函数调用框架

editVersion：0.0.01 9 23 怎样获取参数的讨论
editVersion：0.0.02 10 4  参数转化的具体实现

*/
/***********************************************************************************************************
思路
1 将函数名储存
2 通过输入内容查找函数指针
3 通过注册信息对输入参数进行相应的转换
4 对待指针参数的处理
5 在完成以上内容后能否对动态的函数创建有一个了解


一般以void*强制转换一个函数的时候加入的数据会被自动的再次强制转换到原来的数据类型
但是这里只会用double的类型
所以这里的直接用统一的类型直接加入到强制转化的函数中
对于指针的情况另再讨论


通过一些简单的基础的分解得到一些判据的基础
希望的是通过形式判定
输入
add(123,456)
我们总是希望能够直接对应的输入输出，但是这中间有许多的参数的转化
要把字符转化为数字，或者是字符串（这时其实是一个地址）

除了这些以外还得解决一些问题
1.输入为String类型 即'letter'
2.输入为num
3.输入为表达式 a+33*b




***********************************************************************************************************/





/******************************************会出现很蹩脚的地址选项*********************************************/
/*
void *StringConvert(char *s)
{
	int num=0;
	while(*s!='\0')
	{
		if(*s-'0'>=0&&*s-'0'<=9)
		{
			num*=10;
			num+=(int)(*s-'0');
		}
		else
		{
			s++;
			if(*s!='\0')//给多字符转化的提供帮助
				return 0;
			else
			{
				s--;
				return *s;//这里只是对应单个的字符转换的的结果
			}
		}

		s++;
	}
	return num;
}
*/



//此处将FunctionList中的内容加载到FunctioFrame中
void FunctionFrameInit()
{
	void *p;
	char argumets = 0;
	int i = 0, j = 0;
	for (i = 0; i < FunctionMaxValue; i++)
	{
		if (FunctionList[i].FunctionPointer != 0)//注意这里的指针判断否则会强制退出
		{
			StringBuffEmpty();//清除缓冲区避免干扰
			SplitString(' ', FunctionList[i].FunctionName);//这里处理会有些麻烦还可能会越界
			argumets = GetStringBuffUsedNum();
			SplitString('(', StringOperation_Buff[1]);
			StringCopy(StringOperation_Buff[0], FunctionFrame[j].FunctionName);

			FunctionFrame[j].FunctionPointer = FunctionList[i].FunctionPointer;
			FunctionFrame[j].id = j;
			FunctionFrame[j].arguments = argumets-2;
			printf("FuncName:%s \targuments:%d funcId:%d\n", FunctionFrame[j].FunctionName, FunctionFrame[j].arguments, FunctionFrame[j].id);
			j++;

		}

	}
}
//这将返回值以备后来调用使用
/*
注意这里的关于double类型数据的函数调用会出错

*/
double FunctionLittle(char *inputs)
{
	int i, sr, out[10] = { 0 };
	double result = 0;
	//SplitString(' ',inputs);//这里处理会有些麻烦还可能会越界
	SplitString('(', inputs);

	for (i = 0; i < FunctionMaxValue; i++)
	{
		if (FunctionFrame[i].FunctionPointer != 0)
		{
			//printf("FunctionName:%s\n",StringOperation_Buff[0]);
			if (StringEqual(StringOperation_Buff[0], FunctionFrame[i].FunctionName))
			{
				//printf("FunctionName:%s\n",StringOperation_Buff[0]);
/******************************这里现在只是单个变量的情况而不是表达式所以这里还需要转化的步骤******************************************/

				SplitString(',', StringOperation_Buff[1]);
				for (sr = 0; sr < FunctionFrame[i].arguments; sr++)
				{
					StringTrim(')', StringOperation_Buff[sr]);
					//out[sr]=IntString(StringOperation_Buff[sr]);
					//printf("%c\n",out[sr]);
				}

				//out[0] = 10.1;
				printf("%d\n", FunctionFrame[i].arguments);
				switch (FunctionFrame[i].arguments)
				{
				case(0):
					result = (*((double(*)())(FunctionFrame[i].FunctionPointer)))();
					break;
				case(1):
					result = (*((double(*)(char *s1))(FunctionFrame[i].FunctionPointer)))(FunCBuff(0));
					break;
				case(2):
					result = (*((double(*)(char *s1, char *s2))(FunctionFrame[i].FunctionPointer)))(FunCBuff(0), FunCBuff(1));
					break;
				case(3):
					result = (*((double(*)(char *s1, char *s2, char *s3))(FunctionFrame[i].FunctionPointer)))(FunCBuff(0), FunCBuff(1), FunCBuff(2));
					break;
				case(4):
					result = (*((double(*)(char *s1, char *s2, char *s3, char *s4))(FunctionFrame[i].FunctionPointer)))(FunCBuff(0), FunCBuff(1), FunCBuff(2), FunCBuff(3));
					break;
				case(5):
					result = (*((double(*)(char *s1, char *s2, char *s3, char *s4, char *s5))(FunctionFrame[i].FunctionPointer)))(FunCBuff(0), FunCBuff(1), FunCBuff(2), FunCBuff(3), FunCBuff(4));
					break;
				case(6):
					result = (*((double(*)(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6))(FunctionFrame[i].FunctionPointer)))(FunCBuff(0), FunCBuff(1), FunCBuff(2), FunCBuff(3), FunCBuff(4), FunCBuff(5));
					break;
				case(7):
					result = (*((double(*)(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7))(FunctionFrame[i].FunctionPointer)))(FunCBuff(0), FunCBuff(1), FunCBuff(2), FunCBuff(3), FunCBuff(4), FunCBuff(5), FunCBuff(6));
					break;
				case(8):
					result = (*((double(*)(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7, char *s8))(FunctionFrame[i].FunctionPointer)))(FunCBuff(0), FunCBuff(1), FunCBuff(2), FunCBuff(3), FunCBuff(4), FunCBuff(5), FunCBuff(6), FunCBuff(7));
					break;
				case(9):
					result = (*((double(*)(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7, char *s8, char *s9))(FunctionFrame[i].FunctionPointer)))(FunCBuff(0), FunCBuff(1), FunCBuff(2), FunCBuff(3), FunCBuff(4), FunCBuff(5), FunCBuff(6), FunCBuff(7), FunCBuff(8));
					break;
				case(10):
					result = (*((double(*)(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7, char *s8, char *s9, char *s10))(FunctionFrame[i].FunctionPointer)))(FunCBuff(0), FunCBuff(1), FunCBuff(2), FunCBuff(3), FunCBuff(4), FunCBuff(5), FunCBuff(6), FunCBuff(7), FunCBuff(8), FunCBuff(9));
					break;
				default:return 0;//调用失败
				}
				break;
			}

		}
	}
	return result;
}


/*int main()
{

	//int a = 10;
	char arr[100] = "afjaejfeja";
	//printf("%lf\n",c);
	//(*((int(*)(int a))p))(a);
	FunctionFrameInit();
	printf("Input character to start this test!\n");
	while (1)
	{
		cin >> arr;
		//scanf("%s",arr);
		if (StringEqual(Order[exit], arr))break;
		FunctionLittle(arr);
	}


	return 0;
}*/

#endif // !__FUNCTIONLITTLE_H



