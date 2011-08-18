#ifndef FUNC_H
#define FUNC_H

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


#endif
