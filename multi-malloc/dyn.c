#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "argtype.h"
void tp1(int *a)
{
	printf("in TP1 ptr=%p, value=%d\n",a, *a);
}
void tp2(int *a)
{
	printf("in TP2 ptr=%p, value=%d\n",a, *a);
}

