#include<sys/mman.h>
#include<unistd.h>
#include<sys/ioctl.h>   
#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<linux/input.h>
#include<stdlib.h>
#include <string.h>
#include"slip_or_click.h"
#include "bmp.h"


//获取坐标
void Get_coordinates(struct coordinates *p)
{
	int flag_x=0,flag_y=0;//标志着有没有采集到数据
	struct input_event et;
	int fd = open("/dev/input/event0", O_RDONLY);
	if(fd == -1)
	{
		perror("open event0 failed");
		return;
	}
	while(1)
	{
		int r = read(fd,&et,sizeof(et));
		if(r == sizeof(et))
		{
			printf("type:%d  code:%d  value:%ld\n",et.type,et.code,et.value);
			if(et.type==EV_ABS && et.code==ABS_X)//获取到了横坐标的值
			{
				if(flag_x == 0)
				{
					flag_x = 1;
					p->x0 = et.value*800/1024;//x0只被赋值一次
				}
				p->x1 = et.value*800/1024;//x1一直被更新
			}
			if(et.type==EV_ABS && et.code==ABS_Y)
			{
				if(flag_y == 0)
				{
					flag_y = 1;
					p->y0 = et.value*480/600;//y0只被赋值一次
				}
				p->y1 = et.value*480/600;//y1一直被更新
			}
			if(et.type==1 && et.code==330 && et.value==0)//动作结束了
			{
				close(fd);
				return;
			}
		}
	}
	close(fd);
	
}


//判断滑动方向
//点击返回0
//左滑返回1，右滑返回2
//上滑返回3，下滑返回4
//左划上划就切下一个，右滑下滑就切上一个
int Sliding_direction(struct coordinates *p)
{
	if(abs(p->x0-p->x1)<=50&&abs(p->y0-p->y1)<=50)
	{
		return 0;
	}
	else
	{
		//左右滑
		if(abs(p->y0-p->y1)<=50)
		{
			if(p->x0 < p->x1)//右滑
			{
				return 2;
			}
			else if(p->x0 > p->x1)//左滑
			{
				return 1;
			}
		}//上下滑
		else if(abs(p->x0-p->x1)<=50)
		{
			if(p->y0 > p->y1)//上划
			{
				return 3;
			}
			else if(p->y0 < p->y1)//下滑
			{
				return 4;
			}
		}
	}
	
}

//判断点击区域
int Click_area(struct coordinates *p)
{
	//坐标在电子相册区域
	if(p->x0 >= 638 && p->x0 <= 717 && p->y0 >= 139 && p->y0 <= 187)
	{
		return Electronic_photo_album;
	}//音乐播放器
	else if(p->x0 >= 219 && p->x0 <= 291 && p->y0 >= 134 && p->y0 <= 190)
	{
		return Music_player;
	}//视频播放器
	else if(p->x0 >= 381 && p->x0 <= 458 && p->y0 >= 134 && p->y0 <= 187)
	{
		return Video_player;
	}//游戏1
	else if(p->x0 >= 213 && p->x0 <= 297 && p->y0 >= 255 && p->y0 <= 325)
	{
		return sudoku_Game;
	}//锁屏
	else if(p->x0 >= 658 && p->x0 <= 710 && p->y0 >= 348 && p->y0 <= 407)
	{
		return lock_screen;
	}//修改密码
	else if(p->x0 >= 388 && p->x0 <= 452 && p->y0 >= 352 && p->y0 <= 422)
	{
		return Modify_password;
	}
	else if(p->x0 >= 215 && p->x0 <= 298 && p->y0 >= 371 && p->y0 <= 443)
	{
		return snake_Game;
	}
	else//无效点击区域
	{
		return Invalid_click;
	}
}




//正确返回1，错误返回0
int password(char *num)
{
	int count = 0;//四位数密码
	int click;
	int array[4];//用来存储输入的密码
	memset(array,-1,sizeof(array));
	struct coordinates dd;
	while(1)
	{
		Get_coordinates(&dd);
		//判断是不是点击
		click = Sliding_direction(&dd);
		//如果不是点击，则重新点击再做后续操作
		if(click != 0)
		{
			continue;
		}
		if(dd.x0 >= 0 && dd.x0 <= 75 && dd.y0 >= 0 && dd.y0 <= 75)
		{
			array[count++] = 1;
			display_bmp("./digit/digit1.bmp",200+count*80,370);
		}
		else if(dd.x0 >= 88 && dd.x0 <= 156 && dd.y0 >= 50 && dd.y0 <= 123)
		{
			array[count++] = 2;
			display_bmp("./digit/digit2.bmp",200+count*80,370);
		}
		else if(dd.x0 >= 165 && dd.x0 <= 240 && dd.y0 >= 0 && dd.y0 <= 80)
		{
			array[count++] = 3;
			display_bmp("./digit/digit3.bmp",200+count*80,370);
		}
		else if(dd.x0 >= 263 && dd.x0 <= 327 && dd.y0 >= 49 && dd.y0 <= 118)
		{
			array[count++] = 4;
			display_bmp("./digit/digit4.bmp",200+count*80,370);
		}
		else if(dd.x0 >= 335 && dd.x0 <= 410 && dd.y0 >= 0 && dd.y0 <= 68)
		{
			array[count++] = 5;
			display_bmp("./digit/digit5.bmp",200+count*80,370);
		}
		else if(dd.x0 >= 410 && dd.x0 <= 485 && dd.y0 >= 68 && dd.y0 <= 138)
		{
			array[count++] = 6;
			display_bmp("./digit/digit6.bmp",200+count*80,370);
		}
		else if(dd.x0 >= 503 && dd.x0 <= 567 && dd.y0 >= 0 && dd.y0 <= 75)
		{
			array[count++] = 7;
			display_bmp("./digit/digit7.bmp",200+count*80,370);
		}
		else if(dd.x0 >= 568 && dd.x0 <= 642 && dd.y0 >= 50 && dd.y0 <= 129)
		{
			array[count++] = 8;
			display_bmp("./digit/digit8.bmp",200+count*80,370);
		}
		else if(dd.x0 >= 646 && dd.x0 <= 705 && dd.y0 >= 0 && dd.y0 <= 75)
		{
			array[count++] = 9;
			display_bmp("./digit/digit9.bmp",200+count*80,370);
		}
		else if(dd.x0 >= 724 && dd.x0 <= 785 && dd.y0 >= 80 && dd.y0 <= 144)
		{
			array[count++] = 0;
			display_bmp("./digit/digit0.bmp",200+count*80,370);
		}
		else//无效区域
		{
			continue;
		}
		if(count == 4)
		{
			break;
		}
	}
	//比较正确的密码与输入的密码
	for(int i=0;i<4;i++)
	{
		if(num[i] -48 != array[i])
		{
			return 0;
		}
	}
	return 1;
}


