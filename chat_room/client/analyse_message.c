/*********************************************************
文件名：           analyse_message.c
功能：             解析消息
作者：             王阁阁 
完成时间：         2010-09-22
修改时间：         2011-09-05 
**********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include "wrap.h"
#include "analyse_message.h"

#define MAXLINE        200       /*缓存区大小*/

/****************发送的消息类型标记***************************/
#define REGISTER         0      /*登录*/
#define CAT_ONLINE       1      /*查看在线用户*/
#define GROUP            2      /*群发消息*/
#define PERSONAL         3      /*私聊消息*/
#define EXIT             4      /*下线*/
#define TAP_PEOPLE       5      /*踢人*/
#define FORBID           6      /*禁言*/
#define REGISTER_P       9      /*注册*/
#define C_S              8      /*关闭服务器*/

#define LOGIN            1      /*登录入口*/
#define REGISTE          2      /*注册入口*/

extern int sign_forbid;         /*声明禁言标记位*/

/***************************************************************
函数名：       my_strlen
功能：         计算以\n结束的字符串的长度
输入参数：     字符串首地址
返回值：       字符串长度
****************************************************************/
int my_strlen(char *buf)
{
    int len = 0;      /*字符串长度*/

    if (buf == NULL)  /*出错处理*/
    {
        return 0;
    }
    while (*buf++ != '\n')
    {
        len++;
    }
    return len;
}

/***************************************************************
函数名：           analyse_read
功能：             解析从终端读入的字符串信息
输入参数：buf:     缓存区地址
      p_message:   发送结构体的地址
****************************************************************/
int analyse_read(char *buf,linkk p_message)
{
    if (strncmp(buf,"cat",3) == 0)  /*查看在线用户*/
    {
        return CAT_ONLINE;
    }
    
    if (strncmp(buf,"gro/",4) == 0) /*群发*/
    {
        return GROUP;
    }

    if (strncmp(buf,"exit",4) == 0) /*退出*/
    {
        exit(0);
    }

    if ((strncmp(buf,"cs",2) == 0) && (strncmp(p_message->id_me,"zchadmin",8) == 0))   /*关闭服务器*/
    {
        return C_S;
    }
    if ((strncmp(buf,"tap",3) == 0) && (strncmp(p_message->id_me,"zchadmin",8) == 0))    /*管理员踢人*/
    {
        if ((my_strlen(buf) - 4) != 8)
	{
	    return -1;
	}
        strncpy(p_message->to_id,buf + 4, 8);
	*(p_message->to_id + 8) = '\0';
        return TAP_PEOPLE;
    }

    if ((strncmp(buf,"forbid",6) == 0) && (strncmp(p_message->id_me,"zchadmin",8) == 0))  /*管理员禁言*/
    {
        if ((my_strlen(buf) - 7) != 8)
	{
	    return -1;
	}
        strncpy(p_message->to_id,buf + 7, 8);
	*(p_message->to_id + 8) = '\0';
        return FORBID;
    }

    if (*(buf + 8) == '/') /*私聊*/
    {
        return PERSONAL;
    }
    return -1;
}

/******************************************************************
函数名：             analyse_message_read
功能：               将读到的缓存区数据解析成可以发送的结构体消息
输入参数：buf：      缓存区首地址
输出参数：p_message: 可发送的结构体指针
******************************************************************/
int analyse_message_read(linkk p_message,char *buf,int fd)
{
    int len_mge;
    char *dest = "\n    to ";

    int tmp = analyse_read(buf,p_message);/*解析消息*/
    p_message->stamp = tmp;           /*控制消息类型*/
    len_mge = my_strlen(buf);
    switch (tmp)
    {
        case -1:
	      {
	          printf("请按正确的格式输入\n");
		  return -1;
	      }
	case 2:
	      {
		  strncpy(p_message->mge,buf + 4,len_mge - 4);/*群发*/
		  *(p_message->mge + len_mge -4) = '\0';
                   
		  /*保存聊天记录*/
		  write(fd,dest,8);
		  write(fd,buf,len_mge);

		  break;
	      }
	case 3:
	      {
	           strncpy(p_message->to_id,buf,8); /*私聊*/
		   *(p_message->to_id + 8) = '\0';
		   strncpy(p_message->mge,buf +9,len_mge-9);
		   *(p_message->mge + len_mge -9) = '\0';

		   /*保存聊天记录*/
		   write(fd,dest,8);
		   write(fd,buf,len_mge);

		   break;
	      }
	default:
	       ;
    }
    return tmp;
}

/************************************************************
函数名：          express_write
功能：            特殊输入代表的表情控制函数
输入参数：        接收到的消息结构体
**************************************************************/
void express_write(linkk p_message)
{
   if (*(p_message->mge) == ':')
   {    
        if(p_message->stamp == 2)
	{
            printf("from %s(gro) : %s向大家做了个鬼脸\n",p_message->id_me,p_message->id_me);
	}
	else
	{
            printf("from %s : %s向你做了个鬼脸\n",p_message->id_me,p_message->id_me);
	}
   }
   
   if (strncmp(p_message->mge,"welcome",7) == 0)
   {
        printf("from %s : %s大侠，欢迎你来到咱们的聊天室\n",p_message->id_me,p_message->to_id);
   }
}

/*********************************************************************
函数名：               analyse_message
功能：                 从服务器接收到的消息解析函数
输入参数： p_message:  接收的消息结构体指针
           fd：        保存聊天记录的文件描述符
***********************************************************************/
void analyse_message_write(linkk p_message, int fd)
{
    char buf[MAXLINE] = "\n    from ";

    if (p_message->stamp == CAT_ONLINE)  /*打印在线用户*/
    {
        char *dest = p_message->mge;
        while (*dest != '\0')
	{
            printf("%s\n",dest);
            dest = dest + 9;
	}
    }

    if (p_message->stamp == GROUP)  /*接收到群发消息*/
    {
        if (*(p_message->mge) == ':')
	{
	    express_write(p_message);   /*表情*/
	}
	else
	{
            printf("from %s(gro) : %s\n",p_message->id_me,p_message->mge);
	    strcpy(buf + 10, p_message->id_me);
	    strncpy(buf + 18, "(gro)/",6);
	    strcpy(buf + 24,p_message->mge);

            write(fd,buf,strlen(buf));  /*保存聊天记录*/
	}
    }

    if (p_message->stamp == PERSONAL)  /*接收到私聊*/
    {
        if ((*(p_message->mge) == ':') || (strncmp(p_message->mge,"welcome",6) == 0))/*表情*/
	{
	    express_write(p_message);   
	}
	else
	{
            printf("from %s : %s\n",p_message->id_me,p_message->mge);
	    strcpy(buf + 10, p_message->id_me);
	    *(buf + 18) = '/';
	    strcpy(buf + 19,p_message->mge);

            write(fd,buf,strlen(buf));  /*保存聊天记录*/
	}
    }

    if (p_message->stamp == -1)
    {
        printf("用户 %s 未注册或不在线\n",p_message->to_id);
    }

    if (p_message->stamp == TAP_PEOPLE)  /*踢人*/
    {
        printf("你被管理员踢出聊天室\n");
	sleep(1);
	exit(0);
    }

    if (p_message->stamp == FORBID)   /*禁言*/
    {
        sign_forbid = -sign_forbid;
	if (sign_forbid == -1)
            printf("你已被管理员禁言，只能收到别人的消息\n");
	if (sign_forbid == 1)
	    printf("你的禁言状态已被解除，你可以正常聊天了\n");
    }
}
