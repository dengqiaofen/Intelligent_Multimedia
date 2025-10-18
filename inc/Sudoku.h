#ifndef __SUDOKU_H__
#define __SUDOKU_H__

//生成一个数独
void dfs(int pos); 
void solveSudoku();

//解挖完空的数独
void dfs1(int pos);
void solveSudoku1();


//用创建的有效数独形成一个题解
void creat_unique_solution(int count);

//显示挖完空的数独
void display_sudoku(void);

//解题
void solve(int count);

//数独函数
void sudoku();

//选择做题模式，简单，中等，困难
int display_main(int *count);
//去做一个判断，是失败还是胜利
void win_or_fail(int count,int array_flag[][count*9],int flag,int length);

//判断点的是哪一个数字
int is_digit(int hang_index,int lie_index);
//播放背景音乐
void *play_BGM(void* arg);
#endif