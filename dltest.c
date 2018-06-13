#include <stdio.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct func_list
{
	void (*A_printf)();
	void (*B_printf)();
	void (*C_printf)();
	
};

//注册库文件函数
void load_func(void **alib,char *so_buf,char *get_buf,char *change_buf,struct func_list *list)
{
	void (*my_test)();
	change_buf = strtok(so_buf,",");	
	sprintf(get_buf,"./%s",change_buf);//获取第一个库文件的名称
	
	 while(change_buf != NULL)
	{
		if(strcmp(change_buf,"a.so") == 0)
		{
			*alib = dlopen(get_buf,RTLD_NOW|RTLD_GLOBAL);//动态打开库文件
			if(*alib == NULL)
			{
				perror("open so error:");
				exit(1);
			}

			my_test = dlsym(*alib,"A_register");//注册库文件
			if(my_test == NULL)
			{
				perror("register func A error:");
				exit(1);
			}
			
			my_test(list);//调用注册函数注册
			
		}
		else if(strcmp(change_buf,"b.so") == 0)
		{
			*alib = dlopen(get_buf,RTLD_NOW|RTLD_GLOBAL);//动态打开库文件
			if(*alib == NULL)
			{
				perror("open so error:");
				exit(1);
			}

			my_test = dlsym(*alib,"B_register");//注册库文件
			if(my_test == NULL)
			{
				perror("register func B error:");
				exit(1);
			}
			my_test(list);//调用注册函数注册
			
		}
		else if(strcmp(change_buf,"c.so") == 0)
		{
			*alib = dlopen(get_buf,RTLD_NOW|RTLD_GLOBAL);//动态打开库文件
			if(*alib == NULL)
			{
				perror("open so error:");
				exit(1);
			}
			
			my_test = dlsym(*alib,"C_register");//注册库文件
			if(my_test == NULL)
			{
				perror("register func C error:");
				exit(1);
			}
			my_test(list);//调用注册函数注册
		}
		else 
		{
			printf("The system have no %s lib \r\n",change_buf);//没有加载到库文件的提示
		}
	change_buf = strtok(NULL,",");	
	sprintf(get_buf,"./%s",change_buf);//获取下一个库文件的名称
	}
}

int main(void)
{	
	void *alib = NULL;
	int fd = 0, ret = 0;
	char so_buf[1024] = {0},get_buf[34] = {0};
	char *change_buf = NULL;
	struct func_list list;
	
	
	fd = open("./so_config.txt",O_RDONLY|O_CREAT,0777);//打开配置文件
	if(fd == 0)
	{
		perror("open so.txt error:");
		exit(1);
	}
	
	ret = read(fd,so_buf,sizeof(so_buf));//读取配置文件的信息
	
	load_func(&alib,so_buf,get_buf,change_buf,&list);
	
	list.A_printf();//调用函数A
	
	list.B_printf();//调用函数B
	list.C_printf();//调用函数C
	
	dlclose(alib);//动态关闭库文件
	close(fd);
	
	return 0;
}
