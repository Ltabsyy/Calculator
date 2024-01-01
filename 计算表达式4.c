/*采用链栈简单实现带括号加减乘除表达式运算*/
#include <stdio.h>
#include <stdlib.h>

typedef struct SNode
{
	int data;//数据栈和字符栈采用同一种结构存储
	struct SNode* next;
}LinkStack;

// 入栈
void Push(LinkStack** S, int data)
{
	if(*S == NULL)
	{
		*S =(LinkStack*) malloc(sizeof(LinkStack));
		(*S)->data = data;
		(*S)->next = NULL;		
	}
	else
	{
		LinkStack* newNode =(LinkStack*) malloc(sizeof(LinkStack));
		newNode->data = data;//栈顶存数据
		newNode->next = *S;
		*S = newNode;
	}
}

// 出栈
int Pop(LinkStack** S)
{
	if(*S == NULL)
	{
		printf("栈空！无法出栈！\n");
		return 0;
	}
	else
	{
		LinkStack* p = *S;
		int temp = (*S)->data;
		*S = (*S)->next;
		free(p);
		return temp;
	}
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

// 对栈顶进行简单计算
void CalculateTop(LinkStack** dls, LinkStack** cls)
{
	int a, b, c;
	char operation = Pop(cls);
	b = Pop(dls);
	a = Pop(dls);
	if(*cls != NULL && (*cls)->data == '-')
	{
		Pop(cls);
		a *= -1;
		Push(cls, '+');
	}
	c = Calculate(a, operation, b);
	Push(dls, c);
}

// 无括号+-*/%表达式计算
int EasyCalculator(char* s)
{
	LinkStack* dls = NULL;
	LinkStack* cls = NULL;
	int i, r, dtemp, numberEnd = 1;
	char ctemp;
	// 获取
	// 根据字符串生成数据栈2 4 5，字符栈\r * +
	Push(&dls, 0);
	for(i=0; s[i]!='\n'; i++)
	{
		if(s[i]>='0' && s[i]<='9')
		{
			dls->data *= 10;
			dls->data += s[i]-'0'+0;
			numberEnd = 0;
		}
		else
		{
			if(numberEnd == 0)
			{
				numberEnd = 1;
				Push(&dls, 0);
			}
			Push(&cls, s[i]);//用int存储字符(doge)
		}
	}
	if(numberEnd == 1) Pop(&dls);//栈顶存数据
	// 计算
	while(dls->next != NULL && cls != NULL)//仅剩1数据或无运算字符时退出
	{
		if(cls->data == '*' || cls->data == '/' || cls->data == '%')//表达式末尾为*/%
		{
			CalculateTop(&dls, &cls);
		}
		else if(cls->data == '+' || cls->data == '-')//表达式末尾为+-
		{
			if(cls->next != NULL && (cls->next->data == '*' || cls->next->data == '/' || cls->next->data == '%'))
			{
				// +-前为*/%先计算前面
				//暂存栈顶
				dtemp = Pop(&dls);
				ctemp = Pop(&cls);
				//计算
				CalculateTop(&dls, &cls);//*/结果
				//入栈
				Push(&cls, ctemp);//+-符号
				Push(&dls, dtemp);//末尾数字
			}
			else//+-前不为*/%
			{
				CalculateTop(&dls, &cls);
			}
		}
	}
	// 输出
	r = dls->data;
	free(dls);
	free(cls);
	return r;
}

// 根据字符串位置获取数字
int Number(char* s)
{
	int i;
	int number = 0;
	for(i=0; s[i]>='0' && s[i]<='9'; i++)//只读取一个数字
	{
		number *= 10;
		number += s[i]-'0'+0;
	}
	return number;
}

// 运算符优先级
int OperatorPrecedence(char operation)
{
	if(operation == '(' || operation == ')')
	{
		return 1;
	}
	else if(operation == '*' || operation == '/' || operation == '%')
	{
		return 3;
	}
	else if(operation == '+' || operation == '-')
	{
		return 4;
	}
	else
	{
		return 15;
	}
}

// 含括号+-*/%表达式计算
int IntCalculator(char* s)
{
	LinkStack* dls = NULL;
	LinkStack* cls = NULL;
	int i, number;
	for(i=0; s[i]!='\n'; i++)
	{
		if(s[i]>='0' && s[i]<='9')
		{
			//读取一个数字
			number = 0;
			for(; s[i]>='0' && s[i]<='9'; i++)
			{
				number *= 10;
				number += s[i]-'0'+0;
			}
			i--;
			Push(&dls, number);
		}
		if(s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '%'
			|| s[i] == '(')
		{
			if(s[i] == '-')
			{
				if(i == 0 || s[i-1] == '(') Push(&dls, 0);
			}
			if(s[i] == '(')
			{
				if(i > 0 && s[i-1]>='0' && s[i-1]<='9') Push(&cls, '*');
			}
			if(cls != NULL && cls->data != '(' && dls != NULL && dls->next != NULL
				&& OperatorPrecedence(s[i]) > OperatorPrecedence(cls->data))
			{
				CalculateTop(&dls, &cls);//5*4+2
			}
			Push(&cls, s[i]);
		}
		if(s[i] == ')')
		{
			while(cls != NULL && cls->data != '(')
			{
				CalculateTop(&dls, &cls);
			}
			if(cls != NULL && cls->data == '(')
			{
				Pop(&cls);
			}
		}
	}
	while(cls != NULL && dls->next != NULL)
	{
		CalculateTop(&dls, &cls);
	}
	return dls->data;
}

float FloatCalculator(char* s)
{
	return (float)IntCalculator(s);
}

int main()
{
	char* s;
	int i, selectCalculator;
	while(1)
	{
		printf("[Calculator]>");
		s = GetStr();//5+4*2\n
		//printf("=%d\n", IntCalculator(s));
		selectCalculator = 1;
		for(i=0; s[i]!='\n'; i++)
		{
			if(selectCalculator == 0 && (s[i] == '(' || s[i] == ')'))
			{
				selectCalculator = 1;
			}
			if(s[i] == '/' || s[i] == '.')
			{
				selectCalculator = 2;
			}
		}
		if(selectCalculator == 0)
		{
			//无括号采用简单计算
			printf("=%d\n", EasyCalculator(s));
		}
		else if(selectCalculator == 1)
		{
			//有括号采用重读计算
			printf("=%d\n", IntCalculator(s));
		}
		else
		{
			printf("=%f\n", FloatCalculator(s));
		}
	}
	return 0;
}
