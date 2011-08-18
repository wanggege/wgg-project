#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "various.h"


#define BORD  0x00000fff          //鼠标边界颜色
#define X___  0x000fffff          //鼠标内部颜色
#define T___  0x00000000          //鼠标外部颜色


#define C_W 10                     //鼠标像素数组下标
#define C_H 17                     //鼠标像素数组下标


//鼠标像素数组
static u32_t cursor_pixel[C_W*C_H]=
{
    BORD,T___,T___,T___,T___,T___,T___,T___,T___,T___,
    BORD,BORD,T___,T___,T___,T___,T___,T___,T___,T___,
    BORD,X___,BORD,T___,T___,T___,T___,T___,T___,T___,
    BORD,X___,X___,BORD,T___,T___,T___,T___,T___,T___,
    BORD,X___,X___,X___,BORD,T___,T___,T___,T___,T___,

    BORD,X___,X___,X___,X___,BORD,T___,T___,T___,T___,
    BORD,X___,X___,X___,X___,X___,BORD,T___,T___,T___,
    BORD,X___,X___,X___,X___,X___,X___,BORD,T___,T___,
    BORD,X___,X___,X___,X___,X___,X___,X___,BORD,T___,
    BORD,X___,X___,X___,X___,X___,X___,X___,X___,BORD,

    BORD,X___,X___,X___,X___,X___,BORD,BORD,BORD,BORD,
    BORD,X___,X___,BORD,X___,X___,BORD,T___,T___,T___,
    BORD,X___,BORD,T___,BORD,X___,X___,BORD,T___,T___,
    BORD,BORD,T___,T___,BORD,X___,X___,BORD,T___,T___,
    T___,T___,T___,T___,T___,BORD,X___,X___,BORD,T___,

    T___,T___,T___,T___,T___,BORD,X___,X___,BORD,T___,
    T___,T___,T___,T___,T___,T___,BORD,BORD,T___,T___,
};

u32_t bg[C_H*C_W] = {0};

/**********************
*函 数 名：draw_cursor
*函数功能：在屏幕上画一个鼠标
*输入参数：x，y鼠标位置
*作    者：王阁阁
*日    期：2011-08-18
*************************/

int draw_cursor(int x, int y)
{
    int i;
    int j;

    save_bg(x, y);
    for (j = 0; j < C_H; j++)         
    {
        for (i = 0; i < C_W; i++) 
        {
            fb_one_pixel(x+i,y+j,cursor_pixel[i+j*C_W]);  //打印鼠标像素数组中的每个点
        }
    }

    return 0;
}

/******************************
*函 数 名：save_bg
*函数功能：记录鼠标此刻的位置状态
*输入参数：x，y-鼠标此刻的位置坐标
*作    者：王阁阁
*日    期：2011-08-18
******************************/

int save_bg(int x, int y)
{
    int i = 0;
	int j = 0;

	for (j = 0; j < C_H; j++) 
	{
        for (i = 0; i < C_W; i++) 
        {
            bg[i+j*C_W] = *((u32_t *)fb_v.memo+x+i+(y+j)*fb_v.w);               //记录此刻的位置及状态
        }
	}

	return 0;
}

/******************************
*函 数 名：restore_bg
*函数功能：恢复鼠标前一时刻位置状态
*输入参数：x，y-鼠标前一时刻的位置坐标
*作    者：王阁阁
*日    期：2011-08-18
******************************/
int restore_bg(int x, int y)
{
    int i = 0;
	int j = 0;

	for (j = 0; j < C_H; j++) 
	{
        for (i = 0; i < C_W; i++) 
        {
            fb_one_pixel(x+i, y+j, bg[i+j*C_W]);//恢复打印鼠标前的状态
        }
	}

    return 0;
}


/******************************
*函 数 名：get_mouse_info
*函数功能：取得鼠标此刻的详细信息
*输入参数：fd-鼠标设备文件的文件描述符,m_e-存储鼠标信息的结构体
*作    者：王阁阁
*日    期：2011-08-18
******************************/
int get_mouse_info(int fd, mouse_event *m_e)
{
    char buff[8];
	int n = 0;
	n = read(fd, buff, 3);
	if(3 == n)
	{
	    m_e->dx = buff[1];              //得到x的相对位移
		m_e->dy = -buff[2];             //得到y的相对位移
		m_e->button = (buff[0] & 0x07); //得到鼠标按键的状态
	}

	return n;
}


/******************************
*函 数 名：mouse_doing
*函数功能: 使鼠标能在整个屏幕内任意移动
*输入参数：无
*作    者：王阁阁
*日    期：2011-08-18
******************************/
int mouse_doing(void)
{
    int fd = 0;             //鼠标设备文件的文件表述符
	mouse_event m_e;        //存储鼠标信息的结构体
	
	fd = open("/dev/input/mice",O_RDWR|O_NONBLOCK);  //非阻塞方式打开设备文件
	if(fd == -1)
	{
	    perror("mice open");
		exit(0);
	}

	mx = fb_v.w/2;              //初始化鼠标的横坐标
	my = fb_v.h/2;              //初始化鼠标的纵坐标

    draw_cursor(mx, my);         

	while(1)
	{
	    if(get_mouse_info(fd, &m_e) > 0)
	    {
	        restore_bg(mx, my);
		    mx += m_e.dx;         //移动的下一位置的横坐标
		    my += m_e.dy;         //移动的下一位置的纵坐标

			if(mx < 0)            //鼠标移出屏幕左边界时
			{
			    mx = 0;
			}
			if(mx > (fb_v.w-C_W))  //鼠标移出屏幕右边界时
			{
			    mx = fb_v.w - C_W;
			}
			if(my < 0)             //鼠标移出屏幕上边界时
			{
			    my = 0;
			}
			if(my > (fb_v.h-C_H))   //鼠标移出屏幕下边界时
			{
			    my = fb_v.h - C_H;
			}

			switch(m_e.button)
			{
			    case 1:fb_circle(mx,my,13,0x00ffffff);
				       break;
				case 2:break;
				case 4:break;
				default:break;
			}

		    draw_cursor(mx, my);
	    }
    }

	return 0;

}
