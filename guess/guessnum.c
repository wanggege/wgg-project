#include <stdio.h>
#include <string.h>
#include <time.h>

/*****************************
*函 数 名：itoa
*函数功能：将整型数组转换成字符数组
*作    者：王阁阁
*日    期：2011-09-03
*******************************/

void itoa(int *p, char *dest)
{
    int i;
   
    for(i = 0; i < 4; i++,dest++)
    {
        *dest = p[i] + '0';
    }
    
    *(dest++) = '\0';
}

/*****************************
*函 数 名：generate_num
*函数功能：产生随机四位0～9之间的数字
*作    者：王阁阁
*日    期：2011-09-03
*******************************/
int generate_num(char *numstr)
{
   int i = 0;
   int array[4];                 //产生的四位随机数存放的数组
   int flag[10] = {0};           //已产生的数字标记数组
   int num;

   srand(time(NULL));
   while(i < 4)
   {
       num = rand() % 10; 
       if(flag[num]!=1)               //数否产生相同的数字
       {
           array[i] = num;
	   flag[num] = 1;
	   i++;
       }
   }

   itoa(array,numstr);
   return 0;
}

/*****************************
*函 数 名：is_digital
*函数功能：判断玩家输入的数字字符串是否合法
*作    者：王阁阁
*日    期：2011-09-03
*******************************/

int is_digital(char *str)
{
    int i = 0;
    int j = 0;
    int len = strlen(str);
    
    char *test = str;

    while(*str!='\0')         //判断玩家猜测的字符串是否是数字字符串
    {
        if((*str > '9') || (*str < '0')) 
	{
            return 0;	
	}
	str++;
    }

    for(i = 0; i < len; i++)    //判断玩家猜测的字符串中是否有重复数字
    {
        for(j = i + 1; j < len; j++) 
	{
            if(test[i] == test[j])	
	    {
	        return 0; 
	    }
	}
    }

    return 1;
}

/*****************************
*函 数 名：playagain
*函数功能：游戏重新初始化，产生新的四位随机数字字符串
*作    者：王阁阁
*日    期：2011-09-03
*******************************/
void playagain(char *str)
{
    generate_num(str);
//  printf("a nather num %s\n",str);
}

/*****************************
*函 数 名：analyse_num
*函数功能：解析玩家猜测的字符串是否正确，游戏是否结束
*作    者：王阁阁
*日    期：2011-09-03
*******************************/
int analyse_num(char *str, char *correctnum)
{
    int result;
    int i = 0;
    int j = 0;
    int count_same = 0;             //数字相同且位置相同的数字个数
    int count_diff = 0;             //数字相同但位置不同的数字个数
    char answer[100];               //是否再完一局

    if(strcmp(str,"exit")==0)        //玩家结束游戏
    {
        printf("thank you for using\n");
        printf("the correct num is %s\n",correctnum);
	return 0;
    }
   
    if((strlen(str) > 4) || (is_digital(str) == 0)) //判断输入是否合法
    {
        printf("Invalid input\n");
        return 1;
    }

    //解析玩家字符串正确情况
    for(i = 0; i < strlen(correctnum); i++)
    {
        for(j = 0; j < strlen(str); j++) 
	{
            if(correctnum[i] == str[j])	
	    {
	        if(i==j)                      //数字相同且位置相同
		{
	            count_same++;	
		}
		else                          //数字相同但位置不同
		{
		    count_diff++;
		}
	    }
	}
    }

    if((count_same>0) && (count_same < 4))        //打印本次猜测的结果
    {
        printf("%dA",count_same);              //数字相同位置也相同用A的个数表示
    }
    else if(count_same == 4)                   //猜测结果正确
         {
	     printf("congratulations ,you win!\n"); 
	     printf("Do you want play again ? please answer yes or no.\n");
	     fgets(answer, sizeof(answer), stdin);//是否再玩一次
	     answer[strlen(answer) - 1] = '\0';
	     if(strcmp(answer, "no")==0)         //游戏结束
	     {
	         printf("thank you for using~!\n");
	         return 0; 
	     }
	     else if(strcmp(answer, "yes") == 0)  //游戏继续
	          {
		      memset(correctnum,'0',sizeof(correctnum)); //重新初始化游戏
		      playagain(correctnum); 
		      return 1;
		  }
		  else
		  {
		      printf("please input yes or no!\n");
		  }
	 }

    if(count_diff>0)
    {
        printf("%dB",count_diff);  
    }
    printf("\n");
//    printf("%s\n",numstr);
 
    return 1;
}

int main()
{
    char numstr[5];         //要猜的四位数
    char guess_num[100];    //玩家猜的数字
    char quit[] = "exit";   //游戏结束
    int flag = 1;

    printf("hi friend! please input your guess number, good luck\n");
    generate_num(numstr);    //产生四位随机数，即玩家猜的正确数字
    while(flag)
    {
        fgets(guess_num, sizeof(guess_num), stdin); //接受玩家输入的数字并处理
	guess_num[strlen(guess_num) - 1] = '\0';
	flag = analyse_num(guess_num, numstr);     //解析玩家输入的数字
    }
     
    return 0;
}
