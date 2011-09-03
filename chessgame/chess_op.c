#include <stdio.h>
#include "various.h"

/***********************************
*函 数 名：check
*函数功能：判断此刻棋子放置后是否有五个颜色相同的棋子位于一条直线上
*输入参数：x，y-棋子的坐标
*作    者：王阁阁
*日    期：2011-08-19
*************************************/

int check(int x, int y)
{
    int counter = 1;                    //位于一条直线上相同棋子的个数
	char who = chess_board[x+y*X_NUM];  //棋盘上棋子对应的玩家
	int i = 0;
	int j = 0;

	if(who == 0)                        //该位置为空，即还无玩家放置棋子
	{
	    return 0;
	}

	for (i = 1; i < 5; i++) 
	{
		if(chess_board[x+i+y*X_NUM]==who) //水平直线上是否有相同棋子相邻
		{
		    counter++;
		}
		else
		{
		    break;
		}
	}
    if(counter==5)                      //水平直线上有五个相邻的颜色相同棋子
	{
	    return who;                     //棋子对应的玩家
	}

	counter = 1;
	for (i = 1,j = -1; i < 5; i++,j--) //与x轴夹角是45度角的直线上是否有相同棋子相邻
	{
		if(chess_board[x+i+(y+j)*X_NUM]==who)
		{
		    counter++;
		}
		else
		{
		    break;
		}
	}
	if(counter == 5)
	{
	    return who;
	}

	counter = 1;
	for (j = 1; j < 5; j++)          //垂直方向的直线上是否有相同棋子相邻
	{
        if(chess_board[x+(y+j)*X_NUM] == who)
		{
		    counter++;
		}
		else
		{
		    break;
		}
	}
	if(counter==5)
	{
	    return who;
	}

	counter = 1;
	for (i = 1,j = 1; j < 5; i++,j++) //与x轴夹角为-45度的直线上是否有相同棋子相邻
	{
		if(chess_board[x+i+(y+j)*X_NUM]==who)
		{
		    counter++;
		}
		else
		{
		    break;
		}
	}
	if(counter==5)
	{
	    return who;
	}

	return 0;

}

/**********************************
* 函 数 名: chess_put
* 函数功能：判断游戏是否结束
* 参    数：棋子的放置位置
* 作    者: 王阁阁
* 日    期：2011-08-19
*********************************/

int chess_put(int x, int y)
{
    int i = 0;
	int j = 0;
	int winner = 0;                           //赢家
	chess_board[x+y*X_NUM] = player;          //记录每个玩家的每步棋

	for (j = 0; j < Y_NUM; j++) 
	{
        for (i = 0; i < X_NUM; i++) 
        {
            if((winner = check(i,j)) > 0)     //检查游戏是否结束
			{
			    return winner;
			}
        }
	}

	return 0;
}

/************************
*函 数 名：chess_doing
*函数功能：规范棋子的放置位置-应放在十字交叉点
*作    者：王阁阁
*日    期：2011-08-19
***************************/

int chess_doing(void)
{
    char winner = 0;                 //赢家
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

    if(chess_board[(cx-ST_X)/SPACE+((cy-ST_Y)/SPACE)*X_NUM])//该位置上是否有棋子
	{
	    return 0;
	}
    else
	{
	    fb_circle(cx, cy, 13, current_color);  //放置棋子
	}

    winner = chess_put((cx-ST_X)/SPACE, (cy-ST_Y)/SPACE);  //是否有赢家
	if(winner > 0)
	{
	    return winner;
	}

	if(current_color == WHTTE)         //转换玩家和玩家棋子
	{
	    current_color = BLACK;
		player = 1;
	}
	else
	{
	    current_color = WHTTE;
		player = 2;
	}

	return 0;
}

