/***********************************************************
文 件 名：          analyse_message.c
功    能：          解析收到的消息并发送给对应客户端
作    者：          王阁阁 
完成时间：          2010-09-22
修改时间：          2011-09-05 
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "wrap.h"
#include "my_link.h"
#include "analyse_message.h"

#define MAXLINE 200
#define SERV_PORT 8000

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

extern linkk head;   /*声明外部变量链表头指针*/

/**********************************************************
函数名：        user_register
功能：          用户注册
输入参数：id:   用户id存放地址
      connfd:   与客户端连接的文件描述符
返回值：        返回给客户端成功或失败的标记位 
***********************************************************/
char user_register(char *id, int connfd)
{
        linkk p = search(id);/*检测是否该id已被注册*/
	char tmp;            
	int len;
	char line_change = '\n';
	static int sign = 0; /*控制格式*/
	if (p == NULL)
	{
	    int fd = open("./land.txt",O_RDWR | O_APPEND);

	    len = strlen(id);
	    if (sign != 0)
	    {
	        write(fd,&line_change,1);
	    }

	    write(fd,id,len);/*把新注册的用户信息存放在用户信息文件里*/

	    linkk p = make_node(id,-1);/*加载到链表中*/
	    insert(p);

	    close(fd);
	    sign++;
	    tmp = '1';
	}
	else
	{
	    tmp = '0';
	}
    return tmp;
}

/***************************************************************
函数名：         user_link
功能：           用户登录
输入参数：       用户id，文件描述符
返回值：         登录结果标记位
****************************************************************/
char user_link(char *id,int connfd)
{
    char buf[MAXLINE];
    char tmp = '0';

    /*登录服务器后加载用户信息到链表中*/
    if (connfd == 0)
    {
        FILE *pd = fopen("./land.txt","r+");
	if (pd == NULL)
	{
	    perr_exit("fopen land.txt");
	}
        while(fgets(buf,MAXLINE,pd) != NULL)
	{
	    linkk p = make_node(buf,-1);
	    insert(p);
	}
	fclose(pd);
    }

    /*客户端登录*/
    if (connfd != 0)
    {
        linkk p = search(id);
	int len;
	if (p == NULL)
	{
	    tmp = '3';    /*无此用户*/
	}
	else
	{
	    if (strcmp(p->passward,id+8) == 0)
	    {
	        if (p->connfd == -1)
		{
		    tmp = '1';
		    p->connfd = connfd;     /*登录成功*/
		}
		else
		{
		    tmp = '2';           /*用户已在线*/
		}
	    }
	    else
	    {
	        tmp = '0';              /*密码错误*/
	    }
	}
    }
    return tmp;
}

/**************************************************************
函数名：       analyse
功能：         分析收到的信息，并发送给对应客户端
***************************************************************/
void analyse(p_message p_read,int sockfd,int fd)
{
    char tmp_land;
    linkk p;
    char buf[MAXLINE];
    int tmp = 0;

    if(p_read->stamp == REGISTER) /*用户登录*/
    {
         tmp_land = user_link(p_read->mge,sockfd);
         Write(sockfd,&tmp_land,1);
    }
    if(p_read->stamp == REGISTER_P)/*用户注册*/
    {
         tmp_land = user_register(p_read->mge,sockfd);
         Write(sockfd,&tmp_land,1);
    }

    if(p_read->stamp == CAT_ONLINE)/*查看在线用户*/
    {
        for (p=head; p; p=p->next)
	{
	    if (p->connfd != -1)
	    {
	        strcpy(p_read->mge + 9*tmp,p->id);
		tmp++;
	    }
	}
	*(p_read->mge + 9*tmp) = '\0';
	Write(sockfd,p_read,sizeof(*p_read));
    }

    if(p_read->stamp == GROUP)/*群发消息*/
    {
        *buf = '\n';
        strcpy(buf + 1,p_read->id_me);
	strcpy(buf + 9, " to gro / ");
	strcpy(buf + 19,p_read->mge);

	write(fd,buf,strlen(buf));/*保存聊天记录*/

        for (p=head; p; p=p->next)
	{
	    if ((p->connfd != -1) && (p->connfd != sockfd))
	    {
	        Write(p->connfd,p_read,sizeof(*p_read));
	    }
	}
    }
    
    if(p_read->stamp == PERSONAL)/*私聊消息*/
    {
        for (p=head; p; p=p->next)
	{
	    if ((p->connfd != -1) && (strcmp(p->id,p_read->to_id) == 0))
	    {
	        tmp = 1;
	        Write(p->connfd,p_read,sizeof(*p_read));
                *buf = '\n';
                strcpy(buf + 1,p_read->id_me);
	        strcpy(buf + 9, " to ");
	        strcpy(buf + 13, p_read->to_id);
		strcpy(buf + 21," / ");
	        strcpy(buf + 24,p_read->mge);

	        write(fd,buf,strlen(buf));/*保存聊天记录*/
	    }
	}
	if (tmp == 0)
	{
	    p_read->stamp = -1;
	    Write(sockfd,p_read,sizeof(*p_read));
        }
    }
    if((p_read->stamp == TAP_PEOPLE) || (p_read->stamp == FORBID))/*踢人，禁言*/
    {
        for (p=head; p; p=p->next)
	{
	    if ((p->connfd != -1) && (strcmp(p->id,p_read->to_id) == 0))
	    {
	         tmp = 1;
	         Write(p->connfd,p_read,sizeof(*p_read));
	    }
	}
	if (tmp == 0)
	{
	    p_read->stamp = -1;
	    Write(sockfd,p_read,sizeof(*p_read));/*用户不在线*/
        }
    }

    if(p_read->stamp == C_S)        /*关闭服务器*/
    {
        printf("管理员关闭了服务器\n");
        for (p=head; p; p=p->next)
	{
	    if (p->connfd != -1)
	    {
	         Close(p->connfd);
	    }
	}
	sleep(1);
	exit(0);
    }
}
