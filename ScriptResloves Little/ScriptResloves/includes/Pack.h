#ifndef __PACK_H
#include "StringOperation.h"
//#include "../../common/HashMap.h"
#include <stdio.h>


/*
较为长的时间来考虑如何来构建这样的一个结构
最终还是选用了最为简单的，可以灵活变换的一
种框架来容纳数据以及解析内容。
<package>
	<chain>
		放置解析链，描述，映射等
	</chain>
	<pack>
		放置数据以及信息描述
	</pack>
</package>


希望的是在底层库文件只用实现
open(),write()方法
在传输时只用实现
sent(),read();

此次修改于
2018 7 17

所有的chain里所包含的方法都是为了pack的open operate close等操作服务的


下面是pack的实现
底层实现接口
PackOpen()
PackWrite()
PackClose()

pack 自带的操作
注意这里的操作包含自结束的以及双标记的标签
PackSetLabelContent(Label,content)
PackGetLabel(Label)
PackAddNewLabel(Label,Content)
PackDeleteLabel(Label)


2018 7 23
利用二叉树建立节点网络
					Package
	  chain                      pack
	 .      .              Label1     port
   .  .  .  .         Label2  port
  .  . . .      Label3  .   .




标签的定义
format1
<Label discrible>
	content
</Label>

format2
<Label content/>
标签的定义也是标签拆分的一个依据另外结束符也是检查包的完整性的依据，标记不完整包将会直接丢弃



内容中不允许出现特殊的标记段落例如<vaefe/> >之类的
这里要在包的写入的时候做检查并更改为特殊的段落标记
&(<vaefe/> )  &(>)这样在处理的时候就不会出错

editversion：2018 9 15
这里不再使用全部解析的方式而是解析出chain和pack中的内容然后在进行其他的操作
而且解析时从最起先的内容
1 提取出pack，chain的内容
2 提供修改，添加标签的方法





*/
//分配4K的空间
#define PackInitSize 4096
//可容纳的数量包含节点数目
#define PackTreeSize 50
#define PackTreeUseTrack PackTreeSize/8+1
#define LabelType 0
#define PortType  1
typedef struct
{
	char Type;//0 Label 1 port
	int Offset;//标签的包含内容的偏移量
	char *LabelName;
	char *Content;
	struct __PackTree *Left;
	struct __PackTree *Right;

}__PackTree;


char PackTreeTrack[PackTreeUseTrack] = { 0x00 };
__PackTree PackTree[PackTreeSize];

char Pack[PackInitSize];



char pack[] = "aa&(ccc)<package version='0.1'>\n\t<chain>\n\t</chain>\n\t<pack>\n\t</pack>\n</package>\n<acc>I am here.</acc>\n<hello/>";







int CheckBit(char data)
{
	int i = 0, times = 0;
	for (i = 0; i < 8; i++)
	{
		printf("%d", 0x01 & (data >> 7));
		if (0x01 & (data >> 7))
			times++;
		data <<= 1;
	}
	printf("\n");

	return times;
}


int CheckDouble(double data)
{
	char *p = (char *)&data;
	int i = 0;
	for (i = 0; i < 8; i++)
	{
		CheckBit(*p);
		p++;
	}
	printf("\n");
	return i;

}

struct __PackTree *PackMalloc()
{

	int i = 0;
	char r = 0;
	for (i = 0; i < PackTreeSize; i++)
	{
		r = PackTreeTrack[i / 8];
		r >>= (i % 8);
		r &= 0x01;
		if (!r)
		{
			break;
		}

	}
	if (i == PackTreeSize)
	{
		return 0;
	}
	PackTreeTrack[i / 8] |= (1 << (i % 8));
	//printf("\n%d\n",i);
	return &PackTree[i];
	//printf("arr[%d]=%d\n",i/8,arr[i/8]);

}
char PackFree(int n)
{
	char isFree = 1;
	char r;
	r = PackTreeTrack[n / 8];
	r >>= (n % 8);
	r &= 0x01;
	if (r)
	{
		//清空所有内容只是作为实验的内容
		PackTree[n].Content = 0;
		PackTree[n].LabelName = 0;
		PackTree[n].Left = 0;
		PackTree[n].Right = 0;
		PackTree[n].Type = 0;
		PackTreeTrack[n / 8] &= ~(1 << (n % 8));
	}
	else
		isFree = 0;//未找到

	return isFree;
}
char PackTreeAdd(__PackTree *PackPointer, __PackTree *LastPack)
{
	char isAdd = 0;
	__PackTree *TempPack;

	if (LastPack->Left == NULL)
	{
		LastPack->Left = PackPointer;
	}
	else if (LastPack->Right == NULL)
	{
		LastPack->Right = PackPointer;
	}
	else
	{
		//此处建立的树可能不是太均衡
		TempPack = PackMalloc();
		TempPack->Left = LastPack->Right;
		TempPack->Type = PortType;
		TempPack->Right = PackPointer;
		LastPack->Right = TempPack;
	}
	return isAdd;
}
void PrintPackTree(__PackTree *PackPointer)
{
	int couter = PackPointer->Offset;
	char *p;
	if (PackPointer->Type != PortType)
	{
		p = PackPointer->LabelName;
		printf("\nLabelName:");
		while (*p != '>')
		{
			printf("%c", *p);
			p++;
			if (*p == ' ')break;
		}
		printf("\nthis Label Content:\n");
		p = PackPointer->Content;
		while (couter--)
		{
			printf("%c", *p);
			p++;
		}
		printf("\n");
	}
}
/*
函数调用必须是递归的这样才能一次性写入数据
*/





FILE *FileOpen(char *fileName, char *operation)
{
	//char isOpen=1;
	FILE *fp = NULL;
	if ((fp = fopen(fileName, operation)) == NULL)
	{
		//printf("Open files ERROR!\n");
		return NULL;
	}



	return fp;
}

char FileWrite(char *result)
{
	char isWrite = 1;
	FILE *fp = NULL;
	if ((fp = FileOpen("Pack", "w")) != NULL)
	{
		fprintf(fp, "%s", result);
		fclose(fp);
	}
	else
	{
		isWrite = 0;
	}
	return isWrite;
}

char FilePrint(char *pathAndName)
{
	char isPrint = 1, out;
	FILE *fp = NULL;
	if ((fp = FileOpen(pathAndName, "r")) != NULL)
	{
		while ((fscanf(fp, "%c", &out)) != EOF)
			printf("%c", out);
	}
	else
	{
		isPrint = 0;
	}
	return isPrint;
}



//此函数在打开一个pack以后自动生成一个解析树
//起始解析点 偏移量 上一节点地址
/*用于调节缓存的大小*/
#define LabelBuffSize 50
#define LabelStartSymbol '<'
#define LabelEndSymbol '>'
#define LabelSplitSymbol '/'
#define StringEndSymbol '\0'
#define SpecialBorderSymbol '&'
#define SpecialBorderStartSymbol '('
#define SpecialBorderEndSymbol ')'

void PrintLabel(char *info, char *s)
{
	printf("%s\n", info);
	while (*s != '>')
	{
		printf("%c", *s);
		s++;
	}
	printf("\n");
}
char PackAnalysis(char *p, int offset, __PackTree *LastPackTree)
{
	/*
	如果偏移量内存在标签就递归寻找标签内容是否含有标签
	标签的定义

	format1
	<Label discrible>
		content
	</Label>

	format2
	<Label content/>
	标签的定义也是标签拆分的一个依据另外结束符也是检查包的完整性的依据，标记不完整包将会直接丢弃

	*/
	int couter = offset;
	int imp;
	char *SearchPointer = p;
	char *LabelNamePosition;
	char *ContentPosition;
	char *EndLabelPosition;
	__PackTree *PackPointer;

	//这里需要一些用于定位的数据
	char buff[LabelBuffSize];//用于存放暂时数据
	char LabelBuff[LabelBuffSize] = { '\0' };
	char isFindLabel = 0;
	int i;
	while (couter > 0)
	{

		if (*SearchPointer == SpecialBorderSymbol || *SearchPointer == LabelStartSymbol)
		{
			if (*SearchPointer == SpecialBorderSymbol)
			{
				SearchPointer++;
				couter--;
				if (*SearchPointer == SpecialBorderStartSymbol)
				{
					while (*SearchPointer != SpecialBorderEndSymbol)
					{
						SearchPointer++;
						couter--;
					}
				}
			}
			if (*SearchPointer == LabelStartSymbol)
			{
				i = 0;
				while (*SearchPointer != LabelEndSymbol)
				{
					buff[i] = *SearchPointer;
					i++;
					SearchPointer++;
					couter--;
					if (i > LabelBuffSize)
					{
						break;
					}

				}
				buff[i] = *SearchPointer;
				i++;
				buff[i] = '\0';
				if (i < LabelBuffSize)
					isFindLabel = 1;
			}
		}


		if (isFindLabel)
		{
			/***这里将对获取的标签进行分析**/
			//printf("%s\n",buff);
			if (LabelBuff[0] == '\0')
			{


				/**此处分析标签的类型**/
				if (buff[i - 2] == '/'&&buff[1] != '/')//自结束标签 以及不是结束标签
				{
					LabelNamePosition = SearchPointer - i + 2;
					ContentPosition = LabelNamePosition;
					EndLabelPosition = SearchPointer;
					imp = SearchPointer - LabelNamePosition;

					/*****下面是是建立树的过程****/
					PackPointer = PackMalloc();

					PackPointer->Content = ContentPosition;
					PackPointer->LabelName = LabelNamePosition;
					PackPointer->Type = LabelType;
					PackPointer->Offset = imp;
					PackPointer->Right = NULL;
					PackPointer->Left = NULL;
					PackTreeAdd(PackPointer, LastPackTree);

					//PrintLabel("Self end Label:",LabelNamePosition);
					//printf("offset:%d,i:%d\n",imp,i);

					LabelBuff[0] = '\0';
				}
				else if (buff[1] != '/')//判别不是结束标签
				{

					LabelNamePosition = SearchPointer - i + 2;
					ContentPosition = SearchPointer + 1;//注意特殊情况
					//PrintLabel("Double Label Start:",LabelNamePosition);

					/*****下面是是建立树的过程****/
					PackPointer = PackMalloc();
					if (LastPackTree == NULL)
						LastPackTree = PackPointer;
					PackPointer->Content = ContentPosition;
					PackPointer->LabelName = LabelNamePosition;
					PackPointer->Type = LabelType;

					PackPointer->Right = NULL;
					PackPointer->Left = NULL;

					SplitString(' ', buff);
					StringTrim('<', StringOperation_Buff[0]);
					StringTrim('>', StringOperation_Buff[0]);
					if (!HasSymbol('/', StringOperation_Buff[0]))
					{
						StringCopy(StringOperation_Buff[0], LabelBuff);
						//printf("%s\n",LabelBuff);
					}
				}
			}
			else
			{
				//双标签结束标签
				if (buff[1] == '/')
				{
					//这里应该匹配到最末尾的标签 即贪婪匹配
					SplitString(' ', buff);
					StringTrim('<', StringOperation_Buff[0]);
					StringTrim('>', StringOperation_Buff[0]);
					StringTrim('/', StringOperation_Buff[0]);
					StringCopy(StringOperation_Buff[0], buff);

					//printf("I has find Label:%s\n",buff);
					if (StringEqual(buff, LabelBuff))
					{
						//printf("I has find Label:%s",LabelBuff);
						//PrintLabel(SearchPointer-i+2);
						EndLabelPosition = SearchPointer - i + 1;
						imp = EndLabelPosition - ContentPosition;
						PackPointer->Offset = imp;
						PackTreeAdd(PackPointer, LastPackTree);
						/***tips:递归调用再次分解出该标签所包含的内容中是否包含下一级标签**/
						PackAnalysis(ContentPosition, imp, PackPointer);

						//PrintLabel("Double Label End:",EndLabelPosition);
						//PrintLabel("Double Label Content:",ContentPosition);
						LabelBuff[0] = '\0';
					}
				}
			}



			isFindLabel = 0;
		}
		SearchPointer++;
		couter--;
	}





}
//需要底层实现
char *PackOpen(char *PackName);//将内容传送至Pack缓存中
char PackWrite(char *content);//将Pack缓存的内容写入到需要的地方
char PackClose(char *packname);//清除空闲的操作


//pack方法
char PackSetLabelContent(char *Label, char *content);
char *PackGetLabel(char *Label);
char PackAddNewLabel(char *Label, char *Content);
char PackDeleteLabel(char *Label);



/*int main()
{
	int i,CurentSize=0;
	char *p;
	__PackTree *HeadPack;
	//printf("%s\n",pack);
	PackOpen(NULL);
	printf("%s\n",Pack);
	PackAnalysis(Pack,4096,NULL);
	for(i=0;i<PackTreeUseTrack;i++)
	CurentSize+=CheckBit(PackTreeTrack[i]);
	for(i=0;i<CurentSize;i++)
	{
		HeadPack=&PackTree[i];
		printf("i=%d\n",i);
		PrintPackTree(HeadPack);
	}


}
*/
//打开完成的时候除了把段落加载到Pack还要将标记的树形结构提取出来

char *PackOpen(char *PackName)
{
	int i;
	FILE *fp;
	char *p;
	p = Pack;
	if ((fp = fopen("C:/Users/E.B. Zhou/Desktop/tools/pack/Pack", "r")) == NULL)
	{
		printf("Pack open ERROR\n");
		return NULL;
	}
	for (i = 0; i < PackInitSize - 1; i++)
	{
		fscanf(fp, "%c", &Pack[i]);
	}
	return p;
}
char PackWrite(char *content)
{
	//FileWrite(pack);


}


//<Label>寻找不带有<>  另外这个地方应该是从PackTree中直接对比提取出来的
char *PackGetLabel(char *Label)
{
	char *r, *search;

	return search;
}



#endif

