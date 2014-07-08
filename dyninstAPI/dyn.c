#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "argtype.h"
static int a = 0;
void tp(void)
{
	printf("Hello: %d\n",a);
	a++;
}

void tpvarargs(int arg1, ...)
{
	int i;
	va_list ap;
	printf("We received %d parameters\n", arg1);
	va_start(ap, arg1);
	int type;
	char *name;
	name = malloc(512);
	for(i = 0; i < arg1; ++i)
	{
		type = va_arg (ap,int); // get next argument type
		name = va_arg (ap,char *); // get next argument name
		printf("Arg%d named %s of type ",i,name);
		switch(type)
		{
			case INT:
			{
				int a = va_arg (ap,int); // get next argument
				printf("int and value: %d\n", a);
				break;
			}
			case CHAR:
			{
				char a =(char) va_arg (ap,int); // get next argument
				printf("char and value: %c\n", a);
				break;
			}
			case SHORT_INT:
			{
				short a =(short) va_arg (ap,int); // get next argument
				printf("short int and value: %d\n", a);
				break;
			}
			case INT_PTR:
			{
				int *a = va_arg (ap,int *); // get next argument
				printf("int* and value: %d\n", *a);
				break;
			}
			case CHAR_PTR:
			{
				char *a = va_arg (ap,char *); // get next argument
				printf("char* and value: %s\n", a);
				break;
			}
			default:
			{
				type = va_arg (ap,int); // get next argument
				printf("unknowed.\n");
				break;
			}
		}
	}
	va_end(ap);
	free(name);
}

void tp_print_param_int(char *name ,int value)
{
	printf("\tint %s = %d;\n", name, value);
}

void tp_print_param_char(char *name ,char value)
{
	printf("\tchar %s = \'%c\';\n", name, value);
}

void tp_print_param_char_ptr(char *name ,char *value)
{
	printf("\tchar *%s = \"%s\";\n", name, value);
}

void tp_print_param_void_ptr(char *name ,void *value)
{
	printf("\tvoid *%s = %p;\n", name, value);
}
