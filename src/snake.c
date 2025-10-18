#include "snake.h"
#include "bmp.h"
#include "slip_or_click.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>


//颜色数组，会随机生成这里面的一种颜色成为食物的颜色，被吃掉后就是身体的颜色
int color[12] = {
	0xD98C8C,0xBAE8CB,0xECF6E4,0xECD6C6,
	0xC2EBEB,0xB390DA,0x4BC3BB,0x7936A1,
	0xDED5F1,0xA83C38,0x733826,0x2D8655
	};

int food[3] = {5,7,9};
int food_x,food_y,food_size;
int speed;//贪吃蛇走的速度，也就是睡眠多久走一下
int up,down,left,right;
int gameover;
int color_i;//当前食物的颜色在颜色数组中的下标
int eat;
int final_score;//记分
int new_food;//判断当前触碰到的食物是否是新事物，就还没有吃过的
//初始化蛇
snake_list *init_snake()
{
	snake_list *s = malloc(sizeof(snake_list));
	s->first = s->last = NULL;
	s->length = 0;
	s->score = 0;
	return s;
}

//将蛇的结点尾插插入到蛇尾
void insert_snake_node(snake_list *s,snake_node *p)
{
	if(s->first == NULL)
	{
		s->first = s->last = p;
	}
	else
	{
		s->last->next = p;
		p->prev = s->last;
		s->last = p;
	}
	s->length++;
}

//显示游戏主界面，可以进行一个难度的选择
int display_snake_main(void)
{
	display_bmp("./snake/main.bmp", 0, 0);
	int click;
	struct coordinates dd;
	while(1)
	{
		Get_coordinates(&dd);
		//判断是不是点击
		click = Sliding_direction(&dd);
		if(click == 0)
		{
			if(dd.x0 >= 0 && dd.x1 <= 80 && dd.y0 >= 0 && dd.y1 <= 50)//返回
			{
				return 0;
			}
			else if(dd.x0 >= 314 && dd.x1 <= 461 && dd.y0 >= 267 && dd.y1 <= 303 )//简单模式
			{
				speed = 1000000;
				return 1;
			}
			else if(dd.x0 >= 314 && dd.x1 <= 461 && dd.y0 >= 322 && dd.y1 <= 362)//中等模式
			{
				speed = 500000;
				return 1;
			}
			else if(dd.x0 >= 314 && dd.x1 <= 461 && dd.y0 >= 377 && dd.y1 <= 415)//困难模式
			{
				speed = 100000;
				return 1;
			}
		}
	}
}

//生成蛇结点
snake_node *create_node()
{
	snake_node *n = malloc(sizeof(snake_node));
	n->prev = NULL;
	n->next = NULL;
	n->r = 5;
	return n;
}

//分数显示
void display_score(int num)
{
	if(num == 0)
	{
		display_bmp("./snake/0.bmp", 740, 76);
		return ;
	}
	char as[4] = {0};
	int i = 4;
	while(num)
	{
		as[--i] = num%10 + '0';
		num = num / 10;
		
	}
	char buf[64] = {0};
	while(i < 4)
	{
		sprintf(buf,"./snake/%c.bmp",as[i]);
		display_bmp(buf, 590 + i*50, 73);
		i++;
	}
	return;
}

//用一个线程不断地更新蛇的位置
void *draw_snake(void *arg)
{
	snake_list *s = (snake_list*)arg;
	while(1)
	{
		//暂时保存最后一个结点的值
		snake_node *p = s->last;
		int last_x = p->xCenter;
		int last_y = p->yCenter;
		//更新每个结点的位置
		//后一个结点位置下一次就是前一个结点现在所在的位置
		while(p->prev)
		{
			p->xCenter = p->prev->xCenter;
			p->yCenter = p->prev->yCenter;
			p = p->prev;
		}
		//更新第一个结点的位置
		if(up == 1)
		{
			p->yCenter -= 10;//头向上走一个结点
		}
		else if(down == 1)
		{
			p->yCenter += 10;//头向下走一个结点
		}
		else if(left == 1)
		{
			p->xCenter -= 10;//头向左走一个结点
		}
		else if(right == 1)
		{
			p->xCenter += 10;//头向右走一个结点
		}
		//判断有没有吃到食物
		//如果吃到了就新增结点
		if(((p->yCenter - p->r >= food_y-food[food_size] && p->yCenter - p->r <= food_y + food[food_size]) ||
			(p->yCenter + p->r >= food_y-food[food_size] && p->yCenter + p->r <= food_y + food[food_size]))
			&& ((p->xCenter - p->r >= food_x-food[food_size] && p->xCenter - p->r <= food_x + food[food_size]) || 
			(p->xCenter + p->r >= food_x-food[food_size] && p->xCenter + p->r <= food_x + food[food_size])) 
			&& new_food == 0)
		{
			printf("eat ......\n");
			display_bmp("./snake/center.bmp", 81, 42);
			snake_node* n = create_node();
			n->color = color[color_i];
			eat = 1;
			if(food_size == 0)
			{
				s->score += 1;
			}
			else if(food_size == 1)
			{
				s->score += 2;
			}
			else if(food_size == 2)
			{
				s->score += 3;
			}
			final_score = s->score;
			display_score(s->score);
			//新增结点的位置就在刚刚保存的最后一个结点的位置
			n->xCenter = last_x;
			n->yCenter = last_y;
			//尾插
			insert_snake_node(s,n);
			new_food = 1;
			printf("tou -> x = %d,tou -> y = %d\n",p->xCenter,p->yCenter);
			
		}
		//更新完之后就在界面上画出蛇，此时的p是指向第一个结点的
		
		while(p)
		{
			draw_circle(p->xCenter, p->yCenter, p->r, p->color);
			p = p->next;
		}
		draw_circle(last_x, last_y, 5, 0x0f192e);
		//如果碰到边界了，游戏结束
		p = s->first;
		if((p->xCenter - 5) < 80 || (p->xCenter + 5) > 580 || (p->yCenter - 5) < 40 || (p->yCenter + 5) > 440)
		{
			gameover = 1;
			break;
		}
		//如果头碰到了身体同样也要结束游戏
		p = p->next;
		while(p)
		{
			if(s->first->xCenter == p->xCenter && s->first->yCenter == p->yCenter)
			{
				gameover = 1;
				break;
			}

			p = p->next;
		}
		if(gameover == 1)
		{
			break;
		}
		usleep(speed);
	}
	
	pthread_exit(NULL);
}
void *draw_food(void *arg)
{
	while(1)
	{
		//食物的坐标及大小及颜色
		food_x = rand()%490 + 90;
		food_y = rand()%390 + 50;
		food_size = rand()%3;
		color_i = rand()%12;
		//在屏幕上画出食物
		draw_circle(food_x,food_y, food[food_size], color[color_i]);
		printf("food_x = %d,food_y = %d,food_size = %d,color_i = %d\n",food_x,food_y,food_size,color_i);
		eat = 0;
		new_food = 0;
		while(!eat);
		
		if(gameover == 1)
		{
			break;	
		}
	}
	pthread_exit(NULL);
}
//进入游戏界面
void snake_game(void)
{
	srand(time(NULL));
	display_bmp("./snake/game.bmp", 0, 0);
	display_bmp("./snake/center.bmp", 81, 42);
	snake_list *s = init_snake();
	//生成第一个结点，最开始蛇得有一个头
	snake_node *n = create_node();
	color_i = rand()%12;
	printf("color_i = %d\n",color_i);
	n->color = color[color_i];
	n->xCenter = 330;
	n->yCenter = 240;
	insert_snake_node(s,n);
	struct coordinates dd;
	int click;
	final_score = 0;
	gameover = 0;
	up = 1;
	down = 0;
	left = 0;
	right = 0;
	//画蛇
	pthread_t th_snake,th_food;
	//创建一个线程不断的更新蛇的位置
	int res = pthread_create(&th_snake, NULL, draw_snake, (void*)s);
	if (res != 0)
	{
		perror("pthread_creat error");
		return;
	}
	//创建一个线程画食物的位置，当食物被吃了，就立马更新一个
	res = pthread_create(&th_food, NULL, draw_food, (void*)s);
	if (res != 0)
	{
		perror("pthread_creat error");
		return;
	}
	display_score(0);
	while(1)
	{
		Get_coordinates(&dd);
		click = Sliding_direction(&dd);
		if(click == 0)
		{
			if(dd.x0 >= 0 && dd.x1 <= 80 && dd.y0 >= 0 && dd.y1 <= 50)//退出
			{
				pthread_cancel(th_snake);
				pthread_cancel(th_food);
				return;
			}
			else if(dd.x0 >= 659 && dd.x1 <= 718 && dd.y0 >= 261 && dd.y1 <= 330 && down == 0)//向上
			{
				up = 1;
				down = 0;
				left = 0;
				right = 0;
			}
			else if(dd.x0 >= 667 && dd.x1 <= 724 && dd.y0 >= 394 && dd.y1 <= 465 && up == 0)//向下
			{
				up = 0;
				down = 1;
				left = 0;
				right = 0;
			}
			else if(dd.x0 >= 591 && dd.x1 <= 661 && dd.y0 >= 332 && dd.y1 <= 388 && right == 0)//向左
			{
				up = 0;
				down = 0;
				left = 1;
				right = 0;
			}
			else if(dd.x0 >= 731 && dd.x1 <= 800 && dd.y0 >= 332 && dd.y1 <= 388 && left == 0)//向右
			{
				up = 0;
				down = 0;
				left = 0;
				right = 1;
			}
		}
		if(gameover == 1)
		{
			sleep(1);
			pthread_cancel(th_snake);
			pthread_cancel(th_food);
			gameoverfun();
			return;
		}
		
	}
}

//贪吃蛇
void snake(void)
{
	int yes;
	while(1)
	{
		yes = display_snake_main();
		if(yes == 0)
		{
			return;
		}
		snake_game();
		
	}
}

void gameoverfun(void)
{
	display_bmp("./snake/gameover.bmp", 0, 0);
	//显示分数
	if(final_score == 0)
	{
		display_bmp("./snake/0.bmp", 500, 276);
	}
	else
	{
		char as[4] = {0};
		int i = 4;
		while(final_score)
		{
			as[--i] = final_score%10 + '0';
			final_score = final_score / 10;
			
		}
		char buf[64] = {0};
		int j = i>0?i:1;
		while(i < 4)
		{
			sprintf(buf,"./snake/%c.bmp",as[i]);
			display_bmp(buf, 440 - (j-1)*50 + i*50, 276);
			i++;
		}
	}
	
	struct coordinates dd;
	int click;
	while(1)
	{
		Get_coordinates(&dd);
		click = Sliding_direction(&dd);
		if(click == 0)
		{
			if(dd.x0 >= 0 && dd.x1 <= 80 && dd.y0 >= 0 && dd.y1 <= 50)//退出
			{
				break;
			}
			else if(dd.x0 >= 252 && dd.x1 <= 543 && dd.y0 >= 381 && dd.y1 <= 424)//继续
			{
				break;
			}
		}
	}
}
