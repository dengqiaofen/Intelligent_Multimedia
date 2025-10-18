#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include"slip_or_click.h"
#include "bmp.h"
#include "save_all_same.h"
#include <pthread.h>


int line[9][9];
int column[9][9];
int block[3][3][9];
int valid;
int* spaces[81];//用来保存数独中最开始0的位置，为每一个元素开辟两个字节大小，第一个字节存放行下标，第二个字节存放列下标
int spacesSize;//0的个数


//保存初始化的一个数独
char board[9][9] = {0};
char board1[9][9] = {0};//保存挖空完的数独
char board2[9][9] = {0};//拷贝挖空完的数独，去解题，看是否与初始化数独一样，如果一样，就将这个挖空完的数独，给用户去解


void dfs(int pos) 
{
	//如果所有的0都被补全之后就直接返回
    if (pos == spacesSize) {
        valid = 1;//这是个标记，说明数独已经全部填充完了，返回上一级之后就不会在进行for循环了，减少时间复杂度
        return;
    }
	//space里面保存的是每一个0所在的行下标与列下标
    int i = spaces[pos][0], j = spaces[pos][1];
    for (int digit = 0; digit < 9 && !valid; ++digit) 
	{
		//从1-9中选出一个这一行这一列这一块都没有出现过的数，填到数独的二维数组中
        if (!line[i][digit] && !column[j][digit] && !block[i / 3][j / 3][digit]) 
		{
			//填好之后，将这个数字在这一行这一列这一块做一个标记，标志着已经出现过了
            line[i][digit] = column[j][digit] = block[i / 3][j / 3][digit] = 1;
			//把这个数填到数独的二维数组中
            board[i][j] = digit + '0' + 1;
			//在去找下一个0，去填充这个0，依此类推，直到所有的0都被替换完
            dfs(pos + 1);
			//如果之前填的数字有歧义，则会退回，并将这个数出现的标记给清除，重新填数字去递归
            line[i][digit] = column[j][digit] = block[i / 3][j / 3][digit] = 0;
        }
    }
}

void solveSudoku() {
    memset(line, 0, sizeof(int)*9*9);
    memset(column, 0, sizeof(column));
    memset(block, 0, sizeof(block));
    valid = 0;
    spacesSize = 0;
	//首先遍历数独的二维数组，
    for (int i = 0; i < 9; ++i) 
	{
        for (int j = 0; j < 9; ++j) 
		{
			//如果是0，就只要记录0所在的行下标，列下标，并且0的个数+1
            if (board[i][j] == '0') 
			{
                spaces[spacesSize] = malloc(sizeof(int) * 2);
                spaces[spacesSize][0] = i;
                spaces[spacesSize++][1] = j;
            }//如果是1-9，那么line[][],column[][],block[][][]设置为1
			else 
			{
				/*
					line[][]:第一维是这个数字所在的行下标，第二维是这个数字-1，也就是说，记录下这一行，这个数字已经出现过了，后面不能再出现这个数字了
					同理column[][]:第一维就是这个数字所在的列，第二维就是这个数字-1.标记着这一列这个数字已经有了
					block[][][]:第一第二就是这一块，小的9宫格，第三问就是这个数字-1，即这个小块已经有这个数字了
				*/
                int digit = board[i][j] - '0' - 1;
                line[i][digit] = column[j][digit] = block[i / 3][j / 3][digit] = 1;
            }
        }
    }
	//遍历完之后，就去补全完数独，把0补成相应的数字
    dfs(0);
}
void dfs1(int pos) {
    if (pos == spacesSize) {
        valid = 1;
        return;
    }

    int i = spaces[pos][0], j = spaces[pos][1];
    for (int digit = 0; digit < 9 && !valid; ++digit) {
        if (!line[i][digit] && !column[j][digit] && !block[i / 3][j / 3][digit]) {
            line[i][digit] = column[j][digit] = block[i / 3][j / 3][digit] = 1;
            board2[i][j] = digit + '0' + 1;
            dfs1(pos + 1);
            line[i][digit] = column[j][digit] = block[i / 3][j / 3][digit] = 0;
        }
    }
}

void solveSudoku1() {
    memset(line, 0, sizeof(int)*9*9);
    memset(column, 0, sizeof(column));
    memset(block, 0, sizeof(block));
    valid = 0;
    spacesSize = 0;

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board2[i][j] == '0') {
                spaces[spacesSize] = malloc(sizeof(int) * 2);
                spaces[spacesSize][0] = i;
                spaces[spacesSize++][1] = j;
            } else {
                int digit = board2[i][j] - '0' - 1;
                line[i][digit] = column[j][digit] = block[i / 3][j / 3][digit] = 1;
            }
        }
    }

    dfs1(0);
}
//用创建的有效数独形成一个题解
void creat_unique_solution(int count)
{
	while(1)
	{
		int index;
		for(int i = 0; i<9; i++)
		{
			for(int j=0;j<9;j++)
			{
				board1[i][j] = board[i][j];
				board2[i][j] = board[i][j];
			}
			
		}
		//挖空，每行挖四个
		for(int i = 0; i < 9; i++)
		{
			int ret = count;
			while(ret--)
			{
				index = rand()%9;
				board1[i][index] = '0';
				board2[i][index] = '0';
			}
		}
		
		//解生成的这个数独
		solveSudoku1();
		int flag1 = 0,flag2 = 0;
		for(int i = 0; i<9; i++)
		{
			for(int j=0;j<9;j++)
			{
				if(board[i][j] != board2[i][j])
				{
					break;
				}
				flag1++;
			}
			if(flag1 < 9)
			{
				break;
			}
			flag2++;
		}
		if(flag2 == 9)
		{
			return;
		}
	}
}
void display_sudoku(void)
{
	
	for(int i = 0; i<9;i++)
	{
		for(int j = 0;j<9;j++)
		{
			if(board1[i][j] == '0')
			{
				display_bmp("./sudoku_game/kong.bmp",86+j*50,13+i*50);
			}
			else if(board1[i][j] == '1')
			{
				display_bmp("./sudoku_game/11.bmp",86+j*50,13+i*50);
			}
			else if(board1[i][j] == '2')
			{
				display_bmp("./sudoku_game/22.bmp",86+j*50,13+i*50);
			}
			else if(board1[i][j] == '3')
			{
				display_bmp("./sudoku_game/33.bmp",86+j*50,13+i*50);
			}
			else if(board1[i][j] == '4')
			{
				display_bmp("./sudoku_game/44.bmp",86+j*50,13+i*50);
			}
			else if(board1[i][j] == '5')
			{
				display_bmp("./sudoku_game/55.bmp",86+j*50,13+i*50);
			}
			else if(board1[i][j] == '6')
			{
				display_bmp("./sudoku_game/66.bmp",86+j*50,13+i*50);
			}
			else if(board1[i][j] == '7')
			{
				display_bmp("./sudoku_game/77.bmp",86+j*50,13+i*50);
			}
			else if(board1[i][j] == '8')
			{
				display_bmp("./sudoku_game/88.bmp",86+j*50,13+i*50);
			}
			else if(board1[i][j] == '9')
			{
				display_bmp("./sudoku_game/99.bmp",86+j*50,13+i*50);
			}
		}
	}
	//两条横线
	display_bmp("./sudoku_game/rowline.bmp",86,163);
	display_bmp("./sudoku_game/rowline.bmp",86,313);
	//两条竖线
	display_bmp("./sudoku_game/columnline.bmp",236,13);
	display_bmp("./sudoku_game/columnline.bmp",386,13);
}
//去做一个判断，是失败还是胜利
void win_or_fail(int count,int array_flag[][count*9],int flag,int length)
{
	if(flag == 0 && length != 0)
	{
		display_bmp("./sudoku_game/game_win.bmp", 560,0);
		return;
	}
	else //失败
	{
		display_bmp("./sudoku_game/game_fail.bmp", 560,0);
		char buf[128] = {0};
		for(int i = 0;i<length;i++)
		{
			//如果是错误的答案，则显示一张红色的错误，标记这里填错了
			if(board[array_flag[0][i]][array_flag[1][i]] != array_flag[2][i]+48)
			{
				sprintf(buf,"./sudoku_game/%c%c%c.bmp",array_flag[2][i]+48,array_flag[2][i]+48,array_flag[2][i]+48);
				display_bmp(buf,86+array_flag[1][i]*50,13+array_flag[0][i]*50);
			}
		}
		return;
	}
}

//判断点的是哪一个数字
int is_digit(int hang_index,int lie_index)
{
	struct coordinates dd;
	int direction;
	while(1)
	{
		Get_coordinates(&dd);
		direction = Sliding_direction(&dd);
		if(direction == CLICK)
		{
			if(dd.x0 >= 582 && dd.x0 <= 632 && dd.y0 >= 52 && dd.y0 <= 102)//1
			{
				display_bmp("./sudoku_game/1.bmp",86+lie_index*50,13+hang_index*50);
				return 1;
			}
			else if(dd.x0 >= 651 && dd.x0 <= 701 && dd.y0 >= 51 && dd.y0 <= 101)//2
			{
				display_bmp("./sudoku_game/2.bmp",86+lie_index*50,13+hang_index*50);
				return 2;
			}
			else if(dd.x0 >= 717 && dd.x0 <= 767 && dd.y0 >= 50 && dd.y0 <= 100)//3
			{
				display_bmp("./sudoku_game/3.bmp",86+lie_index*50,13+hang_index*50);
				return 3;
			}
			else if(dd.x0 >= 582 && dd.x0 <= 632 && dd.y0 >= 117 && dd.y0 <= 167)//4
			{
				display_bmp("./sudoku_game/4.bmp",86+lie_index*50,13+hang_index*50);
				return 4;
			}
			else if(dd.x0 >= 651 && dd.x0 <= 701 && dd.y0 >= 117 && dd.y0 <= 167)//5
			{
				display_bmp("./sudoku_game/5.bmp",86+lie_index*50,13+hang_index*50);
				return 5;
			}
			else if(dd.x0 >= 717 && dd.x0 <= 767 && dd.y0 >= 117 && dd.y0 <= 167)//6
			{
				display_bmp("./sudoku_game/6.bmp",86+lie_index*50,13+hang_index*50);
				return 6;
			}
			else if(dd.x0 >= 582 && dd.x0 <= 632 && dd.y0 >= 180 && dd.y0 <= 230)//7
			{
				display_bmp("./sudoku_game/7.bmp",86+lie_index*50,13+hang_index*50);
				return 7;
			}
			else if(dd.x0 >= 651 && dd.x0 <= 701 && dd.y0 >= 180 && dd.y0 <= 230)//8
			{
				display_bmp("./sudoku_game/8.bmp",86+lie_index*50,13+hang_index*50);
				return 8;
			}
			else if(dd.x0 >= 717 && dd.x0 <= 767 && dd.y0 >= 180 && dd.y0 <= 230)//9
			{
				display_bmp("./sudoku_game/9.bmp",86+lie_index*50,13+hang_index*50);
				return 9;
			}
		}
	}
	//两条横线
	display_bmp("./sudoku_game/rowline.bmp",86,163);
	display_bmp("./sudoku_game/rowline.bmp",86,313);
	//两条竖线
	display_bmp("./sudoku_game/columnline.bmp",236,13);
	display_bmp("./sudoku_game/columnline.bmp",386,13);
}

//解题
void solve(int count)
{
	display_bmp("./sudoku_game/game1.bmp",0,0);
	while(1)
	{
		int array_flag[3][count*9];//用来保存用户输入哪个位置的数字,第一维保存行下标，第二维保存列下标，第三维保存值
		int start = 0;//数组开始下标
		memset(array_flag,0,sizeof(array_flag));
		int flag=0;//flag标记表示现在记录了多少个不同，最后根据这个flag的值，判断用户答题正确还是错误
		struct coordinates dd;
		int direction;
		//显示这个挖好空的数组在界面上
		
		display_sudoku();
		int complete =0;//判断是否点击了完成的区域
		while(1)
		{
			Get_coordinates(&dd);
			direction = Sliding_direction(&dd);
			if(direction == CLICK)
			{
				if(dd.x0 >= 0 && dd.x0 <= 80 && dd.y0 >= 0 && dd.y0 <= 100)//退出直接退出，不会保留记录
				{
					return;	
				}
				else if(dd.x0 >= 592 && dd.x0 <= 750 && dd.y0 >= 284 && dd.y0 <= 326 && start > 0)//撤销
				{
					start--;
					if(board[array_flag[0][start]][array_flag[1][start]] != array_flag[2][start]+48)
					{
						flag--;
					}
					printf("flag = %d\n",flag);
					display_bmp("./sudoku_game/kong.bmp",86+array_flag[1][start]*50,13+array_flag[0][start]*50);
					//两条横线
					display_bmp("./sudoku_game/rowline.bmp",86,163);
					display_bmp("./sudoku_game/rowline.bmp",86,313);
					//两条竖线
					display_bmp("./sudoku_game/columnline.bmp",236,13);
					display_bmp("./sudoku_game/columnline.bmp",386,13);
					printf("hang_index = %d,lie_index = %d\n",array_flag[0][start],array_flag[1][start]);
					board1[array_flag[0][start]][array_flag[1][start]] = '0';//表示已经填过了
					array_flag[0][start] = 0;//行下标
					array_flag[1][start] = 0;//列下标
					array_flag[2][start] = 0;//值
				}
				else if(dd.x0 >= 592 && dd.x0 <= 750 && dd.y0 >= 346 && dd.y0 <= 392)//重新开始
				{
					break;//退出循环，重新初始化所有东西，数组，局面等等
				}
				else if(dd.x0 >= 592 && dd.x0 <= 750 && dd.y0 >= 412 && dd.y0 <= 455)//完成
				{
					//去做一个判断，是失败还是胜利
					complete = 1;
					win_or_fail(count,array_flag,flag,start);
					break;
				}
				else if(dd.x0 >= 86 && dd.x0 <= 536 && dd.y0 >= 13 && dd.y0 <= 463)//如果是点击数独区域，再另做判断
				{
					//点击了做题区域，先判断这个地方是不是空位，如果是就去获取数字，否则不做处理
					int lie_index = (dd.x0 - 86)/50;
					int hang_index = (dd.y0 - 15)/50;
					printf("hang_index = %d,lie_index = %d\n",hang_index,lie_index);
					if(board1[hang_index][lie_index] == '0')
					{
						//填数字
						display_bmp("./sudoku_game/blue.bmp",86+lie_index*50,13+hang_index*50);
						board1[hang_index][lie_index] = '1';//表示已经填过了
						int dd = is_digit(hang_index,lie_index);
						//填完数字之后要去判断这个数字是否与原数组相同
						array_flag[0][start] = hang_index;//行下标
						array_flag[1][start] = lie_index;//列下标
						array_flag[2][start] = dd;//值
						
						//如果不相同则做一个标记
						//printf("board[%d][%d] = %c\n",array_flag[0][start],array_flag[1][start],board[array_flag[0][start]][array_flag[1][start]]);
						//printf("array[2][start");
						if(board[array_flag[0][start]][array_flag[1][start]] != array_flag[2][start]+48)
						{
							flag++;
						}
						start++;
						printf("flag = %d\n",flag);
					}
					
				}
			}
		}
		if(complete == 1)
		{
			while(1)
			{
				Get_coordinates(&dd);
				direction = Sliding_direction(&dd);
				if(direction == CLICK)
				{
					if(dd.x0 >= 0 && dd.x0 <= 80 && dd.y0 >= 0 && dd.y0 <= 100)//退出直接退出，不会保留记录
					{
						return;	
					}
					else if(dd.x0 >= 599 && dd.x0 <= 768 && dd.y0 >= 306 && dd.y0 <= 390)//如果是点击继续游戏
					{
						return;
					}
				}
			}
		}
	}
}
//选择做题模式，简单，中等，困难
int display_main(int *count)
{
	display_bmp("./sudoku_game/main.bmp", 0, 0);//显示主界面
	int direction;
	
	struct coordinates dd;
	while(1)
	{
		Get_coordinates(&dd);
		direction = Sliding_direction(&dd);
		if(direction == CLICK)
		{
			if(dd.x0 >= 0 && dd.x0 <= 80 && dd.y0 >= 0 && dd.y0 <= 100)//退出直接退出，不会保留记录
			{
				return -1;	
			}
			else if(dd.x0 >= 70 && dd.x0 <= 226 && dd.y0 >= 362 && dd.y0 <= 416)//简单模式
			{
				*count = 3;
				return 0;
			}
			else if(dd.x0 >= 312 && dd.x0 <= 483 && dd.y0 >= 362 && dd.y0 <= 416)//中等模式
			{
				*count = 4;
				return 0;
			}
			else if(dd.x0 >= 574 && dd.x0 <= 735 && dd.y0 >= 362 && dd.y0 <= 416)//困难模式
			{
				*count = 5;
				return 0;
			}
			
		}
	}
}
//播放背景音乐
void *play_BGM(void* arg)
{
	system("madplay -Q ./sudoku_game/bgm.mp3 &");
}

void sudoku()
{
	pthread_t th;
	int ret = pthread_create(&th,NULL,play_BGM,NULL);
	if (ret != 0)
	{
	   perror("pthread_create failed");
	}
	while(1)
	{	
		int count;
		int xx = display_main(&count);
		if(xx == -1)//如果点击的是退出区域，就退出数独这个游戏，否则表示已经进入了游戏界面
		{
			system("killall madplay");
			return;
		}
		for(int i = 0;i<9;i++)
		{
			for(int j =0;j<9;j++)
			{
				board[i][j] = '0';
				board1[i][j] = '0';
				board2[i][j] = '0';
			}
		}
		srand(time(NULL));//生成随机数的种子
		//给数组的00,33,66位置生成1-9的随机数，去生成一个完整的数独
		for(int i=0; i<9 ; i+=3)
		{
			board[i][i] = rand()%9+1 + 48;
		}
		solveSudoku(); //生成一个数独
		//给生成的这个数独挖空，用0来代替
		creat_unique_solution(count);
		//显示这个挖好空的数组在界面上,并且让用户解题
		solve(count);
	}
	
 } 




