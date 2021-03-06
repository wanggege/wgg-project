#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "func.h"

fbscr_t fb_v;
int mx;            //鼠标位置横坐标
int my;            //鼠标位置纵坐标

char chess_board[X_NUM*Y_NUM];       //棋盘标记
u32_t current_color;                 //当前进行的玩家棋子颜色
char player;                         //当前玩家

/******************************************
* 函数名：init_data
* 功  能: 将屏幕缓冲区映射到用户空间，并初始化
* 作  者：王阁阁
* 日  期：2011-08-17
*******************************************/

int init_data(void)
{
    int fd = 0;
    long int screensize = 0;
    struct fb_var_screeninfo fb_var; //系统自定义屏幕信息结构体

    fd = open("/dev/fb0",O_RDWR);    //打开设备文件
    if(fd < 0)
    {
        perror("open fb0");
        exit(0);
    }

    if(ioctl(fd, FBIOGET_VSCREENINFO, &fb_var)) //取得当前屏幕的参数
    {
        perror("ioctl");
        exit(0);
    }

    fb_v.w = fb_var.xres;
    fb_v.h = fb_var.yres;
    fb_v.bpp = fb_var.bits_per_pixel;
    screensize = fb_v.w * fb_v.h * fb_v.bpp / 8;

    fb_v.memo = mmap(NULL, screensize, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);                 //将屏幕缓冲区映射到用户空间
    if(fb_v.memo == MAP_FAILED)
    {
        perror("map");
        exit(0);
    }

    memset(fb_v.memo, 0, screensize);   //清屏
    memset(chess_board, 0 ,X_NUM*Y_NUM);  //棋盘标记数组初始化
    current_color = BLACK;                //玩家一的棋子颜色
	player = 1;                           //玩家一先开始游戏

    return 0;
}
