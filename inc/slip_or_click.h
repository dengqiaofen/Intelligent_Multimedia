#ifndef __SLIP_OR_CLICK_H__
#define __SLIP_OR_CLICK_H__

//保存起点坐标以及终点坐标的结构体
struct coordinates
{
	//起点坐标
	int x0;
	int y0;
	//终点坐标
	int x1;
	int y1;
};
enum eara
{
	Invalid_click,//无效点击
	Electronic_photo_album,
	Music_player,
	Video_player,
	sudoku_Game,
	Modify_password,
	snake_Game,
	lock_screen
};
//获取坐标
void Get_coordinates(struct coordinates *p);
void Get_coordinates1(struct coordinates *p);
//判断滑动方向
int Sliding_direction(struct coordinates *p);
//判断点击区域
int Click_area(struct coordinates *p);
//正确返回1，错误返回0
int password(char *num);




#endif