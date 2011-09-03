#include <stdio.h>
#include <string.h>
#include <time.h>

void itoa(int *p, char *dest)
{
    int i;
   
    for(i = 0; i < 4; i++,dest++)
    {
        *dest = p[i] + '0';
    }
    
    *(dest++) = '\0';
}

int generate_num(char *numstr)
{
   int i = 0;
   int array[4];
   int flag[10] = {0};
   int num;

   srand(time(NULL));
   while(i < 4)
   {
       num = rand() % 10; 
       if(flag[num]!=1)
       {
           array[i] = num;
	   flag[num] = 1;
	   i++;
       }
   }

   itoa(array,numstr);
   return 0;
}

int is_digital(char *str)
{
    int i = 0;
    int j = 0;
    int len = strlen(str);
    
    char *test = str;

    while(*str!='\0')
    {
        if((*str > '9') || (*str < '0')) 
	{
            return 0;	
	}
	str++;
    }

    for(i = 0; i < len; i++)
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

void playagain(char *str)
{
    generate_num(str);
//  printf("a nather num %s\n",str);
}

int analyse_num(char *str, char *correctnum)
{
    int result;
    int i = 0;
    int j = 0;
    int count_same = 0;
    int count_diff = 0;
    char answer[100];
    int again;

    if(strcmp(str,"exit")==0)
    {
        printf("thank you for using\n");
        printf("the correct num is %s\n",correctnum);
	return 0;
    }
   
    if((strlen(str) > 4) || (is_digital(str) == 0))
    {
        printf("Invalid input\n");
        return 1;
    }

//        printf("the correct num is %s\n",correctnum);
    for(i = 0; i < strlen(correctnum); i++)
    {
        for(j = 0; j < strlen(str); j++) 
	{
            if(correctnum[i] == str[j])	
	    {
	        if(i==j) 
		{
	            count_same++;	
		}
		else
		{
		    count_diff++;
		}
	    }
	}
    }

    if((count_same>0) && (count_same < 4))
    {
        printf("%dA",count_same); 
    }
    else if(count_same == 4)
         {
	     printf("congratulations ,you win!\n"); 
	     printf("Do you want play again ? please answer yes or no.\n");
	     fgets(answer, sizeof(answer), stdin);
	     answer[strlen(answer) - 1] = '\0';
	     if(strcmp(answer, "no")==0)
	     {
	         printf("thank you for using~!\n");
	         return 0; 
	     }
	     else if(strcmp(answer, "yes") == 0)
	          {
		      memset(correctnum,'0',sizeof(correctnum));
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
        printf("%dB\n",count_diff);  
    }
 
    return 1;
}

int main()
{
    char numstr[5];
    char guess_num[100];
    char quit[] = "exit";
    int flag = 1;

    printf("hi friend! please input your guess number, good luck\n");
    generate_num(numstr);
    while(flag)
    {
        fgets(guess_num, sizeof(guess_num), stdin); 
	guess_num[strlen(guess_num) - 1] = '\0';
	flag = analyse_num(guess_num, numstr);
    }
     
    return 0;
}
