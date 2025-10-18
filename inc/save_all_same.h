#ifndef __SAVE_ALL_SAME_H__
#define __SAVE_ALL_SAME_H__

typedef char* ELemtype;
typedef struct node
{
	ELemtype data;//存储的是每个文件的路径
	struct node *next;
	struct node *prev;
}node;
typedef struct linklist
{
	node *first;
	node *last;
}linklist;
enum mov_dir
{
	CLICK,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN
};

//初始化链表
linklist *init_list();

//给链表添加元素
void insert_node(linklist* list,ELemtype path);


//查看图片
void print_all_pic(const char* pathname);
//播放音乐
void Play_music(const char* pathname);
//播放视频
void Play_video(const char* pathname);

//保存一个文件夹下所有的.bmp图片
//或者.mp3文件，总之就是保存后缀是suffix
void save_all_same(const char* pathname,linklist *list,const char *suffix);











#endif
