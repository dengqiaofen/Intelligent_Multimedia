#ifndef __SNAKE_H__
#define __SNAKE_H__

//蛇身体的每一个结点的信息
typedef struct snake_node
{
	//圆心
	int xCenter;
	int yCenter;
	//半径
	int r;
	//结点颜色
	int color;
	//指向下一个结点
	struct snake_node *next;
	//指向上一个结点
	struct snake_node *prev;
}snake_node;
//蛇的管理结点，不属于身体结点
typedef struct snake_list
{
	int length;
	int score;
	snake_node *first;
	snake_node *last;
}snake_list;

//初始化蛇
snake_list *init_snake();
//生成蛇结点
snake_node *create_node();

//将蛇的结点尾插插入到蛇尾
void insert_snake_node(snake_list *s,snake_node* p);
//显示游戏主界面，可以进行一个难度的选择
int display_snake_main(void);
//游戏结束
void gameoverfun(void);
//分数显示
void display_score(int num);
//用一个线程不断地更新蛇的位置
void *draw_snake(void *arg);
void *draw_food(void *arg);

//进入游戏界面
void snake_game(void);

//贪吃蛇
void snake(void);






#endif
