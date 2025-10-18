#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "lcd.h"
int fd;
int *plcd = NULL;
//初始化屏幕，即映射屏幕
void lcd_init()
{
	fd = open("/dev/fb0",O_RDWR);
	if(-1 == fd)
	{
		perror("open failed");
		return ;
	}
	plcd = mmap(NULL, 800*480*4,PROT_WRITE | PROT_READ , MAP_SHARED,fd,0);
}

//画点函数
void lcd_draw_point(int x,int y,int color)
{
	if(x>=0 && x<800 && y>=0 && y<480)
	{
		*(plcd+800*y+x) = color;
	}
}

//画圆函数
/*
	x0,y0是圆心坐标
	r是圆的半径
	color圆的颜色
*/

void draw_circle(int x0,int y0,int r,int color)
{
	for(int x = 0; x < 800; x++)
	{
		for(int y = 0; y < 480; y++)
		{
			if((x-x0)*(x-x0) + (y-y0)*(y-y0) <= r*r)
			{
				lcd_draw_point(x,y,color);
			}
		}
	}
}
//画圆函数
/*
	x0,y0是圆心坐标
	r是圆的半径
	color圆的颜色
*/

void draw_circle_up(int x0,int y0,int r,int color)
{
	for(int x = 0; x < 800; x++)
	{
		for(int y = 0; y < 480; y++)
		{
			if((x-x0)*(x-x0) + (y-y0)*(y-y0) <= r*r && y-y0<=0)
			{
				lcd_draw_point(x,y,color);
			}
		}
	}
}

//画圆函数
/*
	x0,y0是圆心坐标
	r是圆的半径
	color圆的颜色
*/

void draw_circle_down(int x0,int y0,int r,int color)
{
	for(int x = 0; x < 800; x++)
	{
		for(int y = 0; y < 480; y++)
		{
			if((x-x0)*(x-x0) + (y-y0)*(y-y0) <= r*r && y-y0>=0)
			{
				lcd_draw_point(x,y,color);
			}
		}
	}
}
//画圆函数
/*
	x0,y0是圆心坐标
	r是圆的半径
	color圆的颜色
*/

void draw_circle_left(int x0,int y0,int r,int color)
{
	for(int x = 0; x < 800; x++)
	{
		for(int y = 0; y < 480; y++)
		{
			if((x-x0)*(x-x0) + (y-y0)*(y-y0) <= r*r && x-x0<=0)
			{
				lcd_draw_point(x,y,color);
			}
		}
	}
}

//画圆函数
/*
	x0,y0是圆心坐标
	r是圆的半径
	color圆的颜色
*/

void draw_circle_right(int x0,int y0,int r,int color)
{
	for(int x = 0; x < 800; x++)
	{
		for(int y = 0; y < 480; y++)
		{
			if((x-x0)*(x-x0) + (y-y0)*(y-y0) <= r*r && x-x0>=0)
			{
				lcd_draw_point(x,y,color);
			}
		}
	}
}

//画椭圆函数
/*
	x0,y0是椭圆圆心坐标
	a是椭圆的长半轴
	b是椭圆的短半轴
	color椭圆的颜色
*/

void draw_tuoyuan(int x0,int y0,int a,int b,int color)
{
	for(int x = 0; x < 800; x++)
	{
		for(int y = 0; y < 480; y++)
		{
			if((x-x0)*(x-x0)*b*b + (y-y0)*(y-y0)*a*a <= a*a*b*b)
			{
				lcd_draw_point(x,y,color);
			}
		}
	}
}

void draw_tuoyuan_up(int x0,int y0,int a,int b,int color)
{
	for(int x = 0; x < 800; x++)
	{
		for(int y = 0; y < 480; y++)
		{
			if((x-x0)*(x-x0)*b*b + (y-y0)*(y-y0)*a*a <= a*a*b*b && y<=y0)
			{
				lcd_draw_point(x,y,color);
			}
		}
	}
}
void draw_tuoyuan_down(int x0,int y0,int a,int b,int color)
{
	for(int x = 0; x < 800; x++)
	{
		for(int y = 0; y < 480; y++)
		{
			if((x-x0)*(x-x0)*b*b + (y-y0)*(y-y0)*a*a <= a*a*b*b && y>=y0)
			{
				lcd_draw_point(x,y,color);
			}
		}
	}
}
void draw_tuoyuan_left(int x0,int y0,int a,int b,int color)
{
	for(int x = 0; x < 800; x++)
	{
		for(int y = 0; y < 480; y++)
		{
			if((x-x0)*(x-x0)*b*b + (y-y0)*(y-y0)*a*a <= a*a*b*b && x<=x0)
			{
				lcd_draw_point(x,y,color);
			}
		}
	}
}
void draw_tuoyuan_right(int x0,int y0,int a,int b,int color)
{
	for(int x = 0; x < 800; x++)
	{
		for(int y = 0; y < 480; y++)
		{
			if((x-x0)*(x-x0)*b*b + (y-y0)*(y-y0)*a*a <= a*a*b*b && x>=x0)
			{
				lcd_draw_point(x,y,color);
			}
		}
	}
}

//画矩形的函数
/*
	x0,y0是矩形的左上角的起点
	c是矩形长
	k是矩形的宽
	color矩形的颜色
*/
void draw_rect(int x0,int y0,int c,int k,int color)
{
	for(int x = x0; x < c + x0 ; x++)
	{
		for(int y = y0; y < k + y0; y++)
		{
			lcd_draw_point(x,y,color);
		}
	}
}
//解映射屏幕
void lcd_close()
{
	int ret = munmap(plcd, 800*480*4);
	if(-1 == ret)
	{
		perror("munmap failed");
		return;
	}
	close(fd);
}

