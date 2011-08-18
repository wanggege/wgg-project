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
    *((u32_t *)fb_v.memo + y * fb_v.h + x) = color; //计算该坐标将对原坐标的地址并画点
	return 0;
}
