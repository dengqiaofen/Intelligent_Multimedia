#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "lcd.h"
#include "bmp.h"
#include "save_all_same.h"
#include "slip_or_click.h"
#include "Sudoku.h"
#include "modify_password.h"
int main(void)
{
	lcd_init();
	//锁屏壁纸
	
	//输入密码，直到正确就进入主界面
	int pass_fd = open("./password/password.txt",O_RDWR);
	char password_4[4];
	int i = 0; 
	int j = 4;
	while(j--)
	{
		read(pass_fd,&password_4[i++],1);
	}
	//int num[4] = {0,5,2,9};
	
	struct coordinates dd;
	int click,c_eara;
	int ret = 0;
	while(1)
	{
		if(ret == 0)
		{
			//解锁
			while(1)
			{
				display_bmp("./bj_and_tb/suoping1.bmp",0,0);
				//判断密码是否正确
				int flag = password(password_4);
				printf("flag = %d\n",flag);
				if(flag==1)
				{
					ret = 1;
					break;
				}
			}
		}
		
		//显示主背景
		display_bmp("./bj_and_tb/mainbj.bmp",0,0);
		//获取点击区域的坐标，判断去执行什么功能
		Get_coordinates(&dd);
		//判断是不是点击
		click = Sliding_direction(&dd);
		//如果不是点击，则重新点击再做后续操作
		if(click != 0)
		{
			continue;
		}
		//判断点击区域在哪
		c_eara = Click_area(&dd);
		switch (c_eara)
			{
				case Electronic_photo_album:
					print_all_pic("./images/");
					break;
				case Music_player:
					Play_music("./music/");
					break;
				case Video_player:
					Play_video("./video/");
					break;
				case sudoku_Game:
					sudoku();
					break;
				case snake_Game:
					snake();
					break;
				case Modify_password:
					modify_password(pass_fd,password_4);
					break;
				case lock_screen:
					ret = 0;
					break;
				case Invalid_click:
					break;
				
			}
	}
	
	close(pass_fd);
	
	lcd_close();
	return 0;
}
