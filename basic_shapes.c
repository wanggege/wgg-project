#include <stdio.h>
#include <stdlib.h>
#include "various.h"
#include "func.h"

/****************************************
* 函 数 名：fb_one_pixel
* 功    能：利用framebuff在屏幕上画一个点
* 输入参数：x -横坐标 y-竖坐标 color-点的颜色
* 作    者：王阁阁
* 日    期：2011-08-18
*****************************************/
int fb_one_pixel(int x, int y, u32_t color)
{
    *((u32_t *)fb_v.memo + y * fb_v.w + x) = color; //计算该坐标相对原坐标的地址并画点
	return 0;
}


/************************
*函 数 名：swap
*函数功能：交换a和b的值
*作    者：王阁阁
*日    期：2011-08-18
**************************/
void swap(int *a, int *b)
{
    int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}


/********************************
* 函 数 名：fb_line
* 输入参数：x1,y1起始坐标x2,y2终止坐标,color-颜色
* 函数功能：利用Bresenham算法画直线
* 作    者: 王阁阁
* 日    期：2011-08-18
*********************************/
int fb_line(int x1, int y1, int x2, int y2, u32_t color)
{
    int dx = x2 - x1;
	int dy = y2 - y1;
	int inc = ((dx*dy) > 0 ? 1 : -1);
	int p = 0;

	if(abs(dx) > abs(dy))   //直线与x轴的夹角小于45度，即相斜率为0到45度的直线
	{
	    if(dx < 0)
		{
		    swap(&x1, &x2);
		    swap(&y1, &y2);
			dx = -dx;
			dy = -dy;
		}

		dy = abs(dy);
		p = 2*dy - dx;                 //Bresenham画直线算法

		while(x1 <= x2)
		{
		    fb_one_pixel(x1, y1, color);
			x1++;
			if(p < 0)
			{
			    p += 2*dy;
			}
			else
			{
			    y1 += inc;
				p += 2*(dy - dx);
			}
		}
	}
	else                        //直线与X轴的夹角大于45度
	{
	    if(dy < 0)
		{
		    swap(&x1, &x2);
		    swap(&y1, &y2);
			dx = -dx;
			dy = -dy;
		}
		dx = abs(dx);
		p = 2*dx - dy;
		while(y1 <= y2)
		{
		    fb_one_pixel(x1, y1, color);
			y1++;
			if(p < 0)
			{
			    p += 2*dx;
			}
			else
			{
			    x1 += inc;
				p += 2*(dx - dy);
			}
		}
	}

	return 0;
}

/****************************
*函 数 名:fb_circle
*函数参数：x0,y0-圆心坐标r-圆半径 color-圆的颜色
*函数功能：利用直线画圆
*作    者：王阁阁
*日    期：2011-08-18
*******************************/

int fb_circle(int x0, int y0, int r, u32_t color)
{
    int x = 0;
	int y = r;
	int p = 3 - 2*r;               //画圆算法

	while(x<=y)
	{

#if 0   画虚心圆，根据八分之一圆对称的方法
	    fb_one_pixel(x0+x, y0+y, color);
	    fb_one_pixel(x0+y, y0+x, color);
	    fb_one_pixel(x0+x, y0-y, color);
	    fb_one_pixel(x0+y, y0-x, color);
	    
		fb_one_pixel(x0-x, y0+y, color);
	    fb_one_pixel(x0-y, y0+x, color);
	    fb_one_pixel(x0-y, y0-x, color);
	    fb_one_pixel(x0-x, y0-y, color);

#endif
        //画实心圆点，根据四分之一圆对称的方法
		fb_line(x0+x, y0+y, x0-x, y0+y, color);
		fb_line(x0+y, y0+x, x0-y, y0+x, color);
		fb_line(x0+y, y0-x, x0-y, y0-x, color);
		fb_line(x0+x, y0-y, x0-x, y0-y, color);

        if(p < 0)
		{
		    p += 4*x + 6;
		}
		else
		{
		    p += 4*(x-y) + 10;
			y--;
		}
		x++;
	}

	return 0;
}
