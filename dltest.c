#include <stdio.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LOG_IN  1
#define LOG_OUT 2
#define A_PRINTF() list.A_printf()
#define B_PRINTF() list.B_printf()
#define C_PRINTF() list.C_printf()

//函数公共列表
struct func_list
{
	void (*A_printf)();
	void (*B_printf)();
	void (*C_printf)();
};

//注册库文件函数0
void func_operation(void **alib,char *sc_buf,struct func_list *list,int operation)
{
	void (*my_handle)();
	char name[32];
	char func_name[34];
	char so_buf[1024] = {0},get_buf[34] = {0};
	char *change_buf = NULL;
	
	strcpy(so_buf,sc_buf);                             //把配置文件的信息复制到so_buf中去,以免strtok执行后sc_buf被改变
	
	change_buf = strtok(so_buf,",");	
	sprintf(get_buf,"./%s",change_buf);                //获取第一个库文件的名称
	
	 while(change_buf != NULL)
	{
		*alib = dlopen(get_buf,RTLD_NOW|RTLD_GLOBAL);  //动态打开库文件
		if(*alib == NULL)
		{
			perror("open so error:");
			exit(1);
		}
		
		sscanf(change_buf,"%[^.so]",name);            //获取库函数名字

		if(operation == LOG_IN)
		{
			sprintf(func_name,"%s_register",name);    //合成对应的注册库函数名字
		}
		else if(operation == LOG_OUT)
		{
			sprintf(func_name,"%s_release",name);     //合成对应的注销库函数名字
		}
		else
		{
			printf("The operation is illegal!\r\n");  //错误操作警报
			exit(1);
		}
		
		my_handle = dlsym(*alib,func_name);           //动态加载对应的库文件函数
		if(my_handle == NULL)
		{
			perror("handle func error:");
			exit(1);
		}
		my_handle(list);                              //调用相应的操作函数
		
	change_buf = strtok(NULL,",");	
	sprintf(get_buf,"./%s",change_buf);               //获取下一个库文件的名称
	}
}


int main(void)
{	
	void *alib = NULL;
	int fd = 0, ret = 0;
	char sc_buf[1024] = {0};
	
	struct func_list list;
	
	fd = open("./so_config.txt",O_RDONLY|O_CREAT,0777);              //打开配置文件
	if(fd == 0)
	{
		perror("open so.txt error:");
		exit(1);
	}
	
	ret = read(fd,sc_buf,sizeof(sc_buf));                            //读取配置文件的信息
	
	func_operation(&alib,sc_buf,&list,LOG_IN);    					 //注册各个库文件里面的函数
	
	A_PRINTF();	                                                     //调用函数A
	B_PRINTF();	                                                     //调用函数B
	C_PRINTF();	                                                     //调用函数C
	
	func_operation(&alib,sc_buf,&list,LOG_OUT);	                     //注销各个库文件里面的函数
	
	dlclose(alib);                                                   //动态关闭库文件
	close(fd);
	
	return 0;
}