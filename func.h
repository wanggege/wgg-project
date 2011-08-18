#ifndef FUNC_H
#define FUNC_H

#define ST_X     100           //棋盘横坐标离屏幕横坐标的距离
#define ST_Y     40            //棋盘纵坐标离屏幕纵坐标的距离
#define SPACE    30

#define X_NUM    29            //横向方格的个数
#define Y_NUM    24            //纵向方格的个数

typedef unsigned int u32_t;
typedef unsigned char u8_t;

typedef struct
{
    int w;                         //屏幕的宽度
    int h;                         //屏幕的高度
    int bpp;                       //每个像素点的比特数
    void *memo;
}fbscr_t;

int init_data(void);
int fb_one_pixel(int x, int y, u32_t color);
int fb_line(int x1, int y1, int x2, int y2, u32_t color);
int fb_circle(int x0, int y0, int r, u32_t color);
int print_board();

#endif
