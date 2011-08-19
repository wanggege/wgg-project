#include <stdio.h>
#include "various.h"

/************************
*函 数 名：chess_doing
*函数功能：规范棋子的放置位置-应放在十字交叉点
*作    者：王阁阁
*日    期：2011-08-19
***************************/

int chess_doing(void)
{
    int rx = (mx-ST_X) % SPACE;      //棋子离棋盘中的位置周围交叉点横坐标的距离
    int ry = (my-ST_Y) % SPACE;      //棋子离棋盘中的位置周围交叉点纵坐标的距离

    int cx = mx - rx;                //棋子当前要放置的正确位置纵坐标
	int cy = my - ry;                //棋子当前要放置的正确位置纵坐标

	if((mx < ST_X) || (mx > (ST_X+(X_NUM-1)*SPACE)))  //横坐标超出棋盘边界
	{
	    return 0;
	}
	if((my < ST_Y) || ((my > ST_Y+(Y_NUM-1)*SPACE)))   //纵坐标超出棋盘边界
	{
	    return 0;
	}
	if(rx > SPACE/2)                //距离横坐标最近的十字横坐标
	{
	    cx += SPACE;
	}
	if(ry > SPACE/2)               //距离纵坐标最近的十字纵坐标
	{
	    cy += SPACE;
	}

	fb_circle(cx, cy, 13, 0x00fffff0);

	return 0;
}

