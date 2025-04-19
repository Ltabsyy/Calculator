/*采用数组栈简单实现带括号加减乘除表达式运算*/
#include <stdio.h>

// 获取一行字符串
void GetStr(char s[])
{
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
int Calculator(char* s)
{
	int ds[32];
	char cs[32];
	int i, n, dtop = 0, ctop = 0;
	for(i=0; s[i]!='\n'; i++)
	{
		if(s[i]>='0' && s[i]<='9')
		{
			//读取一个数字
			for(n=0; s[i]>='0' && s[i]<='9'; i++)
			{
				n *= 10;
				n += s[i]-'0'+0;
			}
			i--;
			ds[dtop] = n;
			dtop++;
		}
		if(s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '%'
			|| s[i] == '(')
		{
			if(s[i] == '-')
			{
				if(i == 0 || s[i-1] == '(')
				{
					ds[dtop] = 0;
					dtop++;
				}
			}
			if(s[i] == '(')
			{
				if(i > 0 && s[i-1]>='0' && s[i-1]<='9')
				{
					cs[ctop] = '*';
					ctop++;
				}
			}
			if(ctop > 0 && cs[ctop-1] != '(' && dtop > 1
				&& OperatorPrecedence(s[i]) > OperatorPrecedence(cs[ctop-1]))
			{
				if(ctop > 1 && cs[ctop-2] == '-')
				{
					ds[dtop-2] *= -1;
					cs[ctop-2] = '+';
				}
				n = Calculate(ds[dtop-2], cs[ctop-1], ds[dtop-1]);
				dtop -= 2;
				ctop--;
				ds[dtop] = n;
				dtop++;
			}
			cs[ctop] = s[i];
			ctop++;
		}
		if(s[i] == ')')
		{
			while(ctop > 0 && cs[ctop-1] != '(')
			{
				if(ctop > 1 && cs[ctop-2] == '-')
				{
					ds[dtop-2] *= -1;
					cs[ctop-2] = '+';
				}
				n = Calculate(ds[dtop-2], cs[ctop-1], ds[dtop-1]);
				dtop -= 2;
				ctop--;
				ds[dtop] = n;
				dtop++;
			}
			if(ctop > 0 && cs[ctop-1] == '(') ctop--;
		}
	}
	while(ctop > 0 && dtop > 1)
	{
		if(ctop > 1 && cs[ctop-2] == '-')
		{
			ds[dtop-2] *= -1;
			cs[ctop-2] = '+';
		}
		n = Calculate(ds[dtop-2], cs[ctop-1], ds[dtop-1]);
		dtop -= 2;
		ctop--;
		ds[dtop] = n;
		dtop++;
	}
	return ds[dtop-1];
}

int main()
{
	char s[64];
	while(1)
	{
		printf("[Calculator]>");
		GetStr(s);//5+4*2\n
		printf("=%d\n", Calculator(s));
	}
	return 0;
}
