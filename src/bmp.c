#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "lcd.h"
#include "save_all_same.h"


void display_bmp(const char* pathname,int x0,int y0)
{
	
	int fd = open(pathname,O_RDWR);
	if(-1 == fd)
	{
		perror("open bmp failed");
		return ;
	}
	char buf[10] = {0};
	read(fd,buf,2);
	if(buf[0] == 'B' && buf[1] == 'M')
	{
		printf("it is a bmp file!\n");
	}
	else
	{
		printf("it is not a bmp file!\n");
		return;
	}
	int size,width,height,depth;
	read(fd,&size,4);
	lseek(fd,0x12,SEEK_SET);
	read(fd,&width,4);
	read(fd,&height,4);
	lseek(fd,0x1c,SEEK_SET);
	read(fd,&depth,4);
	int line_valid_bytes=abs(width) * depth/8; //一行的有效字节数
	int line_bytes; //一行的总字节数=有效字节数 + 赖子数
	int laizi=0;
	if(line_valid_bytes%4)
	{
		laizi=4-line_valid_bytes%4;
	}
	line_bytes=line_valid_bytes + laizi;
	
	int total_bytes=line_bytes * abs(height);//整个像素数组的大小
	char * p=malloc(total_bytes);// 开辟空间保存像素数组
	lseek(fd,54,SEEK_SET);//将光标移到54字节处读取像素数组
	//读取像素数组
	read(fd,p,total_bytes);
	int x,y;
	unsigned char a,r,g,b;
	int color;
	int i=0;
	for(y=0;y<abs(height);y++)
	{
		for(x=0;x<abs(width);x++)
		{
			b=p[i++];
			g=p[i++];
			r=p[i++];
			if(depth==32)
			{
				a=p[i++];
			}
			else if(depth==24)
			{
				a=0;
			}
			color=(a<<24) | (r<<16) | (g<<8) | b;
			lcd_draw_point(width>0?(x+x0):(abs(width)-1-x+x0),
							height>0?(abs(height)-1-y+y0):(y+y0),color);
		}
		i=i+laizi;
	}


	
//	int total_byte = (width * height) * 4;
//	int *p = malloc(total_byte);
//	int i = 0;
//	lseek(fd,54,SEEK_SET);
//	for(int y = 0; y < abs(height); y++)
//	{
//		for(int x = 0; x < abs(width); x++)
//		{
//			read(fd,&p[i++],depth/8);
//		}
//	}
//	i = 0;
//	for(int y = 0; y < abs(height); y++)
//	{
//		for(int x = 0; x < abs(width); x++)
//		{
//			int color = p[i++];
//			int xx = width > 0 ? x0 + x:(abs(width)+x0-x-1);
//			int yy = height > 0 ? (abs(height)+y0-y-1) : y0+y;
//			lcd_draw_point(xx, yy, color);
//		}
//	}
	free(p);
	close(fd);
		
}

