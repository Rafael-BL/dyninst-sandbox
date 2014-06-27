#include <stdio.h>
#include <unistd.h>
int getAnswer(int f, char g)
{
	return 42;
}
void print(int a, short b, char *c, char d, int *e)
{
	printf("%d, %d, %s, %c, %d\n",a ,b ,c ,d, *e);
}


int main()
{
	int i, var = 18;
	for(i = 0; i < 5; ++i)
	{
		print(1337, 86, "Hello World", 'c',&var );
		sleep(2);
		getAnswer(18, 'a');
	}
	return 0;
}
