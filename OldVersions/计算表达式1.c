#include <stdio.h>
#include <stdlib.h>

#define MaxStrSize 128

typedef struct DSNode
{
	int data;
	struct DSNode* next;
}DataLinkStack;//数据栈

typedef struct CSNode
{
	char ch;
	struct CSNode* next;
}CharLinkStack;//字符栈，支持+-*/

// 获取一行字符串
char* GetStr()
{
	char* s =(char*) malloc(sizeof(char));
	int i = 0;
	while(1)
	{
		s[i] = getchar();
		if(s[i] == '\n' || i >= MaxStrSize) break;//回车符结束
		if(s[i] == '\b')//退格符处理
		{
			if(i != 0) i--;
			continue;
		}
		i++;
	}
	return s;
}
// 入栈
DataLinkStack* PushData(DataLinkStack* dls, int data)
{
	DataLinkStack* newNode;
	if(dls == NULL)
	{
		dls =(DataLinkStack*) malloc(sizeof(DataLinkStack));
		dls->next = NULL;
	}
	dls->data = data;
	newNode =(DataLinkStack*) malloc(sizeof(DataLinkStack));
	newNode->data = 0;
	newNode->next = dls;
	return newNode;
}

CharLinkStack* PushChar(CharLinkStack* cls, char ch)
{
	CharLinkStack* newNode;
	if(cls == NULL)
	{
		cls =(CharLinkStack*) malloc(sizeof(CharLinkStack));
		cls->next = NULL;
	}
	cls->ch = ch;
	newNode =(CharLinkStack*) malloc(sizeof(CharLinkStack));
	newNode->next = cls;
	return newNode;
}
// 出栈
DataLinkStack* PopData(DataLinkStack* dls)
{
	DataLinkStack* p = dls;
	if(dls == NULL)
	{
		printf("数据栈空！无法出栈！\n");
	}
	else
	{
		dls = dls->next;
		free(p);
	}
	return dls;
}

CharLinkStack* PopChar(CharLinkStack* cls)
{
	CharLinkStack* p = cls;
	if(cls == NULL)
	{
		printf("字符栈空！无法出栈！\n");
	}
	else
	{
		cls = cls->next;
		free(p);
	}
	return cls;
}
// 获取栈顶元素直接dls->data
// 根据字符串生成两栈
DataLinkStack* GetDataStack(char* s)
{
	DataLinkStack* dls =(DataLinkStack*) malloc(sizeof(DataLinkStack));//栈顶指针
	DataLinkStack* newNode;
	int numberEnd = 0;
	int i;
	dls->data = 0; 
	dls->next = NULL;
	for(i=0; s[i]!='\n'; i++)
	{
		if(s[i]>='0' && s[i]<='9')
		{
			dls->data *= 10;
			dls->data += s[i]-'0'+0;
			numberEnd = 0;
		}
		else if(numberEnd == 0)
		{
			numberEnd = 1;
			newNode =(DataLinkStack*) malloc(sizeof(DataLinkStack));
			newNode->data = 0;
			newNode->next = dls;
			dls = newNode;
		}
	}
	if(numberEnd == 0)
	{
		numberEnd = 1;
		newNode =(DataLinkStack*) malloc(sizeof(DataLinkStack));
		newNode->data = 0;
		newNode->next = dls;
		dls = newNode;
	}
	return dls;
}

CharLinkStack* GetCharStack(char* s)
{
	CharLinkStack* cls =(CharLinkStack*) malloc(sizeof(CharLinkStack));
	CharLinkStack* newNode;
	int i;
	cls->next = NULL;
	for(i=0; s[i]!='\n'; i++)
	{
		if(s[i]>='0' && s[i]<='9')
		{
			//跳过数字
		}
		else
		{
			cls->ch = s[i];
			newNode =(CharLinkStack*) malloc(sizeof(CharLinkStack));
			newNode->next = cls;
			cls = newNode;
		}
	}
	return cls;
}

int main()
{
	char* s;
	DataLinkStack* dls;
	CharLinkStack* cls;
	int r, dtemp;
	char ctemp;
	while(1)
	{
		// 获取
		s = GetStr();//5+4*2\n
		dls = GetDataStack(s);//2 4 5
		cls = GetCharStack(s);//\r * +
		// 计算
		//栈顶不存数据，dls->data必为0，cls->ch必为回车
		while(dls->next->next != NULL && cls->next != NULL)//仅剩1数据或无运算字符时退出
		{
			if(cls->next->ch == '*' || cls->next->ch == '/')//表达式末尾为*/
			{
				if(cls->next->ch == '*')
				{
					r = dls->next->data * dls->next->next->data;
				}
				if(cls->next->ch == '/')
				{
					r = dls->next->next->data / dls->next->data;
				}
				//出栈
				dls = PopData(dls);
				dls = PopData(dls);
				cls = PopChar(cls);
				//结果入栈
				dls = PushData(dls, r);
			}
			else if(cls->next->ch == '+' || cls->next->ch == '-')//表达式末尾为+-
			{
				if(cls->next->next != NULL && (cls->next->next->ch == '*' || cls->next->next->ch == '/'))
				{
					if(cls->next->next->ch == '*')
					{
						r = dls->next->next->data * dls->next->next->next->data;
					}
					if(cls->next->next->ch == '/')
					{
						r = dls->next->next->next->data / dls->next->next->data;
					}
					//暂存
					dtemp = dls->next->data;
					ctemp = cls->next->ch;
					//出栈
					dls = PopData(dls);
					dls = PopData(dls);
					dls = PopData(dls);
					cls = PopChar(cls);
					cls = PopChar(cls);
					//入栈
					dls = PushData(dls, r);//*/结果
					cls = PushChar(cls, ctemp);//+-符号
					dls = PushData(dls, dtemp);//末尾数字
				}
				else//+-前不为*/
				{
					if(cls->next->ch == '+')
					{
						r = dls->next->data + dls->next->next->data;
					}
					if(cls->next->ch == '-')
					{
						r = dls->next->next->data - dls->next->data;
					}
					//出栈
					dls = PopData(dls);
					dls = PopData(dls);
					cls = PopChar(cls);
					//结果入栈
					dls = PushData(dls, r);
				}
			}
		}
		// 输出
		printf("=%d\n", dls->next->data);
		free(dls->next);
		free(dls);
		free(cls);
	}
	return 0;
}
