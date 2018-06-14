#include <stdio.h>

struct func_list
{
	void (*A_printf)();
	void (*B_printf)();
	void (*C_printf)();
};

struct func_list *a_list;

void B_printf()
{
	printf("Hello B\r\n");
}

void B_register(struct func_list *a)
{
	a_list = a;
	a->B_printf = B_printf;//把函数放到公共列表中
	printf("register B\r\n");
}
void B_release(struct func_list *a)
{
	a_list = a;//把库里的函数列表指向公共列表
	a->B_printf = NULL;//把函数放到公共列表中
	printf("release B\r\n");
}