#include "save_all_same.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "bmp.h"
#include "lcd.h"
#include"slip_or_click.h"

//初始化链表
linklist *init_list()
{
	linklist* list = malloc(sizeof(linklist));
	list->first = list->last = NULL;
	return list;
}

//给链表添加元素
void insert_node(linklist* list,ELemtype path)
{
	if(list == NULL)
	{
		return ;
	}
	node *p = malloc(sizeof(node));
	int len = strlen(path);
	p->data = malloc(len+1);
	strcpy(p->data,path);
	p->next = p->prev = NULL;
	if(list->first == NULL)
	{
		list->first = list->last = p;
		list->last->next = list->first;//实现循环
		list->first->prev = list->last;
	}
	else
	{
		list->last->next = p;
		p->prev = list->last;
		list->last = p;
		list->last->next = list->first;//实现循环
		list->first->prev = list->last;
	}

}

//遍历链表，获取链表中的数据
void print_all_pic(const char* pathname)
{
	linklist* list = init_list();
	save_all_same(pathname,list,".bmp");
	node *p = list->first;
	struct coordinates dd;
	//相册的背景
	display_bmp("./bj_and_tb/picbj.bmp",0,0);
	while(p)
	{
		display_bmp(p->data,100,50);
		Get_coordinates(&dd);
		int direction = Sliding_direction(&dd);
		switch (direction)
		{
			case CLICK:
				//如果是点击的左上角就退出
				if(dd.x0 >= 0 && dd.x0 <= 80 && dd.y0 >= 0 && dd.y0 <= 100)
				{
					free(list);
					return ;
				}
				//否则继续查看图片
				continue;
			case MOVE_LEFT:
				p=p->next;
				break;
			case MOVE_RIGHT:
				p=p->prev;
				break;
			case MOVE_UP:
				p=p->next;
				break;
			case MOVE_DOWN:
				p=p->prev;
				break;	
		}
	}
}
void Play_music(const char* pathname)
{
	linklist* list1 = init_list();
	save_all_same(pathname,list1,".mp3");
	node *p1 = list1->first;
	struct coordinates dd;
	//背景
	display_bmp("./bj_and_tb/picbj.bmp",0,0);
	char buf[256] = {0};
	int stop_and_continue = 0;//继续与暂停的标志
	while(p1)
	{
		sprintf(buf,"madplay -Q %s &",p1->data);
		system(buf);
		int len = strlen(p1->data);
		char *s = malloc(len+1);
		memset(s,0,len+1);
		strncpy(s,p1->data,len-3);
		strcat(s,"bmp");//显示与歌名一样的图片
		display_bmp(s,0,0);
		display_bmp("./bj_and_tb/close.bmp",0,0);//退出的图标
		if(stop_and_continue == 0)
		{
			display_bmp("./bj_and_tb/stop.bmp",0,380);
		}
		else if(stop_and_continue == 1)
		{
			display_bmp("./bj_and_tb/continue.bmp",0,380);
		}
		Get_coordinates(&dd);
		int direction = Sliding_direction(&dd);
		switch (direction)
		{
			case CLICK:
				//如果是点击的左上角就退出
				if(dd.x0 >= 0 && dd.x0 <= 100 && dd.y0 >= 0 && dd.y0 <= 100)
				{
					free(list1);
					system("killall madplay");
					return ;
				}
				if(dd.x0 >= 0 && dd.x0 <= 100 && dd.y0 >= 380 && dd.y0 <= 480 && stop_and_continue == 0)//暂停区域
				{
					stop_and_continue = 1;
					system("killall -STOP madplay");
				}
				else if(dd.x0 >= 0 && dd.x0 <= 100 && dd.y0 >= 380 && dd.y0 <= 480 && stop_and_continue == 1)//继续播放
				{
					stop_and_continue = 0;
					system("killall -CONT madplay");
				}
				//否则点击无效，不做处理
				continue;
			case MOVE_LEFT:
				system("killall madplay");
				p1=p1->next;
				break;
			case MOVE_RIGHT:
				system("killall madplay");
				p1=p1->prev;
				break;
			case MOVE_UP:
				system("killall madplay");
				p1=p1->next;
				break;
			case MOVE_DOWN:
				system("killall madplay");
				p1=p1->prev;
				break;	
		}
	}
}


//播放视频
void Play_video(const char* pathname)
{
	linklist* list1 = init_list();
	save_all_same(pathname,list1,".mp4");
	node *p1 = list1->first;
	struct coordinates dd;
	
	//背景
	display_bmp("./bj_and_tb/picbj.bmp",0,0);
	char buf[256] = {0};
	int stop_and_continue = 0;//继续与暂停的标志
	int direction,ret;
	while(p1)
	{
		sprintf(buf,"mplayer -x 800 -y 480 -Zoom  %s &",p1->data);
		system(buf);
		display_bmp("./bj_and_tb/close.bmp",0,0);//退出的图标
		Get_coordinates(&dd);
		direction = Sliding_direction(&dd);
		ret = 0;
		while(1)
		{
			switch (direction)
			{
				case CLICK:
					//如果是点击的左上角就退出
					if(dd.x0 >= 0 && dd.x0 <= 100 && dd.y0 >= 0 && dd.y0 <= 100)
					{
						free(list1);
						system("killall mplayer");
						return ;
					}
					if(dd.x0 >= 0 && dd.x0 <= 100 && dd.y0 >= 380 && dd.y0 <= 480 && stop_and_continue == 0)//暂停区域
					{
						stop_and_continue = 1;
						display_bmp("./bj_and_tb/continue.bmp",0,380);
						system("killall -STOP mplayer");
					}
					else if(dd.x0 >= 0 && dd.x0 <= 100 && dd.y0 >= 380 && dd.y0 <= 480 && stop_and_continue == 1 )//继续播放
					{
						stop_and_continue = 0;
						display_bmp("./bj_and_tb/stop.bmp",0,380);
						system("killall -CONT mplayer");
					}
					//否则点击无效，不做处理
					break;
				case MOVE_LEFT:
					system("killall mplayer");
					ret = 1;
					p1=p1->next;
					break;
				case MOVE_RIGHT:
					system("killall mplayer");
					p1=p1->prev;
					ret = 1;
					break;
				case MOVE_UP:
					system("killall mplayer");
					p1=p1->next;				
					ret = 1;
					break;
				case MOVE_DOWN:
					system("killall mplayer");
					p1=p1->prev;
					ret = 1;
					break;	
			}
			if(ret == 1)
			{
				break;
			}
			Get_coordinates(&dd);
			direction = Sliding_direction(&dd);
			
		}
	}
}



void save_all_same(const char* pathname,linklist *list,const char *suffix)
{
	DIR *dir = opendir(pathname);
	if(NULL == dir)
	{
		perror("opendir failed");
		return ;
	}
	struct dirent* dirp ;
	char buf[256] = {0};
	struct stat st;
	while(dirp= readdir(dir))
	{	
		if(!strcmp(dirp->d_name,".") || !strcmp(dirp->d_name,".."))
		{
			continue;
		}
		int len = strlen(dirp->d_name);
		char *a = dirp->d_name;
		a = a + len - strlen(suffix);
		//如果是.bmp结尾的文件，就是我们想要保存的图片，就将图片的路径保存到链表当中
		if(strcmp(a,suffix) == 0)
		{
			sprintf(buf,"%s%s",pathname,dirp->d_name);
			insert_node(list,buf);
		}
		stat(dirp->d_name,&st);
		//如果是目录就要搜寻目录下还有没有后缀为suffix的文件
		if(S_ISDIR(st.st_mode))//目录文件
		{
			//如果是目录就要在路径最后追加一个/

			sprintf(buf,"%s%s%s",pathname,dirp->d_name,"/");
			
			save_all_same(buf,list,suffix);
		}
	}
	closedir(dir);
}
