#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "slip_or_click.h"
#include "bmp.h"


//修改密码
void modify_password(int fd,char *password)
{
	
	struct coordinates dd;
	int click;
	//两次输入一样就设置新密码，否则不更改
	char num1[4] = {0};
	char num2[4] = {0};
	int flag = 0;//密码输入四位
	int count = 0; //输入两次密码，相同则修改
	while(1)
	{
		display_bmp("./password/password.bmp", 0, 0);
		int flag = 0;//输入四次
		while(1)
		{
			Get_coordinates(&dd);
			//判断是不是点击
			click = Sliding_direction(&dd);
			if(click == 0)
			{
				if(dd.x0 >= 0 && dd.x0 <= 100 && dd.y0 >= 0 && dd.y0 <= 100)//点击返回就返回
				{
					return;
				}
				else if(dd.x0 >= 226 && dd.x0 <= 283 && dd.y0 >= 143 && dd.y0 <= 173)//数字1
				{
					display_bmp("./password/1.bmp",220+flag*80,50);
					num1[flag++] = '1';
				}
				else if(dd.x0 >= 354 && dd.x0 <= 413 &&dd.y0 >= 143 && dd.y0 <= 173)//数字2
				{
					display_bmp("./password/2.bmp",220+flag*80,50);
					num1[flag++] = '2';
					
				}
				else if(dd.x0 >= 439 && dd.x0 <= 546 && dd.y0 >= 143 && dd.y0 <= 173)//数字3
				{
					display_bmp("./password/3.bmp",220+flag*80,50);
					num1[flag++] = '3';
					
				}
				else if(dd.x0 >= 226 && dd.x0 <= 283 && dd.y0 >= 240 && dd.y0 <= 278)//数字4
				{
					display_bmp("./password/4.bmp",220+flag*80,50);
					num1[flag++] = '4';
					
				}
				else if(dd.x0 >= 354 && dd.x0 <= 413 && dd.y0 >= 240 && dd.y0 <= 278)//数字5
				{
					display_bmp("./password/5.bmp",220+flag*80,50);
					num1[flag++] = '5';
					
				}
				else if(dd.x0 >= 439 && dd.x0 <= 546 && dd.y0 >= 240 && dd.y0 <= 278)//数字6
				{
					display_bmp("./password/6.bmp",220+flag*80,50);
					num1[flag++] = '6';
					
				}
				else if(dd.x0 >= 226 && dd.x0 <= 283&& dd.y0 >= 336 && dd.y0 <= 375)//数字7
				{
					display_bmp("./password/7.bmp",220+flag*80,50);
					num1[flag++] = '7';
					
				}
				else if(dd.x0 >= 354 && dd.x0 <= 413 && dd.y0 >= 336 && dd.y0 <= 375)//数字8
				{
					display_bmp("./password/8.bmp",220+flag*80,50);
					num1[flag++] = '8';
					
				}
				else if(dd.x0 >= 439 && dd.x0 <= 546 && dd.y0 >= 336 && dd.y0 <= 375)//数字9
				{
					display_bmp("./password/9.bmp",220+flag*80,50);
					num1[flag++] = '9';
					
				}
				else if(dd.x0 >= 354 && dd.x0 <= 413 && dd.y0 >= 432 && dd.y0 <= 473)//数字0
				{
					display_bmp("./password/0.bmp",220+flag*80,50);
					num1[flag++] = '0';
					
				}
			}
			if(flag == 4)
			{
				break;
			}
		}
		count++;
		int ret = 0;
		if(count == 2)
		{
			for(int k = 0; k < 4; k++)//比较两次输入
			{
				if(num1[k] != num2[k])
				{
					ret = 1;
				}
			}
			if(ret == 1)//两次密码不一样
			{
				count = 0;
				display_bmp("./password/error.bmp",150,0);
				sleep(1);
				continue;
			}
			else//两次输入相同
			{
				lseek(fd,0,SEEK_SET);//先将光标移动到文件开头，方便写入新的密码
				for(int k = 0; k < 4; k++)
				{
					password[k] = num1[k];
					write(fd,&num1[k],1);
				}
				display_bmp("./password/success.bmp", 600, 180);//显示修改成功的图片
				while(1)
				{
					Get_coordinates(&dd);
					//判断是不是点击
					click = Sliding_direction(&dd);
					if(click == 0)
					{
						if(dd.x0 >= 0 && dd.x0 <= 100 && dd.y0 >= 0 && dd.y0 <= 100)//点击返回就返回
						{
							return;
						}
					}
				}
			}
		}
		display_bmp("./password/again.bmp",150,0);
		sleep(1);
		for(int k = 0; k < 4; k++)
		{
			num2[k] = num1[k];
		}
	}
	
	
}

