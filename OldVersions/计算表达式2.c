/*采用链栈简单实现简单的无括号加减乘除表达式运算*/
#include <stdio.h>
#include <stdlib.h>

typedef struct SNode
{
	int data;//数据栈和字符栈采用同一种结构存储
	struct SNode* next;
}LinkStack;
// 入栈
LinkStack* Push(LinkStack* S, int data)
{
	LinkStack* newNode =(LinkStack*) malloc(sizeof(LinkStack));
	if(S == NULL)
	{
		S =(LinkStack*) malloc(sizeof(LinkStack));
		S->next = NULL;
	}
	S->data = data;
	newNode->data = 0;//栈顶不存数据
	newNode->next = S;
	return newNode;
}
// 出栈
LinkStack* Pop(LinkStack* S)
{
	LinkStack* p = S;
	if(S == NULL)
	{
		printf("栈空！无法出栈！\n");
	}
	else
	{
		S = S->next;
		free(p);
	}
	return S;
}
// 获取一行字符串
char* GetStr()
{
	char* s =(char*) malloc(sizeof(char));
	int i = 0;
	while(1)
	{
		s[i] = getchar();
		if(s[i] == '\n') break;//回车符结束
		if(s[i] == '\b')//退格符处理
		{
			if(i != 0) i--;
			continue;
		}
		i++;
	}
	return s;
}
// 根据字符串生成数据栈
LinkStack* GetDataStack(char* s)
{
	LinkStack* S =(LinkStack*) malloc(sizeof(LinkStack));
	LinkStack* newNode;
	int numberEnd = 1;
	int i;
	S->data = 0;
	S->next = NULL;
	for(i=0; s[i]!='\n'; i++)
	{
		if(s[i]>='0' && s[i]<='9')
		{
			S->data *= 10;
			S->data += s[i]-'0'+0;
			numberEnd = 0;
		}
		else if(numberEnd == 0)
		{
			numberEnd = 1;
			newNode =(LinkStack*) malloc(sizeof(LinkStack));
			newNode->data = 0;
			newNode->next = S;
			S = newNode;
		}
	}
	if(numberEnd == 0)
	{
		numberEnd = 1;
		newNode =(LinkStack*) malloc(sizeof(LinkStack));
		newNode->data = 0;
		newNode->next = S;
		S = newNode;
	}
	return S;
}
// 根据字符串生成字符栈
LinkStack* GetCharStack(char* s)
{
	LinkStack* S =(LinkStack*) malloc(sizeof(LinkStack));
	LinkStack* newNode;
	int i;
	S->next = NULL;
	for(i=0; s[i]!='\n'; i++)
	{
		if(s[i]>='0' && s[i]<='9')
		{
			//跳过数字
		}
		else
		{
			S->data = s[i];//用int存储字符(doge)
			newNode =(LinkStack*) malloc(sizeof(LinkStack));
			newNode->next = S;
			S = newNode;
		}
	}
	return S;
}
// 两数简单计算
int Calculate(int a, char operation, int b)
{
	int result;
	if(operation == '+')
	{
		result = a+b;
	}
	else if(operation == '-')
	{
		result = a-b;
	}
	else if(operation == '*')
	{
		result = a*b;
	}
	else if(operation == '/')
	{
		if(b == 0)
		{
			printf("Division by zero!\n");
			return 0;
		}
		result = a/b;
	}
	else if(operation == '%')
	{
		if(b == 0)
		{
			printf("Division by zero!\n");
			return 0;
		}
		result = a%b;
	}
	return result;
}
// 无括号+-*/%表达式计算
int EasyCalculator(char* s)
{
	LinkStack* dls;
	LinkStack* cls;
	int r, dtemp;
	char ctemp;
	// 获取
	dls = GetDataStack(s);//2 4 5
	cls = GetCharStack(s);//\r * +
	// 计算
	//栈顶不存数据，dls->data必为0，cls->ch必为回车
	while(dls->next->next != NULL && cls->next != NULL)//仅剩1数据或无运算字符时退出
	{
		if(cls->next->data == '*' || cls->next->data == '/' || cls->next->data == '%')//表达式末尾为*/%
		{
			r = Calculate(dls->next->next->data, cls->next->data, dls->next->data);
			//出栈
			dls = Pop(dls);
			dls = Pop(dls);
			cls = Pop(cls);
			//结果入栈
			dls = Push(dls, r);
		}
		else if(cls->next->data == '+' || cls->next->data == '-')//表达式末尾为+-
		{
			if(cls->next->next != NULL && (cls->next->next->data == '*' || cls->next->next->data == '/' || cls->next->next->data == '%'))
			{
				// +-前为*/%先计算前面
				r = Calculate(dls->next->next->next->data, cls->next->next->data, dls->next->next->data);
				//暂存
				dtemp = dls->next->data;
				ctemp = cls->next->data;
				//出栈
				dls = Pop(dls);
				dls = Pop(dls);
				dls = Pop(dls);
				cls = Pop(cls);
				cls = Pop(cls);
				//入栈
				dls = Push(dls, r);//*/结果
				cls = Push(cls, ctemp);//+-符号
				dls = Push(dls, dtemp);//末尾数字
			}
			else//+-前不为*/%
			{
				r = Calculate(dls->next->next->data, cls->next->data, dls->next->data);
				//出栈
				dls = Pop(dls);
				dls = Pop(dls);
				cls = Pop(cls);
				//结果入栈
				dls = Push(dls, r);
			}
		}
	}
	// 输出
	r = dls->next->data;
	free(dls->next);
	free(dls);
	free(cls);
	return r;
}

int main()
{
	char* s;
	while(1)
	{
		s = GetStr();//5+4*2\n
		printf("=%d\n", EasyCalculator(s));
	}
	return 0;
}
