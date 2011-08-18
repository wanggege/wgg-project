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

#endif
