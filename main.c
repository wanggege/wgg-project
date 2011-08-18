#include <stdio.h>
#include "func.h"
#include "various.h"

int main(int argc, const char *argv[])
{
    init_data();
//	fb_one_pixel(322,345,0x00fff000);
//    fb_circle(500,500,100,0x00ffff00);
    print_board();
//    draw_cursor(500,322);
    mouse_doing();
    return 0;
}
