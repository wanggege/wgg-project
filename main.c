#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

typedef struct
{
    int xres;
    int yres;
    int bits_per_pixel;
}fd_var_screeninfo;

int main(int argc, const char *argv[])
{
    int fd = 0;
    char * fbp;
    fd_var_screeninfo vinfo;

    long int screensize = 0;
    fd = open("/dev/fd0",O_RDWR);
    ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
    screensize = vinfo.xres*vinfo.yres*vinfo.bits_per_pixel / 8;
    fbp = (char *)mmap(0, screensize, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
    printf("screensize:xres = %d,yres = %d\n %ld\n",vinfo.xres,vinfo.yres,screensize);
    return 0;
}
