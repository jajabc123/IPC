#include <stdio.h>


struct func_list
{
	void (*A_printf)();
	void (*B_printf)();
	void (*C_printf)();
};

struct func_list *a_list;

void A_printf()
{
	printf("Hello A\r\n");
	
	a_list->B_printf();
	a_list->C_printf();
}

void A_register(struct func_list *a)
{
	a_list = a;//把库里的函数列表指向公共列表
	a->A_printf = A_printf;//把函数放到公共列表中
	printf("register A\r\n");
}

void A_release(struct func_list *a)
{
	a_list = a;//把库里的函数列表指向公共列表
	a->A_printf = NULL;//把函数放到公共列表中
	printf("release A\r\n");
}