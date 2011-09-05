/**************************************************************
文 件 名：     select_server.c
功    能：     设置服务器属性
作    者：     王阁阁 
完成时间：     2010-09-22
修改时间：     2011-09-05
**************************************************************/

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

#define MAXLINE 200                /*缓存区大小*/
#define SERV_PORT 8000             /*服务器端口*/

int main(int argc, char **argv)
{
	int i, maxi, maxfd, listenfd, connfd, sockfd;    
	int nready, client[FD_SETSIZE];                  /*监听存放数组*/
	ssize_t n;
	fd_set rset, allset;                           /*select监听的文件集*/

	char buf[MAXLINE];                           /*用户信息读取缓存区*/
	char str[INET_ADDRSTRLEN];

	socklen_t cliaddr_len;
	struct sockaddr_in  cliaddr, servaddr;       /*服务器参数结构体*/

	p_message p_struct_message;                  /*接收，发送消息结构体*/
	linkk p_link;                                /*用户信息结构体指针*/

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(listenfd,SOL_SOCKET,1,&opt,sizeof(opt));

	bzero(&servaddr, sizeof(servaddr));          /*清空服务器结构体缓冲区*/
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	Listen(listenfd, 20);
        
        user_link(buf,0);               /*加载用户信息到链表*/

        p_struct_message = (p_message)malloc(sizeof(*p_struct_message)); 
	if(p_struct_message==NULL)
	{
  		perr_exit("malloc");       	
	}

	maxfd = listenfd;	/*当前监听是文件描述符最大数目*/	
	maxi = -1;			

	/*初始化与服务器链接的客户端文件描述符存放集*/
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;	     

	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);  /*把服务器加到文件读集中*/

	/*打开存放用户登录信息的文件*/
        int fd = open("./chat_record.txt",O_RDWR | O_APPEND | O_CREAT,0644);
        if (fd == -1)
        {
            perr_exit("open chat_record:");
        }

	while(1)
	{
	    rset = allset;	       /*初始化读集 */
	    nready = select(maxfd+1, &rset, NULL, NULL, NULL);
	    if (nready < 0)
		perr_exit("select error");

	    if (FD_ISSET(listenfd, &rset)) /*新的客户端连接请求*/
	    {
	        cliaddr_len = sizeof(cliaddr);
		connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

		printf("received from %s at PORT %d\n",
		inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
		ntohs(cliaddr.sin_port));

		for (i = 0; i < FD_SETSIZE; i++)
		{
		    if (client[i] < 0) 
		    {
			client[i] = connfd; /*存放描述符*/
			break;
	            }
		 }
		if (i == FD_SETSIZE) 
		{
			fputs("too many clients\n", stderr);
			exit(1);
		}

		FD_SET(connfd, &allset);	/*添加新的文件描述符到读集*/
		if (connfd > maxfd)
			maxfd = connfd;      
		if (i > maxi)
			maxi = i;	

		if (--nready == 0)          
			continue;
		}

		for (i = 0; i <= maxi; i++) 
		{	
	            if ( (sockfd = client[i]) < 0)   /*select监听的字符集中有客户端活动*/
			continue;
		    if (FD_ISSET(sockfd, &rset)) 
		    {
			if ( (n = Read(sockfd, p_struct_message, MAXLINE)) == 0) 
			{
			    Close(sockfd);           /*客户端关闭*/
			    FD_CLR(sockfd, &allset); /*清除关闭的客户端*/
		      	    client[i] = -1;
		            p_link = search_connfd(sockfd);
			    if (p_link != NULL)
			    {
			        p_link->connfd = -1;
			    }
			}
			else 
			{
                            analyse(p_struct_message,sockfd,fd);
			}

			if (--nready == 0)
			    break;	/*处理结束*/
			}
		}
	}
}
