#include <stdio.h>
#include "func.h"
#include "various.h"

int main(int argc, const char *argv[])
{
    init_data();
	fb_one_pixel(322,345,0x00fff000);
    return 0;
}
