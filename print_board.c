#include <stdio.h>
#include "various.h"

/****************************
*函 数 名：print_board
*函数功能：在屏幕上画棋盘
*作    者：王阁阁
*日    期：2011-08-18
*****************************/

int print_board(void)
{
    int i = 0;

    //打印横线
    for(i = 0; i < Y_NUM; i++)
	{
	    fb_line(ST_X, ST_Y+i*SPACE, ST_X+(X_NUM-1)*SPACE, ST_Y+i*SPACE, 0x0000ff00);
	}

    //打印竖线
	for (i = 0; i < X_NUM; i++) 
	{
		
	    fb_line(ST_X+i*SPACE, ST_Y, ST_X+i*SPACE, ST_Y+(Y_NUM-1)*SPACE, 0x0000ff00);
	}

	return 0;
}
