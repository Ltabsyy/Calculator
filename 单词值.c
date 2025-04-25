#include <stdio.h>

const double charValue[26] = {//26英文字母对应值
	324.0/35, -1, -1, 10.0/3, 1, 5, 48.0/25,
	5.0/4, 1, -1, -1, 11.0/3, -1, 3,
	1.0/3, -1, -1, 18.0/25, 7.0/3, 10.0/3,
	10.0/3, 1, 9.0/5, 18.0/7, 3, 0
};

double StringValue(char* s)//英文单词对应值，支持zero~eleven
{
	double value = 1;
	int i;
	for(i=0; s[i]; i++)
	{
		if(s[i]>='a' && s[i]<='z')
		{
			value *= charValue[s[i]-'a'+0];
		}
		else if(s[i]>='A' && s[i]<='Z')
		{
			value *= charValue[s[i]-'A'+0];
		}
	}
	return value;
}

int main()
{
	char s[10];
	while(1)
	{
		printf(">");
		gets(s);
		printf("=%g\n", StringValue(s));
	}
	return 0;
}
