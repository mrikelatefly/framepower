#ifndef		__NUMSET_H
#define		__NUMSET_H

#include "../../../common/StringOperation.h"
#include "Order.h"
#include<stdio.h>
using namespace std;

/************************************************************************
时间：2018 9 18
作者：latefly
功能：如下

设计使用规则：

添加变量
VaryableName=[0-9]|[0-9-a-z]

删除变量
delete VaryableName

访问变量
VaryableName

修改变量(注意这里的变量会自动匹配变量类型)
VaryableName=[0-9]*|[0-9-a-z]*

ShowAll 格式
变量名 类型 值 地址



这些基本的操作只需要将变量的地址加入到要操作的链表中就可以进行操作

对于一个输入的一般的分解步骤
1 拆分为命令 操作 变量
2 将拆分的结果保存到数组
3 调用函数进行操作


editVersion:0.0.01 9 18
editVersion:0.0.02 9 23
editVersion:0.0.02 10 8
标准化输入
editVersion:0.0.03 10 9
***************************************************************************/

#define Num_Set_Size  50
//#define StringBuff_Deep Num_Set_Size
//#define StringBuff_Size 30



//在外界可以直接分配变量进入
/********************************************************************************************
mark：
	标记信息：
		是否使用：标记是否有被使用
		类型：float double int char String(虽然储存都是以double储存的这里就是读取的方式不同)
		创建方式：加入式，还是内置式加入式的
		第一位：是否使用
		第二位：创建类型 1,为内置型，0为外置型
		第三位：是否为char型
		第四位：是否为int型
		第五位：是否为double型
		第六位：是否为String型
		其余位作用预留

varyName[15];变量名
*vary; 内置加入式传入地址以直接改变改变量的值
value; 外加以及其他加入的都是存储在这里的
*s;String类型的在此处储存，外置用以申请储存

创建的原则
内部只允许创建double类型其余的变量类型不允许在内部方法中使用
这也就是说内部创建只有种形式
即NumSetCreateOrSet(varyableName,Vary,0)


*********************************************************************************************/

typedef struct
{
	char mark;//包含是否使用，类型，创建等信息
	char varyName[15];
	double *vary;
	double value;
	char source[30];
}__Num_Set;

__Num_Set Num_Set[Num_Set_Size] = { 0,0,0,0,0 };
//char StringBuff[StringBuff_Deep][StringBuff_Size]={'\0'};

/*********************错误定义******************/
//执行成功
#define     Successful 0x00
//创建错误
#define 	MutilName 0x01
#define		OutOfSize 0x02
//访问错误 删除错误 修改错误
#define		HasNotThisVary 0x03



char GetVaryType(char *vary);
double NumSetGetValue(char *varyableName);
//查是否有该变量，若有则修改,无则创建,首先检查content中的内容若为空则加入地址
char NumSetCreateOrSet(char *varyableName, double *Vary, char *content)
{
	char isCreate = 0, *s;
	int i, Unused = 0, test = 0;
	//__Num_Set *p=;
	//printf("Vary: %lf\n",*Vary);
	for (i = 0; i < Num_Set_Size; i++)
	{
		if (!(0x01 & Num_Set[i].mark))//快速匹配到未使用的变量
		{
			Unused = i;
			break;
		}
	}
	for (i = 0; i < Num_Set_Size; i++)
	{
		if (StringEqual(Num_Set[i].varyName, varyableName))
		{
			//仍然需要检查是否有被使用
			Num_Set[i].mark = 0x00;//重新写入类型以及使用情况
			Num_Set[i].mark |= 0x01;//标记使用
			if (content == 0)//加入的为内置数
			{
				//printf("I am here did you see me1!\n");
				Num_Set[i].mark |= 0x10;
				Num_Set[i].mark |= 0x02;
				Num_Set[i].vary = Vary;
			}
			else//外置变量加入
			{
				if (*content <= '9'&&*content >= '0')//输入为数
				{
					Num_Set[i].value = DoubleString(content);
					test = (int)Num_Set[i].value;
					if (Num_Set[i].value - test == 0)
					{
						//默认的类别为外置变量所以这里不用更改
						Num_Set[i].mark |= 0x08;
					}
					else
					{
						Num_Set[i].mark |= 0x10;
					}
				}
				else//输入为字符串
				{
					if (*content == '\'' || *content == '"')
					{
						StringCopy(content, Num_Set[i].source);
						Num_Set[i].mark |= 0x04;
					}
					else//加入的以前的变量
					{
						/*******************************注意这里变量加入的时候要检查是否是已加入的变量*********************************/

						Num_Set[i].mark |= 0x10;
						Num_Set[i].value = NumSetGetValue(content);
						/*
						 Num_Set[i].mark|=GetVaryType(content);//外置的double类型
						 if( Num_Set[i].mark&0x20)//String类型
						 {

						 }
						 else
						 {
							 Num_Set[i].value=NumSetGetValue(content);
						 }
						*/
					}

				}
			}
			break;
		}
	}
	if (i == Num_Set_Size)//没有同名的变量
	{
		if (Unused == Num_Set_Size)
		{
			return OutOfSize;
		}
		else
		{
			//仍然需要检查是否有被使用
			Num_Set[Unused].mark = 0x00;//重新写入类型以及使用情况
			Num_Set[Unused].mark |= 0x01;//标记使用
			StringCopy(varyableName, Num_Set[Unused].varyName);
			if (content == 0)//加入的为内置数
			{
				//printf("I am here did you see me2!\n");
				Num_Set[Unused].mark |= 0x10;
				Num_Set[Unused].mark |= 0x02;
				Num_Set[Unused].vary = Vary;
				//printf("VaryName:%s\n",Num_Set[Unused].varyName);
				//printf("NumSet:%lf\n",*(Num_Set[Unused].vary));
			}
			else//外置变量加入
			{
				//printf("**********************!\n");
				if (*content <= '9'&&*content >= '0')//输入为数
				{
					Num_Set[Unused].value = DoubleString(content);
					test = (int)Num_Set[Unused].value;
					if (Num_Set[Unused].value - test == 0)
					{
						//默认的类别为外置变量所以这里不用更改
						Num_Set[Unused].mark |= 0x08;
					}
					else
					{
						Num_Set[Unused].mark |= 0x10;
					}
				}
				else//输入为字符串
				{

					if (*content == '\'' || *content == '"')
					{
						StringCopy(content, Num_Set[Unused].source);
						Num_Set[Unused].mark |= 0x04;
					}
					else
					{
						/*******************************注意这里变量加入的时候要检查是否是已加入的变量*********************************/
						Num_Set[Unused].mark |= 0x10;//外置的double类型
						Num_Set[Unused].value = NumSetGetValue(content);
					}

				}
			}
		}
	}
	return isCreate;
}

char NumSetDelete(char *varyableName)
{
	char isDelete = 1, *p;
	int i;
	//__Num_Set *p=;
	//printf("I have Delete the varyable 1\n");
	for (i = 0; i < Num_Set_Size; i++)
	{
		if ((Num_Set[i].mark) & 0x01)//使用的时候回收
		{
			if (StringEqual(varyableName, Num_Set[i].varyName))//快速匹配到未使用的变量
			{
				//内容回收
				Num_Set[i].mark = 0x00;
				Num_Set[i].vary = 0;
				*(Num_Set[i].source) = '\0';
				//p=&(Num_Set[i].source);//虽然有修改但是这里为什么要取地址有待考量
				//*p='\0';
				//*(Num_Set[i].source)=0;
				break;
			}
		}
	}
	if (i == Num_Set_Size)
	{
		//printf("I have Delete the varyable 4\n");
		return HasNotThisVary;
	}
	return isDelete;
}

char GetVaryType(char *vary)
{
	char type = 0;
	int i;
	for (i = 0; i < Num_Set_Size; i++)
	{
		if (Num_Set[i].mark & 0x01)
		{
			if (StringEqual(vary, Num_Set[i].varyName))
			{
				type = Num_Set[i].mark & 0xfe;
				break;
			}
		}
	}
	return type;
}


void *GetVaryable(char *varyableName)
{
	//这里需要获取类型然后返回相应的类型
	char type;
	int i;
	//type=GetVaryableType(varyableName);
	for (i = 0; i < Num_Set_Size; i++)
	{
		if (Num_Set[i].mark & 0x01)
		{
			if (StringEqual(varyableName, Num_Set[i].varyName))//快速匹配到未使用的变量
			{
				//内容回收
				//printf("I hava get this vName:%s\n",Num_Set[i].varyName);
				type = (Num_Set[i].mark & 0xfc);
				break;
			}
		}
	}
	if (i == Num_Set_Size)
	{
		return 0;
	}
	if (type & 0x02)//内置
	{
		return (Num_Set[i].vary);
	}
	else//外置
	{
		switch (type)
		{
		case(0x04):; break;//保留部分没有这种创建的方式
		case(0x08):;
		case(0x10):return &(Num_Set[i].value); break;
		case(0x12):return Num_Set[i].vary; break;
		case(0x20):return Num_Set[i].source; break;
		}
	}

}

double NumSetGetValue(char *varyableName)
{
	//这里需要获取类型然后返回相应的类型
	char type;
	int i;
	double result = 0.0;
	//type=GetVaryableType(varyableName);
	for (i = 0; i < Num_Set_Size; i++)
	{
		if (Num_Set[i].mark & 0x01)//标记使用
		{
			if (StringEqual(varyableName, Num_Set[i].varyName))
			{
				type = (Num_Set[i].mark & 0xfe);
				switch (type)
				{
				case(0x04):result = Num_Set[i].source[1]; break;//保留部分没有这种创建的方式
				case(0x08):result = (int)(Num_Set[i].value);
				case(0x10):result = Num_Set[i].value; break;
				case(0x12):result = *(Num_Set[i].vary); break;
				}
				break;
			}
		}
	}
	return result;
}
//此处需要外部实现
void ShowVaryable(char *order, char *varyableName)
{

	int i;
	printf("\n");
	for (i = 0; i < Num_Set_Size; i++)
	{
		if (Num_Set[i].mark & 0x01)
		{
			if (order == 0)
			{
				//CheckBit(Num_Set[i].mark);
				if (StringEqual(Num_Set[i].varyName, varyableName))
				{
					switch (Num_Set[i].mark & 0xfe)
					{
					case(0x12):printf("%s:%lf\n", Num_Set[i].varyName, *(Num_Set[i].vary)); break;
					case(0x04):printf("%s:%s\n", Num_Set[i].varyName, Num_Set[i].source); break;
					case(0x08):printf("%s:%d\n", Num_Set[i].varyName, (int)(Num_Set[i].value)); break;
					case(0x10):printf("%s:%lf\n", Num_Set[i].varyName, (Num_Set[i].value)); break;
					case(0x20):printf("%s:%s\n", Num_Set[i].varyName, Num_Set[i].source); break;
						//case(0x12):printf("%lf\n",*(Num_Set[i].vary));break;
					}
				}
			}
			else if (StringEqual(Order[0], order))
			{
				switch (Num_Set[i].mark & 0xfe)
				{
				case(0x12):printf("%s:%lf\n", Num_Set[i].varyName, *(Num_Set[i].vary)); break;
				case(0x04):printf("%s:%s\n", Num_Set[i].varyName, Num_Set[i].source); break;
				case(0x08):printf("%s:%d\n", Num_Set[i].varyName, (int)(Num_Set[i].value)); break;
				case(0x10):printf("%s:%lf\n", Num_Set[i].varyName, (Num_Set[i].value)); break;
				case(0x20):printf("%s:%s\n", Num_Set[i].varyName, Num_Set[i].source); break;

					/*
						case(0x12):printf("%s:%lf\n",Num_Set[i].varyName,(double *)GetVaryable(Num_Set[i].varyName));break;
						case(0x04):printf("%s:%s\n",Num_Set[i].varyName,(char *)GetVaryable(Num_Set[i].varyName));break;
						case(0x08):printf("%s:%d\n",Num_Set[i].varyName,(int)(*((double *)GetVaryable(Num_Set[i].varyName))));break;
						case(0x10):printf("%s:%lf\n",Num_Set[i].varyName,(double)(*(double *)(GetVaryable(Num_Set[i].varyName))));break;
						case(0x20):printf("%s:%s\n",Num_Set[i].varyName,(char *)GetVaryable(Num_Set[i].varyName));break;
					*/
					//case(0x12):printf("%lf\n",*(Num_Set[i].vary));break;
				}
			}

		}

	}
	printf("\n");
}
//下面就正式进行设置
char NumSet(char *s)
{
	char Error = 0;
	if (HasSymbol('=', s))
	{
		SplitString('=', s);
		NumSetCreateOrSet(StringOperation_Buff[0], 0, StringOperation_Buff[1]);
	}
	ShowVaryable(0, StringOperation_Buff[0]);
	return Error;
}


/*void CheckBit(char data)
{
	int i=0;
	for(i=0;i<8;i++)
	{
		printf("%d",0x01&data);
		data>>=1;
	}
	printf("\n");


}*/
/**********************************************************************************
	测试项:
	1 创建
		int 内置创建，外部创建
		double 内部外部创建
		String 外部创建
	2 修改
		内置值的修改
			int double
		外置
			int
			double
			String
	3 访问
			int
			double
			String
	4 删除（韩内外置）
			int
			double
			String

	测试问题
	int double内部变量加入会出现值未载入的问题
	删除变量没有变化

	外置加入int变量时值未加入或者访问不到
	删除变量貌似都有问题
**********************************************************************************/

/*void main()
{
	char arr[100];
	int a=10,*p=&a;
	double b=128.20;
	while(1)
	{
		cin>>arr;
		if(StringEqual(Order[1],arr))break;
		NumSet(arr);
		SplitString('=',arr);
		ShowVaryable(Order[0],0);
		printf("%lf",NumSetGetValue(StringOperation_Buff[0]));
	}


}
*/

#endif