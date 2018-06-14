#include <stdio.h>

struct func_list
{
	void (*A_printf)();
	void (*B_printf)();
	void (*C_printf)();
};

struct func_list *a_list;

void C_printf()
{
	printf("Hello C\r\n");
}

void C_register(struct func_list *a)
{
	a_list = a;
	a->C_printf = C_printf;//把函数放到公共列表中
	printf("register C\r\n");
}

void C_release(struct func_list *a)
{
	a_list = a;//把库里的函数列表指向公共列表
	a->C_printf = NULL;//把函数放到公共列表中
	printf("release C\r\n");
}