#include <stdio.h>
#define HOW_MANY_TIMES_TO_SAY_HELLO 10

void say_three_hello();

int main(int argc,const char * argv[])
{
    int j;

    for (j = 0; j < HOW_MANY_TIMES_TO_SAY_HELLO; j++) 
    {
        say_three_hello();
    }

    return 0;
}

void say_three_hello()
{
    printf("helloworld\n");
}
