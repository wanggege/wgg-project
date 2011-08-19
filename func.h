#ifndef FUNC_H
#define FUNC_H

#define ST_X     100           //棋盘横坐标离屏幕横坐标的距离
#define ST_Y     40            //棋盘纵坐标离屏幕纵坐标的距离
#define SPACE    30

#define X_NUM    29            //横向方格的个数
#define Y_NUM    24            //纵向方格的个数

typedef unsigned int u32_t;
typedef unsigned char u8_t;

//存储屏幕信息的结构体
typedef struct
{
    int w;                         //屏幕的宽度
    int h;                         //屏幕的高度
    int bpp;                       //每个像素点的比特数
    void *memo;
}fbscr_t;

//存储鼠标信息的结构体
typedef struct
{
    int dx;                       //x的相对位移
	int dy;                       //y的相对位移
	char button;                  //按键信息
}mouse_event;

int init_data(void);
int fb_one_pixel(int x, int y, u32_t color);
int fb_line(int x1, int y1, int x2, int y2, u32_t color);
int fb_circle(int x0, int y0, int r, u32_t color);
int print_board();
int draw_cursor(int x, int y);
int save_bg(int x, int y);
int restore(int x, int y);
int get_mouse_info(int fd, mouse_event *p);
int mouse_doing(void);
int chess_doing(void);

#endif
