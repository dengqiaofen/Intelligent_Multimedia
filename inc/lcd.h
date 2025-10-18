#ifndef __LCD_H__
#define __LCD_H__

//初始化屏幕，即映射屏幕
void lcd_init();

//画点函数
void lcd_draw_point();
//画圆函数
void draw_circle(int x0,int y0,int r,int color);
void draw_circle_up(int x0,int y0,int r,int color);
void draw_circle_down(int x0,int y0,int r,int color);
void draw_circle_left(int x0,int y0,int r,int color);
void draw_circle_right(int x0,int y0,int r,int color);



//画椭圆函数
void draw_tuoyuan(int x0,int y0,int a,int b,int color);
void draw_tuoyuan_up(int x0,int y0,int a,int b,int color);
void draw_tuoyuan_down(int x0,int y0,int a,int b,int color);
void draw_tuoyuan_left(int x0,int y0,int a,int b,int color);
void draw_tuoyuan_right(int x0,int y0,int a,int b,int color);




//画矩形的函数
void draw_rect(int x0,int y0,int c,int k,int color);
//解映射屏幕
void lcd_close();



#endif
