#include <stdio.h>
#include <unistd.h>

void print(int a, short b,char c, char *d, int *e)
{
	printf("%d, %d, %c, %s, %d\n",a ,b ,c ,d, *e);
}

int getAnswer(int f, char g)
{
	return 42;
}

void awesome(char *h, char *i)
{
	return;
}
int main()
{
	int i, var = 18;
	for(i = 0; i < 5; ++i)
	{
		print(1337, 86,'c', "Hello World",&var );
		sleep(1);
		getAnswer(18, 'a');
		awesome("Good Morning", "Good Night");
	}
	return 0;
}
